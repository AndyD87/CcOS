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
 * @brief     Implementation of Class CcSha256
 */
#include "Hash/CcSha256.h"
#include "CcArray.h"
#include "CcString.h"

//! Size of data buffer
#define SHA256_DATASIZE (16*sizeof(uint32))
//! Cast from char to uint32
#define CAST_CHAR_TO_UINT32(a) static_cast<uint32>(static_cast<uchar>(a))

const uint32 CcSha256::c_aInitState[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
const uint32 CcSha256::c_aK[64] = {
  0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
  0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
  0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
  0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
  0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
  0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
  0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
  0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
  0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
  0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
  0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
  0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
  0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
  0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
  0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
  0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};
//! SHA256 e0 method
#define e0(x)         (doRor32(x, 2) ^ doRor32(x,13) ^ doRor32(x,22))
//! SHA256 e1 method
#define e1(x)         (doRor32(x, 6) ^ doRor32(x,11) ^ doRor32(x,25))
//! SHA256 ch method
#define doCh(x,y,z)   (((x) & (y)) ^ (~(x) & (z)))
//! SHA256 maj method
#define doMaj(x,y,z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

CcSha256::CcSha256() :
  m_oResult((size_t)32)
{
  m_aState[0] = c_aInitState[0];
  m_aState[1] = c_aInitState[1];
  m_aState[2] = c_aInitState[2];
  m_aState[3] = c_aInitState[3];
  m_aState[4] = c_aInitState[4];
  m_aState[5] = c_aInitState[5];
  m_aState[6] = c_aInitState[6];
  m_aState[7] = c_aInitState[7];
}

CcSha256::CcSha256(const CcByteArray& oInputData) :
  m_oResult((size_t)32)
{
  m_aState[0] = c_aInitState[0];
  m_aState[1] = c_aInitState[1];
  m_aState[2] = c_aInitState[2];
  m_aState[3] = c_aInitState[3];
  m_aState[4] = c_aInitState[4];
  m_aState[5] = c_aInitState[5];
  m_aState[6] = c_aInitState[6];
  m_aState[7] = c_aInitState[7];
  generateByteArray(oInputData);
}

CcSha256::~CcSha256()
{
}

bool CcSha256::operator==(const CcSha256& oToCompare) const
{
  return m_oResult == oToCompare.m_oResult;
}

bool CcSha256::operator!=(const CcSha256& oToCompare) const
{
  return m_oResult != oToCompare.m_oResult;
}

CcSha256& CcSha256::operator=(const CcByteArray& oByteArray)
{
  return generate(oByteArray.getArray(), oByteArray.size());
}

CcSha256& CcSha256::operator=(const CcString& sString)
{
  return generate(sString.getCharString(), sString.length());
}

CcSha256& CcSha256::generate(const void* pcData, size_t uiLen)
{
  return finalize(pcData, uiLen);
}

CcSha256& CcSha256::append(const void* pcData, size_t uiLen)
{
  size_t uiOffset = 0;
  while (uiOffset + (SHA256_DATASIZE-1) < uiLen)
  {
    doTransform(reinterpret_cast<const uchar*>(pcData) + uiOffset);
    uiOffset += SHA256_DATASIZE;
    m_uiLength += SHA256_DATASIZE;
  }
  return *this;
}

CcSha256& CcSha256::finalize(const void* pvData, size_t uiLen)
{
  const char* pcData = static_cast<const char*>(pvData);
  if (uiLen > SHA256_DATASIZE)
  {
    size_t uiStart = uiLen & ~static_cast<size_t>(SHA256_DATASIZE - 1);
    append(pcData, uiStart);
    uiLen = uiLen - uiStart;
    pcData += uiStart;
  }
  CcArray<uchar> aData(64);
  size_t i, j;
  for (i = 0; i < uiLen; i++) aData[i] = static_cast<uchar>(pcData[i]);
  if (uiLen < 56) 
  {
    aData[i++] = 0x80;
    while (i < 56)
      aData[i++] = 0x00;
  }
  else 
  {
    aData[i++] = 0x80;
    while (i < 64)
      aData[i++] = 0x00;
    doTransform(aData.getArray());
    CcStatic::memset(aData.getArray(), 0, 56);
  }
  m_uiLength += uiLen;

  uint64 uiBitLen = m_uiLength * 8;
  aData[63] = static_cast<char>((uiBitLen      ) & 0xff);
  aData[62] = static_cast<char>((uiBitLen >>  8) & 0xff);
  aData[61] = static_cast<char>((uiBitLen >> 16) & 0xff);
  aData[60] = static_cast<char>((uiBitLen >> 24) & 0xff);
  aData[59] = static_cast<char>((uiBitLen >> 32) & 0xff);
  aData[58] = static_cast<char>((uiBitLen >> 40) & 0xff);
  aData[57] = static_cast<char>((uiBitLen >> 48) & 0xff);
  aData[56] = static_cast<char>((uiBitLen >> 56) & 0xff);

  doTransform(aData.getArray());

  for (i = j = 0; i < 8; ++i, j += 4)
  {
    m_oResult[j + 0] = static_cast<char>((m_aState[i] >> 24) & 0xff);
    m_oResult[j + 1] = static_cast<char>((m_aState[i] >> 16) & 0xff);
    m_oResult[j + 2] = static_cast<char>((m_aState[i] >> 8 ) & 0xff);
    m_oResult[j + 3] = static_cast<char>((m_aState[i]      ) & 0xff);
  }
  return *this;
}

void CcSha256::setMidstate(const CcByteArray& oMidstate, size_t uiLength)
{
  for (int i=0, j=0; i < 8; i++, j+=4 )
  {
    uint32 uiTemp = (uint8)oMidstate[j] << 24;
    uiTemp |= (uint8)oMidstate[j + 1] << 16;
    uiTemp |= (uint8)oMidstate[j + 2] << 8;
    uiTemp |= (uint8)oMidstate[j + 3];
    m_aState[i] = uiTemp;
  }
  m_uiLength = uiLength;
}

uint32 CcSha256::doRor32(uint32 word, unsigned int shift)
{
  return (word >> shift) | (word << (32 - shift));
}

void CcSha256::doTransform(const uchar *input)
{
  uint32 a, b, c, d, e, f, g, h, t1, t2;
  uint32 W[64];
  int i, j;

  for (i = j = 0; i < 16; ++i, j+=4 )
  {
    W[i] =  (input[j] << 24)     | 
            (input[j + 1] << 16) | 
            (input[j + 2] << 8)  | 
            (input[j + 3]);
  }

  for (; i < 64; ++i)
  {
    W[i] = s1(W[i - 2]) + W[i - 7] + s0(W[i - 15]) + W[i - 16];
  }

  /* load the state into our registers */
  a = m_aState[0];  b = m_aState[1];  c = m_aState[2];  d = m_aState[3];
  e = m_aState[4];  f = m_aState[5];  g = m_aState[6];  h = m_aState[7];

  /* now iterate */
  for (i = 0; i < 64; i++)
  {
    t1 = h + e1(e) + doCh(e, f, g) + c_aK[i] + W[i];
    t2 = e0(a) + doMaj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  m_aState[0] += a; m_aState[1] += b; m_aState[2] += c; m_aState[3] += d;
  m_aState[4] += e; m_aState[5] += f; m_aState[6] += g; m_aState[7] += h;

  /* clear any sensitive info... */
  a = b = c = d = e = f = g = h = t1 = t2 = 0;
}
