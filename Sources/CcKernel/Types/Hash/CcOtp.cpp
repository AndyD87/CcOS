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
 * @brief     Class CcOtp
 */
#include "Hash/CcOtp.h"
#include "CcByteArray.h"
#include "CcString.h"
#include "CcStatic.h"
#include "CcKernel.h"

const int64 CcOtp::s_DefaultBaseTime = 0;
const uint64 CcOtp::s_DefaultTimesteps = 30;

CcOtp::CcOtp() :
  m_oResult(static_cast<size_t>(32)),
  m_oHash(EHashType::Sha1)
{
}

CcOtp& CcOtp::generate(const void* pData, size_t uiSize)
{
  return finalize(pData, uiSize);
}

CcOtp& CcOtp::append(const void *pData, size_t uiSize)
{
  return finalize(pData, uiSize);
}

CcOtp& CcOtp::finalize(const void* pData, size_t uiSize)
{
  if(uiSize == 8)
  {
    m_oHash.finalize(pData, uiSize);

    m_oResult = m_oHash.getValue();
  }
  return *this;
}

CcOtp& CcOtp::finalizeTotp(uint64 uiTimesteps, int64 uiBaseTime)
{
  uint64 uiCounter = CcKernel::getDateTime().getTimestampS();
  if (uiTimesteps > 0)
  {
    uiCounter = (uiCounter - uiBaseTime) / uiTimesteps;
    return finalizeCounter(uiCounter);
  }
  else
  {
    return *this;
  }
}

CcOtp& CcOtp::finalizeCounter(uint64 uiCounter)
{
  uint64 endianness = 0xdeadbeef;
  if ((*(const uint8_t *)&endianness) == 0xef) {
    uiCounter = ((uiCounter & 0x00000000ffffffff) << 32) | ((uiCounter & 0xffffffff00000000) >> 32);
    uiCounter = ((uiCounter & 0x0000ffff0000ffff) << 16) | ((uiCounter & 0xffff0000ffff0000) >> 16);
    uiCounter = ((uiCounter & 0x00ff00ff00ff00ff) << 8)  | ((uiCounter & 0xff00ff00ff00ff00) >> 8);
  };
  return finalize(&uiCounter, sizeof(uiCounter));
}

inline size_t size_pow(size_t base, size_t exp)
{
  size_t result = 1;
  while (exp)
  {
    if (exp % 2)
      result *= base;
    exp /= 2;
    base *= base;
  }
  return result;
}

CcString CcOtp::getDigits(size_t uiSize)
{
  CcString sResult;
  size_t uiPower = size_pow(10, uiSize);

  uint32 offset = m_oResult[19] & 0x0f;

  uint32 bin_code = (m_oResult[offset] & 0x7f) << 24
    | (m_oResult[offset + 1] & 0xff) << 16
    | (m_oResult[offset + 2] & 0xff) << 8
    | (m_oResult[offset + 3] & 0xff);

  uint32 otp = bin_code % uiPower;

  return CcString::fromNumber(otp).fillBeginUpToLength("0", uiSize);
}
