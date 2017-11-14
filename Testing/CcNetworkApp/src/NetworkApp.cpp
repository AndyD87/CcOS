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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: 
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @file     NetworkApp.h
 * @brief     Class NetworkApp
 *
 *  Implementation of Main Application
 */

#include "NetworkApp.h"
#include "CcKernel.h"
#include "CcUserList.h"

NetworkApp::NetworkApp() 
{
}

NetworkApp::~NetworkApp() 
{

}

void NetworkApp::run(void)
{
  CCDEBUG("CcNetworkApp starting...");

  CcHandle<CcCamera>tempCam = CcKernel::getDevice(EDeviceType::Camera).cast<CcCamera>();
  if (tempCam != nullptr)
  {
    m_CameraProvider = new CcHttpCamera(tempCam); CCMONITORNEW(m_CameraProvider.get());
    m_CameraProvider->setPath("/cam.bmp");
    m_HttpServer.registerProvider(m_CameraProvider);
  }
  m_HttpServer.start();

  m_FtpServer.setUserList(CcKernel::getUserList());
  m_FtpServer.setAnonymous(true);
  m_FtpServer.start();

  m_Telnet.start();

  while (m_TftpServer.getThreadState() != EThreadState::Stopped ||
          m_HttpServer.getThreadState() != EThreadState::Stopped ||
          m_Telnet.getThreadState()     != EThreadState::Stopped ||
          m_FtpServer.getThreadState()  != EThreadState::Stopped
    )
  {
    if (getThreadState() != EThreadState::Running)
    {
      m_HttpServer.stop();
      m_Telnet.stop();
      m_FtpServer.stop();
      m_TftpServer.stop();
    }
    else
    {
      CcKernel::delayS(1);
    }
  }
}
