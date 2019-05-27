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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcHttpWorkData
 */
#include "CcHttpWorkData.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"

CcHttpWorkData::~CcHttpWorkData()
{
}

bool CcHttpWorkData::sendHeader()
{
  if (m_oSocket.isValid())
  {
    m_oSocket.writeString(m_oResponse.getHeader());
    m_bHeaderSend = true;
  }
  return m_bHeaderSend;
}

size_t CcHttpWorkData::writeChunked(const void* pData, size_t uiLength)
{
  size_t uiCurrentOffset = 0;
  while(uiCurrentOffset < uiLength)
  {
    CcString sLength = CcString::fromNumber(uiLength, 16);
    sLength += CcHttpGlobalStrings::EOL;
    m_oSocket.writeString(sLength);
    m_oSocket.write(pData, uiLength);
    m_oSocket.writeString(CcHttpGlobalStrings::EOL);
    uiCurrentOffset += uiLength;
  }
  return uiCurrentOffset;
}
