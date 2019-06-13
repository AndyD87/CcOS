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

#define MAX_TRANSER_BUFFER 1024

size_t CcHttpWorkData::readAllContent()
{
  if (getRequest().getTransferEncoding().isChunked())
  {
    size_t uiLeftLine = 0;
    bool bDone = false;
    do
    {
      CcByteArray oBuffer(MAX_TRANSER_BUFFER);
      if (uiLeftLine == 0)
      {
        size_t uiPos = oBuffer.find(CcHttpGlobalStrings::EOL);
        if (uiPos != SIZE_MAX)
        {
          CcString sLength(oBuffer.getArray(), uiPos);
          oBuffer.remove(0, uiPos + CcHttpGlobalStrings::EOL.length());
          bool bOk;
          uiLeftLine = static_cast<size_t>(sLength.toUint64(&bOk, 16));
          if (uiLeftLine == 0)
          {
            uiLeftLine = 100;
            bDone = true;
          }
        }
        else
        {
          oBuffer.resize(MAX_TRANSER_BUFFER);
          m_oSocket.readArray(oBuffer);
        }
      }
      else
      {
        if (oBuffer.size() <= uiLeftLine)
        {
          getRequest().getContent().append(oBuffer);
          uiLeftLine -= oBuffer.size();
          oBuffer.clear();
          oBuffer.resize(MAX_TRANSER_BUFFER);
          m_oSocket.readArray(oBuffer);
        }
        else
        {
          getRequest().getContent().append(oBuffer, uiLeftLine);
          oBuffer.remove(0, uiLeftLine);
          uiLeftLine = 0;
          if (oBuffer.find(CcHttpGlobalStrings::EOL) == 0)
          {
            oBuffer.remove(0, CcHttpGlobalStrings::EOL.length());
            if (oBuffer.size() == 0)
            {
              oBuffer.resize(MAX_TRANSER_BUFFER);
              m_oSocket.readArray(oBuffer);
            }
          }
          else if (oBuffer.size() == 1)
          {
            oBuffer.resize(MAX_TRANSER_BUFFER);
            m_oSocket.readArray(oBuffer);
            oBuffer.remove(0);
          }
          else
          {
            bDone = true;
          }
        }
      }
    } while (bDone == false);
  }
  else
  {
    size_t uiExpectedSize = static_cast<size_t>(getRequest().getContentLength());
    if (getRequest().getContent().size() < uiExpectedSize)
    {
      getRequest().getContent().resize(uiExpectedSize - getRequest().getContent().size());
    }
    while(getRequest().getContent().size() < uiExpectedSize)
    {
      m_oSocket.read(getRequest().getContent().getArray() + getRequest().getContent().size(), uiExpectedSize - getRequest().getContent().size());
    }
  }
  return getRequest().getContent().size();
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
