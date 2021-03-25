/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcBitcoinBlockTemplate
 *            Code is inspired from libblkmaker
 */
#include "CcBitcoinBlockTemplate.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcStatic.h"
#include "Hash/CcSha256.h"

#define BLOCKHEADER_SIZE        80
#define max_varint_size         (9)

#define OP_CHECKSIG             0xac
#define OP_CHECKSIGVERIFY       0xad
#define OP_CHECKMULTISIG        0xae
#define OP_CHECKMULTISIGVERIFY  0xaf

void CBitcoinHash::operator=(const CBitcoinHash& oToCopy)
{
  CcStatic::memcpy(pHash, oToCopy.pHash, sizeof(oToCopy.pHash));
}

static inline
uint16 upk_u16le(const void* bufp, const size_t offset)
{
  const uint8 * buf = static_cast<const uint8*>(bufp);
  return (((uint16)buf[offset + 0]) << 0)
    | (((uint16)buf[offset + 1]) << 8);
}

static inline
uint32 upk_u32le(const void* bufp, const size_t offset)
{
  const uint8* buf = static_cast<const uint8*>(bufp);
  return (((uint32)buf[offset + 0]) << 0)
    | (((uint32)buf[offset + 1]) << 8)
    | (((uint32)buf[offset + 2]) << 0x10)
    | (((uint32)buf[offset + 3]) << 0x18);
}

static inline
uint64 upk_u64le(const void* bufp, const size_t offset)
{
  const uint8* buf = static_cast<const uint8*>(bufp);
  return (((uint64)buf[offset + 0]) << 0)
    | (((uint64)buf[offset + 1]) << 8)
    | (((uint64)buf[offset + 2]) << 0x10)
    | (((uint64)buf[offset + 3]) << 0x18)
    | (((uint64)buf[offset + 4]) << 0x20)
    | (((uint64)buf[offset + 5]) << 0x28)
    | (((uint64)buf[offset + 6]) << 0x30)
    | (((uint64)buf[offset + 7]) << 0x38);
}

size_t varintDecode(const uint8 *p, size_t size, uint64 *n)
{
  if (size > 8 && p[0] == 0xff)
  {
    *n = upk_u64le(p, 1);
    return 9;
  }
  if (size > 4 && p[0] == 0xfe)
  {
    *n = upk_u32le(p, 1);
    return 5;
  }
  if (size > 2 && p[0] == 0xfd)
  {
    *n = upk_u16le(p, 1);
    return 3;
  }
  if (size > 0 && p[0] <= 0xfc)
  {
    *n = p[0];
    return 1;
  }
  return 0;
}

char varintEncode(unsigned char *out, uint64 n)
{
  if (n < 0xfd)
  {
    out[0] = static_cast<unsigned char>(n);
    return 1;
  }
  char L;
  if (n <= 0xffff)
  {
    out[0] = U'\xfd';
    L = 3;
  }
  else
    if (n <= 0xffffffff)
    {
      out[0] = U'\xfe';
      L = 5;
    }
    else
    {
      out[0] = U'\xff';
      L = 9;
    }
  for (unsigned char i = 1; i < L; ++i)
    out[i] = (n >> ((i - 1) * 8)) % 256;
  return L;
}

static int64 blkmk_calc_gentx_weight(const void* CCUNUSED_PARAM(data), const size_t datasz) 
{
  return (datasz * 4) + 2 /* marker & flag */ + 1 /* witness stack count */ + 1 /* stack item size */ + 32 /* stack item: nonce */;
}

