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
 * @brief     Implementation of Class CcSha1
 */
#include "Hash/CcSha1.h"
#include "CcArray.h"
#include "CcString.h"

//! Size of data buffer
#define SHA1_DATASIZE (8*sizeof(uint32))
#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))
#define W(t)       w[t&0x0F]

//! SHA-1 auxiliary functions
#define CH(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define PARITY(x, y, z) ((x) ^ (y) ^ (z))
#define MAJ(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))



const uint32 CcSha1::c_aInitState[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
const uint32 CcSha1::c_aK[4] =
{
   0x5A827999,
   0x6ED9EBA1,
   0x8F1BBCDC,
   0xCA62C1D6
};

CcSha1::CcSha1() :
  m_oContextData((size_t)64),
  m_oResult((size_t)20)
{
  initValues();
}

CcSha1::CcSha1(const CcByteArray& oInputData) :
  CcSha1()
{
  generateByteArray(oInputData);
}

CcSha1::~CcSha1()
{
}

bool CcSha1::operator==(const CcSha1& oToCompare) const
{
  return m_oResult == oToCompare.m_oResult;
}

bool CcSha1::operator!=(const CcSha1& oToCompare) const
{
  return m_oResult != oToCompare.m_oResult;
}

CcSha1& CcSha1::operator=(const CcByteArray& oByteArray)
{
  return generate(oByteArray.getArray(), oByteArray.size());
}

CcSha1& CcSha1::operator=(const CcString& sString)
{
  return generate(sString.getCharString(), sString.length());
}

CcSha1& CcSha1::generate(const void* pcData, size_t uiLen)
{
  initValues();
  return finalize(pcData, uiLen);
}

CcSha1& CcSha1::append(const void* pcData, size_t uiLen)
{
  size_t uiOffset = 0;
  while (uiOffset + (SHA1_DATASIZE-1) < uiLen)
  {
    doTransform(reinterpret_cast<const uchar*>(pcData) + uiOffset);
    uiOffset += SHA1_DATASIZE;
    m_uiLength += SHA1_DATASIZE;
  }
  return *this;
}

CcSha1& CcSha1::finalize(const void* pvData, size_t uiLen)
{
  uint32 i;
  size_t paddingSize;
  const uchar* pData = static_cast<const uchar*>(pvData);
  while (uiLen > 64)
  {
    doTransform(pData);
    pData += 64;
    uiLen -= 64;
    m_uiLength += 64;
  }

  //Length of the original message (before padding)
  m_uiLength += uiLen;
  uint64_t totalSize = m_uiLength * 8;

  //Pad the message so that its length is congruent to 56 modulo 64
  if (uiLen < 56)
  {
    paddingSize = 56 - uiLen;
  }
  else
  {
    paddingSize = 64 + 56 - uiLen;
  }

  CcByteArray aData;
  aData.resize(64, 0);
  aData.write(pData, uiLen);
  aData.cast<uint8>()[uiLen] = 0x80;
  uint32* w = aData.cast<uint32>();

  //Append the length of the original message
  w[14] = CcStatic::swapUint32((uint32)(totalSize >> 32));
  w[15] = CcStatic::swapUint32((uint32)totalSize);

  doTransform(aData.cast<uchar>());

  //Convert from host byte order to big-endian byte order
  uint32* h = m_oResult.cast<uint32>();
  for (i = 0; i < 5; i++)
  {
    h[i] = CcStatic::swapUint32(m_aState[i]);
  }

  return *this;
}

void CcSha1::setMidstate(const CcByteArray& oMidstate, size_t uiLength)
{
  for (int i=0, j=0; i < 5; i++, j+=4 )
  {
    uint32 uiTemp = (uint8)oMidstate[j] << 24;
    uiTemp |= (uint8)oMidstate[j + 1] << 16;
    uiTemp |= (uint8)oMidstate[j + 2] << 8;
    uiTemp |= (uint8)oMidstate[j + 3];
    m_aState[i] = uiTemp;
  }
  m_uiLength = uiLength;
}

void CcSha1::initValues()
{
  m_uiLength = 0;
  m_aState[0] = c_aInitState[0];
  m_aState[1] = c_aInitState[1];
  m_aState[2] = c_aInitState[2];
  m_aState[3] = c_aInitState[3];
  m_aState[4] = c_aInitState[4];
}

void CcSha1::doTransform(const uchar *input)
{
  //Initialize the 5 working registers
  uint32 temp = 0;
  uint32 a = m_aState[0];
  uint32 b = m_aState[1];
  uint32 c = m_aState[2];
  uint32 d = m_aState[3];
  uint32 e = m_aState[4];

  //Process message in 16-word blocks
  m_oContextData.write(input, 64);
  uint32* w = m_oContextData.cast<uint32>();


  //Convert from big-endian byte order to host byte order
  for (uint32 t = 0; t < 16; t++)
  {
    w[t] = CcStatic::swapUint32(w[t]);
  }

  //SHA-1 hash computation (alternate method)
  for (uint32 t = 0; t < 80; t++)
  {
    //Prepare the message schedule
    if (t >= 16)
    {
      W(t) = ROL32(W(t + 13) ^ W(t + 8) ^ W(t + 2) ^ W(t), 1);
    }

    //Calculate T
    if (t < 20)
    {
      temp = ROL32(a, 5) + CH(b, c, d) + e + W(t) + c_aK[0];
    }
    else if (t < 40)
    {
      temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(t) + c_aK[1];
    }
    else if (t < 60)
    {
      temp = ROL32(a, 5) + MAJ(b, c, d) + e + W(t) + c_aK[2];
    }
    else
    {
      temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(t) + c_aK[3];
    }

    //Update the working registers
    e = d;
    d = c;
    c = ROL32(b, 30);
    b = a;
    a = temp;
  }

  //Update the hash value
  m_aState[0] += a;
  m_aState[1] += b;
  m_aState[2] += c;
  m_aState[3] += d;
  m_aState[4] += e;
}
