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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:
 * @par       Language: C++11
 * @brief     Class NetworkApp
 *
 *  Example GUI-Application with Menue Structure and Application Loader
 */
#pragma once

#include "CcBase.h"
#include "CcApp.h"
#include "CcHttpServer.h"
#include "CcFtpServer.h"
#include "CcTftpServer.h"
#include "Network/Server/CcUdpEchoServer.h"
#include "Network/Server/CcTcpEchoServer.h"
#ifdef TEST_DHCP_SERVER
#include "CcDhcpServer.h"
#endif // TEST_DHCP_SERVER
#include "HttpProvider/CcHttpCamera.h"
#include "Network/CcCommonPorts.h"

class NetworkApp: public CcApp
{
public:
  NetworkApp();
  virtual ~NetworkApp();

  void run() override;

private:
  CcHttpServer  m_HttpServer;
  CcFtpServer   m_FtpServer;
  CcTftpServer  m_TftpServer;
  CcUdpEchoServer m_oUdpEchoServer;
  CcTcpEchoServer m_oTcpEchoServer;
#ifdef TEST_DHCP_SERVER
  //CcDhcpServer  m_DhcpServer;
#endif // TEST_DHCP_SERVER

  CcHandle<IHttpPathProvider> m_CameraProvider = NULL;
};
