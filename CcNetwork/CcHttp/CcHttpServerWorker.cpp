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
 * @brief     Implementation of Class CcHttpServerWorker
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "IThread.h"
#include "CcHttpServerWorker.h"
#include "CcHttpServer.h"
#include "CcHttpResponse.h"
#include "IHttpProvider.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"

CcHttpServerWorker::~CcHttpServerWorker()
{
  m_oData.getSocket().close();
  m_oData.getServer().decWorker();
}

void CcHttpServerWorker::run()
{
  if (m_oData.getSocket().isValid())
  {
    m_oData.getSocket().setTimeout(m_oData.getServer().getConfig().getComTimeout());
    size_t uiReadData;
    size_t uiContentOffset = 0;
    CcString sInputData;
    CcByteArray oArray(m_oData.getServer().getConfig().getMaxTransferPacketSize());
    CcStatus oInputState;
    do
    {
      uiReadData = m_oData.getSocket().read(oArray.getArray(), oArray.size());
      if(uiReadData <= oArray.size())
        sInputData.append(oArray, 0, uiReadData);
      oInputState = chkReadBuf(sInputData, uiContentOffset);
    } while ( (oInputState == EStatus::MoreProcessingRequired ) &&
              uiReadData > 0 &&
              uiReadData < m_oData.getServer().getConfig().getMaxTransferPacketSize());
    // Check for valid data
    if (oInputState             &&
        sInputData.length() > 0 &&
        uiReadData < SIZE_MAX)
    {
      if (uiContentOffset < uiReadData)
      {
        m_oData.getRequest().appendContent(sInputData.getCharString() + uiContentOffset, uiReadData - uiContentOffset);
      }
      m_oData.getRequest().parse(sInputData);
      CcHandle<IHttpProvider> provider = m_oData.getServer().findProvider(m_oData);
      if (provider.isValid())
      {
        provider->exec(m_oData);
      }
      finish();
    }
  }
}

CcStatus CcHttpServerWorker::chkReadBuf(const CcString& sInputData, size_t& uiContentOffset)
{
  CcStatus bRet = EStatus::Error;
  size_t pos;
  pos = sInputData.find(CcHttpGlobalStrings::EOLSeperator);
  if (pos < sInputData.length())
  {
    uiContentOffset = pos + CcHttpGlobalStrings::EOLSeperator.length();
    CcString sHeader;
    sHeader.append(sInputData,0, pos);
    m_oData.getRequest().parse(sHeader);
    bRet = EStatus::Success;
  }
  else
  {
    // Check if it is a valid http packet
    pos = sInputData.find(CcHttpGlobalStrings::EOL);
    if (pos < sInputData.length())
    {
      CcString sLine = sInputData.substr(0, pos);
      sLine.toLower();
      if (!sLine.contains("http"))
      {
        bRet = EStatus::MoreProcessingRequired;
      }
    }
  }
  return bRet;
}

void CcHttpServerWorker::finish()
{
  if (m_oData.isHeaderSend() == false)
  {
    m_oData.sendHeader();
  }
  if(m_oData.getResponse().getTransferEncoding().isChunked())
  {
    m_oData.writeAllChunked();
    m_oData.getSocket().writeString('0');
    m_oData.getSocket().writeString(CcHttpGlobalStrings::EOL);
    m_oData.getSocket().writeString(CcHttpGlobalStrings::EOL);
  }
  else
  {
    if (m_oData.getResponse().m_oContent.size())
    {
      m_oData.getSocket().writeBufferList(m_oData.getResponse().m_oContent);
    }
  }
  m_oData.getSocket().close();
}

void CcHttpServerWorker::error()
{
  finish();
}
