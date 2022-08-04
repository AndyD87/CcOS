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
 * @brief     Class NetworkApp
 *
 *  Implementation of Main Application
 */

#include "NetworkApp.h"
#include "CcKernel.h"
#include "CcUserList.h"
#include "Network/CcCommonPorts.h"
#include "CcDevice.h"

NetworkApp::NetworkApp():
m_HttpServer(CcCommonPorts::CcTestBase + CcCommonPorts::HTTPS),
m_FtpServer(CcCommonPorts::CcTestBase + CcCommonPorts::FTP),
m_oUdpEchoServer(CcCommonPorts::CcTestBase),
m_oTcpEchoServer(CcCommonPorts::CcTestBase)
{
}

NetworkApp::~NetworkApp() 
{
}

void NetworkApp::run()
{
  CCDEBUG("CcNetworkApp starting...");

  CcHandle<ICamera>tempCam = CcKernel::getDevice(EDeviceType::Camera).cast<ICamera>();
  if (tempCam != nullptr)
  {
    CCNEW(m_CameraProvider, CcHttpCamera, tempCam);
    m_CameraProvider->setPath("/cam.bmp");
    m_HttpServer.registerProvider(m_CameraProvider.cast<IHttpProvider>());
  }
  m_HttpServer.start();

  m_FtpServer.setUserList(CcKernel::getUserList());
  m_FtpServer.setAnonymous(true);
  m_FtpServer.start();

  m_TftpServer.config().setRootDir("C:/tftpboot");
  m_TftpServer.start();

  m_oUdpEchoServer.start();
  m_oTcpEchoServer.start();


#ifdef TEST_DHCP_SERVER
  m_DhcpServer.start();
#endif // TEST_DHCP_SERVER

  while ( m_TftpServer.getThreadState() != EThreadState::Stopped ||
          m_HttpServer.getThreadState() != EThreadState::Stopped ||
          m_oUdpEchoServer.getThreadState() != EThreadState::Stopped ||
          m_oTcpEchoServer.getThreadState() != EThreadState::Stopped ||
          m_FtpServer.getThreadState() != EThreadState::Stopped
#ifdef TEST_DHCP_SERVER
           ||m_DhcpServer.getThreadState() != EThreadState::Stopped
#endif // TEST_DHCP_SERVER
    )
  {
    if (getThreadState() != EThreadState::Running)
    {
      m_HttpServer.stop();
      m_FtpServer.stop();
      m_TftpServer.stop();
      m_oUdpEchoServer.stop();
      m_oTcpEchoServer.stop();
#ifdef TEST_DHCP_SERVER
      m_DhcpServer.stop();
#endif // TEST_DHCP_SERVER
    }
    else
    {
      CcKernel::delayS(1);
    }
  }
}
