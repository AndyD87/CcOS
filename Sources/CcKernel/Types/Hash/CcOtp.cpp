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

CcOtp::CcOtp() :
  m_oResult(static_cast<size_t>(32))
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
    uint64 uiDigits = *static_cast<const uint64*>(pData);
    uiDigits = CcStatic::swapUint64(uiDigits);

    m_oHash.finalize(&uiDigits, sizeof(uiDigits));

    m_oResult = m_oHash.getValue();
    
    uint32 offset   = m_oResult[19] & 0x0f;

    uint32 bin_code = (m_oResult[offset]  & 0x7f) << 24
        | (m_oResult[offset+1] & 0xff) << 16
        | (m_oResult[offset+2] & 0xff) <<  8
        | (m_oResult[offset+3] & 0xff);
  }
  return *this;
}
