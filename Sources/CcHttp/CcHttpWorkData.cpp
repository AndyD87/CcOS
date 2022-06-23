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
 * @brief     Implementation of Class CcHttpWorkData
 */
#include "CcHttpWorkData.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcStatic.h"
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

//! Default transfer size for reading from http server
#define MAX_TRANSER_BUFFER 10240

size_t CcHttpWorkData::readAllContent()
{
  if (getRequest().getTransferEncoding().isChunked())
  {
    size_t uiLeftLine     = 0;
    size_t uiLastReadSize = 0;
    bool bDone = false;

    // Copy incoming data to new array
    CcByteArray oBuffer(getRequest().getContent());
    getRequest().getContent().clear();

    // Setup working data
    uiLastReadSize = oBuffer.size();
    oBuffer.resize(MAX_TRANSER_BUFFER);
    do
    {
      // Check if next read will 
      if (uiLeftLine == 0 && uiLastReadSize > 0)
      {
        size_t uiPos = oBuffer.findString(CcHttpGlobalStrings::EOL);
        if (uiPos <= oBuffer.size())
        {
          // Get Hex value as string from data
          CcString sLength(oBuffer.getArray(), uiPos);
          oBuffer.move(0, uiPos + CcHttpGlobalStrings::EOL.size());
          bool bOk;
          // Convert hex value to length of next packet
          uiLeftLine = static_cast<size_t>(sLength.toUint64(&bOk, 16));
          // Check if packet size is null, then we are ready
          if (uiLeftLine == 0)
          {
            bDone = true;
          }
        }
        else
        {
          // No data found, read from socket
          uiLastReadSize = 0;
        }
      }
      else if (uiLastReadSize > 0)
      {
        if (uiLastReadSize <= uiLeftLine)
        {
          getRequest().getContent().append(oBuffer.getArray(), uiLastReadSize);
          uiLeftLine -= uiLastReadSize;
          uiLastReadSize = 0;
        }
        else
        {
          // Add real content to buffer
          getRequest().getContent().append(oBuffer.getArray(), uiLeftLine);
          // Move buffer pointer to next chunk
          size_t uiMoveSize = uiLeftLine + CcHttpGlobalStrings::EOL.size();
          uiLastReadSize -= uiMoveSize;
          oBuffer.move(0, uiMoveSize, uiLastReadSize);
          // Reset line
          uiLeftLine = 0;
        }
      }
      else
      {
        uiLastReadSize = m_oSocket.readArray(oBuffer, false);
      }
    } while (bDone == false && 
             uiLastReadSize <= MAX_TRANSER_BUFFER);
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

size_t CcHttpWorkData::writeAllChunked()
{
  CcString sLength = CcString::fromSize(m_oResponse.getContent().size(), 16);
  sLength += CcHttpGlobalStrings::EOL;
  m_oResponse.getContent().prepend(CCMOVE(sLength));
  m_oResponse.getContent().append(CcHttpGlobalStrings::EOL);
  m_oResponse.getContent().setPosition(0);
  size_t uiSent = m_oSocket.write(m_oResponse.getContent().getBuffer(), m_oResponse.getContent().size());
  m_oResponse.getContent().clear();
  return uiSent;
}

size_t CcHttpWorkData::write(const void* pData, size_t uiLength)
{
  if(getResponse().getTransferEncoding().isChunked())
  {
    size_t uiCurrentOffset = uiLength;
    m_oResponse.getContent().append(pData, uiLength);
    if(m_oResponse.getContent().size() > 1024)
    {
      writeAllChunked();
    }
    return uiCurrentOffset;
  }
  else
  {
    getResponse().getContent().append(pData, uiLength);
    return uiLength;
  }
}

CcString CcHttpWorkData::splitQueryLine(CcString& sPath)
{
  CcString sQuery;
  size_t uiOffset = sPath.find(CcGlobalStrings::Seperators::QuestionMark);
  if (uiOffset < sPath.length())
  {
    sQuery = sPath.substr(uiOffset + 1);
    sPath.remove(uiOffset, sPath.size());
  }
  return sQuery;
}

size_t CcHttpWorkData::read(void* pData, size_t uiSize)
{
  size_t uiReturn = SIZE_MAX;
  if (m_oRequest.getContent().size())
  {
    uiReturn = m_oRequest.getContent().read(pData, uiSize);
    m_oRequest.getContent().remove(0, uiSize);
  }
  else
    uiReturn = m_oSocket.read(pData, uiSize);
  return uiReturn;
}

CcStringMap CcHttpWorkData::parseQueryLine(const CcString& sData)
{
  CcStringMap oMap;
  CcStringList oVars = sData.split(CcGlobalStrings::Seperators::Ampersand);
  for (const CcString& sPair : oVars)
  {
    CcStringList oPair = sPair.split(CcGlobalStrings::Seperators::Equal);
    if (oPair.size() == 1)
    {
      oMap.append(oPair[0], CcGlobalStrings::Empty);
    }
    else if (oPair.size() > 1)
    {
      oMap.append(oPair[0], oPair[1]);
    }
  }
  return oMap;
}

CcString CcHttpWorkData::generateQueryLine(const CcStringMap& oData)
{
  CcString sQuery;
  bool bFirst = true;
  for (const CcStringPair& oPair : oData)
  {
    if (!bFirst)
    {
      sQuery << CcGlobalStrings::Seperators::Ampersand;
    }
    sQuery << oPair.getKey() << CcGlobalStrings::Seperators::Equal << oPair.getValue();
  }
  return sQuery;
}
