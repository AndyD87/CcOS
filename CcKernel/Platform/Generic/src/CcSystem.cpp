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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSystem
 **/

#include "CcSystem.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcDateTime.h"
#include "CcFileSystem.h"
#include "CcDevice.h"
#include "Network/CcSocket.h"
#include <ctime>


CcSystem::CcSystem()
{
}

CcSystem::~CcSystem() {
}

void CcSystem::init(void)
{
}

bool CcSystem::initGUI(void)
{
  return true; // YES we have a gui
}

bool CcSystem::initCLI(void)
{
  return true; // YES we have a cli
}

int CcSystem::initService()
{
  return 1;
}

bool CcSystem::start( void )
{
  m_bSystemState = true; // We are done
  CcKernel::systemReady();
  while (m_bSystemState == true)
  {
  }
  return false;
}

void CcSystem::stop(void)
{
  m_bSystemState = false;
}

/**
 * @brief Function to start the ThreadObject
 * @param Param: Param containing pointer to ThreadObject
 * @return alway returns 0, todo: get success of threads as return value;
 */
int threadFunction(void *Param)
{
  // Just set Name only on debug ( save system ressources )
  CcThreadObject *pThreadObject = static_cast<CcThreadObject *>(Param);
#ifdef DEBUG
  //SetThreadName(pThreadObject->getName().getCharString());
#endif
  pThreadObject->enterState(EThreadState::Running);
  pThreadObject->run();
  pThreadObject->enterState(EThreadState::Stopped);
  pThreadObject->onStopped();
  return 0;
}

bool CcSystem::createThread(CcThreadObject &Thread)
{
  CCUNUSED(Thread);
  return false;
}

bool CcSystem::createProcess(CcProcess &processToStart)
{
  CCUNUSED(processToStart);
  return false;
}

void CcSystem::loadModule(const CcString& Path)
{
  CCUNUSED(Path);
}

CcDateTime CcSystem::getDateTime(void)
{
  CcDateTime oRet;
  return oRet;
}

void CcSystem::sleep(uint32 timeoutMs)
{
  CCUNUSED(timeoutMs);
}

CcSocket* CcSystem::getSocket(ESocketType type)
{
  CCUNUSED(type);
  return nullptr;
}

CcUserList CcSystem::getUserList()
{
  CcUserList UserList;
  return UserList;
}

CcHandle<CcFileSystem> CcSystem::getFileSystemManager()
{
  return CcHandle<CcFileSystem>(nullptr);
}
