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
 * @brief     Class NetworkApp
 *
 *  Implementation of Main Application
 */

#include "NetworkApp.h"
#include "CcKernel.h"
#include "CcUserList.h"
#include "Network/CcCommonPorts.h"

NetworkApp::NetworkApp():
m_Telnet(CcCommonPorts::CcTestBase + CcCommonPorts::TELNET),
m_HttpServer(CcCommonPorts::CcTestBase + CcCommonPorts::HTTP),
m_FtpServer(CcCommonPorts::CcTestBase + CcCommonPorts::FTP)
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
    m_CameraProvider = new CcHttpCamera(tempCam); CCMONITORNEW(m_CameraProvider.ptr());
    m_CameraProvider->setPath("/cam.bmp");
    m_HttpServer.registerProvider(m_CameraProvider);
  }
  m_HttpServer.start();

  m_FtpServer.setUserList(CcKernel::getUserList());
  m_FtpServer.setAnonymous(true);
  m_FtpServer.start();

  m_TftpServer.config().setRootDir("C:/tftpboot");
  m_TftpServer.start();

  m_Telnet.start();

  m_DhcpServer.start();

  while (m_TftpServer.getThreadState() != EThreadState::Stopped ||
          m_HttpServer.getThreadState() != EThreadState::Stopped ||
          m_Telnet.getThreadState() != EThreadState::Stopped ||
          m_FtpServer.getThreadState() != EThreadState::Stopped ||
          m_DhcpServer.getThreadState() != EThreadState::Stopped
    )
  {
    if (getThreadState() != EThreadState::Running)
    {
      m_HttpServer.stop();
      m_Telnet.stop();
      m_FtpServer.stop();
      m_TftpServer.stop();
      m_DhcpServer.stop();
    }
    else
    {
      CcKernel::delayS(1);
    }
  }
}
