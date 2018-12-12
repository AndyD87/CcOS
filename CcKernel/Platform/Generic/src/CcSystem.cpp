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
#include "CcGlobalStrings.h"
#include "CcUserList.h"

class CcSystemPrivate
{
public:
};

CcSystem::CcSystem()
{
  m_pPrivateData = new CcSystemPrivate();
}

CcSystem::~CcSystem() {
  delete m_pPrivateData;
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

CcDateTime CcSystem::getDateTime(void)
{
  CcDateTime oRet;
  return oRet;
}

void CcSystem::sleep(uint32 timeoutMs)
{
  CCUNUSED(timeoutMs);
}

CcHandle<CcDevice> CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{ 
  CCUNUSED(Type); 
  CCUNUSED(Name); 
  return nullptr; 
}

CcSocketAbstract* CcSystem::getSocket(ESocketType type)
{
  CCUNUSED(type);
  return nullptr;
}

CcString CcSystem::getConfigDir() const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getDataDir() const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getBinaryDir() const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getWorkingDir(void) const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getTemporaryDir(void) const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getUserDir() const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getUserDataDir() const
{
  return CcGlobalStrings::Empty;
}

CcUserList CcSystem::getUserList()
{
  CcUserList UserList;
  return UserList;
}
