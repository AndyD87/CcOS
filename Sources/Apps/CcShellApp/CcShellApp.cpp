/*
 * This file is part of CcShellApp.
 *
 * CcShellApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcShellApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcShellApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcShellApp
 */
#include "CcShellApp.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"
#include "CcShellApp.h"
#include "CcShellWorker.h"
#include "CcSshWorker.h"
#include "Network/CcCommonPorts.h"
#include "CcSslSocket.h"
#include "CcSslControl.h"
#include "CcConsole.h"
#ifdef WINDOWS
  #include "Shell/CcWinRawConsole.h"
#endif

CcShellApp::CcShellApp() :
  m_oArguments(
    {
      {"server",       CcVariant::EType::Switch},
      {"connect",      CcVariant::EType::Switch},
      {"ssh",          CcVariant::EType::Switch},
      {"-address",     CcVariant::EType::String}
    }
  )
{
}

CcShellApp::~CcShellApp()
{
  for (IThread* pWorker : m_oWorker)
  {
    CCDELETE(pWorker);
  }
  m_oWorker.clear();
}

void CcShellApp::run()
{
  CcStatus oStatus;
  if (!m_oArguments.isValid())
  {
    m_oArguments.writeHelp(CcConsole::getOutStream());
  }
  else if ( m_oArguments.contains("server") &&
            m_oArguments.getValue("server").getBool())
  {
    runServer();
  }
  else if (m_oArguments.contains("connect") &&
           m_oArguments.getValue("connect").getBool())
  {
    runConnect();
  }
  else if ( m_oArguments.contains("ssh") &&
            m_oArguments.getValue("ssh").getBool())
  {
    runSsh();
  }
  else
  {
    runLocal();
  }
  if (getExitCode() && !oStatus)
  {
    setExitCode(oStatus);
  }
  CCDEBUG("CcShellApp stopped, code " + CcString::fromNumber(getExitCode().getErrorUint()));
}

CcStatus CcShellApp::parseArguments(int iArgs, char** pArgs)
{
  return m_oArguments.parse(iArgs, pArgs);
}

void CcShellApp::runServer()
{
  CCDEBUG("Shell-Server starting on Port: " + CcString::fromNumber(CcCommonPorts::CcRemoteShell));
  ISocket *temp;
  m_Socket = CcSocket(ESocketType::TCP);
  if (m_Socket.bind(CcCommonPorts::CcRemoteShell))
  {
    if (m_Socket.listen())
    {
      while (getThreadState() == EThreadState::Running)
      {
        temp = m_Socket.accept();
        if (temp != nullptr)
        {
          CCNEWTYPE(pWorker, CcShellWorker, this, temp, false);
          pWorker->start();
          pWorker->setEcho(false);
          m_oWorker.add(pWorker);
        }
        else
        {
          CCDEBUG("Shell-Server Accept failed, wait a second for retry");
          CcKernel::delayS(1);
        }
      }
    }
    else
    {
      CCDEBUG("CcTelnetServerServer::run Listen failed, wait a second for retry");
      CcKernel::delayS(1);
    }
  }
  else
  {
    CCDEBUG("CcTelnetServerServer::run Bind failed, wait a second for retry");
    CcKernel::delayS(1);
  }
}

void CcShellApp::runSsh()
{
  CCDEBUG("Ssh-Server starting on Port: " + CcString::fromNumber(CcCommonPorts::CcRemoteSsh));
  ISocket *temp;
  m_Socket = CCNEW_INLINE(CcSocket, ESocketType::TCP);
  if (m_Socket.bind(CcCommonPorts::CcRemoteSsh))
  {
    if (m_Socket.listen())
    {
      while (getThreadState() == EThreadState::Running)
      {
        temp = m_Socket.accept();
        if (temp != nullptr)
        {
          CCNEWTYPE(pWorker, CcSshWorker, this, temp);
          pWorker->start();
          pWorker->setEcho(false);
          m_oWorker.add(pWorker);
        }
        else
        {
          CCDEBUG("Shell-Server Accept failed, wait a second for retry");
          CcKernel::delayS(1);
        }
      }
    }
    else
    {
      CCDEBUG("CcTelnetServerServer::run Listen failed, wait a second for retry");
      CcKernel::delayS(1);
    }
  }
  else
  {
    CCDEBUG("CcTelnetServerServer::run Bind failed, wait a second for retry");
    CcKernel::delayS(1);
  }
}

void CcShellApp::runConnect()
{
  CcString sTarget = m_oArguments.getValue("connect").getString();

}

void CcShellApp::runSconnect()
{

}

void CcShellApp::runLocal()
{
  CcShell oShell;
  setExitCode(oShell.exec());
}
