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
 * @brief     Class CcSqlEnDecode
 */
#include "Hash/CcSqlEnDecode.h"
#include "CcByteArray.h"
#include "CcString.h"

CcSqlEnDecode::CcSqlEnDecode()
{
}

CcSqlEnDecode& CcSqlEnDecode::generate(const void* pData, size_t uiSize)
{
  m_oValue.clear();
  initRandom();
  return append(pData, uiSize);
}

CcSqlEnDecode& CcSqlEnDecode::append(const void *pData, size_t uiSize)
{
  m_oValue.resize(uiSize);
  const uchar* pcData = static_cast<const uchar*>(pData);
  uchar* pcOutput = reinterpret_cast<uchar*>(m_oValue.getArray());
  for (size_t i=0; i < uiSize; i++)
  {
    m_uiShift^=(uint) (Random()*255.0);
    uchar idx= pcData[0];
    *pcOutput++ = m_oEncodeBuffer[idx] ^ m_uiShift;
    m_uiShift^= idx;
    pcData++;
  }
  return *this;
}

CcSqlEnDecode& CcSqlEnDecode::finalize(const void* pData, size_t uiSize)
{
  return append(pData, uiSize);
}

IHash& CcSqlEnDecode::setKey(const void* pData, size_t uiSize)
{
  uint32 nr=1345345333L, add=7, nr2=0x12345671L;
  uint32 tmp;
  const char *password    = static_cast<const char*>(pData);
  const char *password_end= static_cast<const char*>(pData) + uiSize;
  for (; password < password_end; password++)
  {
    if (*password == ' ' || *password == '\t')
      continue;                                 /* skip space in password */
    tmp= (uint32) (uchar) *password;
    nr^= (((nr & 63)+add)*tmp)+ (nr << 8);
    nr2+=(nr2 << 8) ^ nr;
    add+=tmp;
  }
  m_uiKey[0]=nr & (((uint32) 1L << 31) -1L); /* Don't use sign bit (str2int) */;
  m_uiKey[1]=nr2 & (((uint32) 1L << 31) -1L);
  initRandom();
  return *this;
}

const CcByteArray &CcSqlEnDecode::decode(const void* pData, size_t uiSize)
{
  initRandom();
  m_oValue.clear();
  m_oValue.resize(uiSize);
  const uchar* pcData = static_cast<const uchar*>(pData);
  uchar* pcOutput = reinterpret_cast<uchar*>(m_oValue.getArray());
  for (size_t i=0; i < uiSize; i++)
  {
    m_uiShift^=(uint32)(Random()*255.0);
    uchar idx= pcData[0] ^ m_uiShift;
    *pcOutput = m_oDecodeBuffer[idx];
    m_uiShift^= *pcOutput++;
    pcData++;
  }
  return m_oValue;
}

void CcSqlEnDecode::initRandom()
{
  RandWork.max_value= 0x3FFFFFFFL;
  RandWork.max_value_dbl=(double) RandWork.max_value;
  RandWork.seed1=m_uiKey[0]%RandWork.max_value ;
  RandWork.seed2=m_uiKey[1]%RandWork.max_value;

  for (uint16 i=0 ; i < CcSqlEnDecode_CryptoBufferSize; i++)
   m_oDecodeBuffer[i]= static_cast<uchar>(i);

  for (uint16 i=0 ; i < CcSqlEnDecode_CryptoBufferSize ; i++)
  {
    uchar idx= (uchar)(Random()*255.0);
    char a= m_oDecodeBuffer[idx];
    m_oDecodeBuffer[idx]= m_oDecodeBuffer[i];
    m_oDecodeBuffer[+i]=a;
  }
  for (uint16 i=0 ; i < CcSqlEnDecode_CryptoBufferSize ; i++)
   m_oEncodeBuffer[m_oDecodeBuffer[i]]=i;
  m_uiShift=0;
}

double CcSqlEnDecode::Random()
{
  unsigned long seed1;
  seed1= (RandWork.seed1*3+RandWork.seed2) % RandWork.max_value;
  RandWork.seed2=(seed1+RandWork.seed2+33) % RandWork.max_value;
  RandWork.seed1= seed1;
  return (((double) seed1)/RandWork.max_value_dbl);
}
