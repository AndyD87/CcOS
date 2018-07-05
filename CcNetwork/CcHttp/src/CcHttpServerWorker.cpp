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
#include "CcThreadObject.h"
#include "CcHttpServerWorker.h"
#include "CcHttpServer.h"
#include "CcHttpResponse.h"
#include "CcHttpProvider.h"
#include "CcHttpGlobalStrings.h"

CcHttpServerWorker::CcHttpServerWorker(CcHttpServer* Server, CcSocket socket) :
  m_Socket(socket),
  m_Server(Server),
  m_Header()
{
}

CcHttpServerWorker::~CcHttpServerWorker(void)
{
}

void CcHttpServerWorker::run()
{
  if (m_Socket != 0)
  {
    size_t uiReadData;
    do
    {
      CcByteArray oArray(1024); // @todo: magic number
      uiReadData = m_Socket.readArray(oArray);
      m_InBuf.append(oArray);
      if (chkReadBuf())
        break;
    } while (uiReadData > 0 && uiReadData < SIZE_MAX); // @todo remove SIZE_MAX with a max transfer size
    m_Header.parse(m_InBuf);
    CcHandle<CcHttpProvider> provider = m_Server->findProvider(m_Header.getPath());
    if (m_Header.getRequestType() == EHttpRequestType::Get)
    {
      CcHttpResponse Response = provider->execGet(m_Header);
      CcByteArray ResponseHead = Response.getHeader().getByteArray();
      m_Socket.write(ResponseHead.getArray(), ResponseHead.size());
      m_Socket.write(Response.m_oContent.getArray(), Response.m_oContent.size());
      m_Socket.close();
    }
    else if (m_Header.getRequestType() == EHttpRequestType::PostMultip)
    {
      CcHttpResponse Response = provider->execPost(m_Header);
      CcByteArray ResponseHead = Response.getHeader().getByteArray();
      m_Socket.write(ResponseHead.getArray(), ResponseHead.size());
      m_Socket.write(Response.m_oContent.getArray(), Response.m_oContent.size());
      m_Socket.close();
    }
    else if (m_Header.getRequestType() == EHttpRequestType::Head)
    {
      CcHttpResponse Response = provider->execHead(m_Header);
      CcByteArray ResponseHead = Response.getHeader().getByteArray();
      m_Socket.write(ResponseHead.getArray(), ResponseHead.size());
      m_Socket.close();
    }
  }
}

bool CcHttpServerWorker::chkReadBuf(void)
{
  bool bRet = false;
  size_t pos;
  pos = m_InBuf.find(CcHttpGlobalStrings::EOLSeperator);
  if (pos != SIZE_MAX)
  {
    CcString sHeader;
    sHeader.append(m_InBuf,0, pos);
    m_Header.parse(sHeader);
    bRet = true;
  }
  return bRet;
}