int16 blkmk_count_sigops(const uint8 * const script, const size_t scriptsz, const bool bip141) 
{
  int16 sigops = 0;
  for (size_t i = 0; i < scriptsz; ++i) 
  {
    if (script[i] <= 0x4c /* OP_PUSHDATA1 */) 
    {
      if (script[i] == 0x4c) 
      {
        if (i + 1 >= scriptsz) 
        {
          break;
        }
        ++i;
      }
      i += script[i];
    }
    else if (script[i] == 0x4d /* OP_PUSHDATA2 */) 
    {
      if (i + 2 >= scriptsz) 
      {
        break;
      }
      i += 2 + upk_u16le(script, i + 1);
    }
    else if (script[i] == 0x4e /* OP_PUSHDATA4 */) 
    {
      if (i + 4 >= scriptsz) {
        break;
      }
      i += 4 + upk_u32le(script, i + 1);
    }
    else if (script[i] == OP_CHECKSIG || script[i] == OP_CHECKSIGVERIFY)
    {
      ++sigops;
    }
    else if (script[i] == OP_CHECKMULTISIG || script[i] == OP_CHECKMULTISIGVERIFY)
    {
      sigops += 20;
    }
  }
  if (bip141) 
  {
    sigops *= 4;
  }
  return sigops;
}

static uint8 blkmk_varint_encode_size(const uint64 n)
{
  uint8 dummy[max_varint_size];
  return varintEncode(dummy, n);
}


bool CcBitcoinBlockTemplate::createCoinbase()
{
  bool bRet = false;
  oCoinbaseData.resize(168 + oCoinbaseExtraData.size());
  uint32 uiOffset = 0;

  CcStatic::memcpy(oCoinbaseData.getArray(),
                   "\x01\0\0\0"																													// txn ver
                   "\x01"																																// input count
                   "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"		// prevout
                   "\xff\xff\xff\xff"																										// index (-1)
                   "\x02"																																// scriptSig length
                   "\0"	  																															// height serialization length (set later)
                   , 43);
  uiOffset += 43;

  // Add Block height in litle endian format
  uint32 uiHeight = uiBlockHeight;
  while (uiHeight > 127)
  {
    ++oCoinbaseData[41];
    oCoinbaseData[uiOffset++] = uiOffset & 0xff;
    uiHeight >>= 8;
  }
  oCoinbaseData[uiOffset++] = uiOffset & 0xff;
  oCoinbaseData[42] = oCoinbaseData[41] - 1;

  // Template aux data (not set in bitcoin core)

  //- Template aux data (not set in bitcoin core)

  CcStatic::memcpy(&oCoinbaseData[uiOffset],
                   "\xff\xff\xff\xff"  // sequence
                   "\x01"							// output count
                   , 5
  );
  uiOffset += 5;

  CcStatic::swap(oCoinbaseData.getArray(uiOffset), &uiCoinbaseValue, sizeof(uiCoinbaseValue));
  uiOffset += 8;
  oCoinbaseData[uiOffset++] = static_cast<uint8>(oCoinbaseExtraData.size());
  oCoinbaseExtraData.read(oCoinbaseData.getArray(uiOffset), oCoinbaseExtraData.size());
  uiOffset += static_cast<uint32>(oCoinbaseExtraData.size());

  CcStatic::memset(oCoinbaseData.getArray(uiOffset), 0, 4);
  uiOffset += 4;

  const unsigned long pretx_size = BLOCKHEADER_SIZE + blkmk_varint_encode_size(1 + oTransactions.size());
  const int16 sigops_counted = blkmk_count_sigops(reinterpret_cast<uint8*>(oCoinbaseData.getArray()), oCoinbaseData.size(), bIsSegwitEnabled);
  const int64 gentx_weight = blkmk_calc_gentx_weight(oCoinbaseData.getArray(), uiOffset);
  // Ignor value verifiaction value at the moment
  //if (pretx_size + tmpl->txns_datasz + off > tmpl->sizelimit
  // || (tmpl->txns_weight >= 0 && tmpl->txns_weight + gentx_weight > tmpl->weightlimit)
  // || (tmpl->txns_sigops >= 0 && tmpl->txns_sigops + sigops_counted > tmpl->sigoplimit)) {
  //	free(data);
  //	return 0;
  //}

  return bRet;
}
