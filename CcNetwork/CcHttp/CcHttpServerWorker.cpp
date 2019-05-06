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
    size_t uiReadData;
    CcString sInputData;
    do
    {
      CcByteArray oArray(2048); // @todo: magic number
      uiReadData = m_oData.getSocket().readArray(oArray);
      sInputData.append(oArray);
    } while (chkReadBuf(sInputData) == false &&
            uiReadData > 0 &&
            uiReadData < SIZE_MAX); // @todo remove SIZE_MAX with a max transfer size
    m_oData.getRequest().parse(sInputData);
    CcHandle<IHttpProvider> provider = m_oData.getServer().findProvider(m_oData.getRequest().getPath());
    if(provider.isValid())
    {
      provider->exec(m_oData);
    }
    finish();
  }
}

bool CcHttpServerWorker::chkReadBuf(const CcString& sInputData)
{
  bool bRet = false;
  size_t pos;
  pos = sInputData.find(CcHttpGlobalStrings::EOLSeperator);
  if (pos != SIZE_MAX)
  {
    CcString sHeader;
    sHeader.append(sInputData,0, pos);
    m_oData.getRequest().parse(sHeader);
    bRet = true;
  }
  return bRet;
}

void CcHttpServerWorker::finish()
{
  if (m_oData.isHeaderSend() == false)
  {
    m_oData.sendHeader();
  }
  if (m_oData.getResponse().m_oContent.size())
  {
    m_oData.getSocket().writeBufferList(m_oData.getResponse().m_oContent);
  }
  m_oData.getSocket().close();
}

void CcHttpServerWorker::error()
{
  finish();
}
