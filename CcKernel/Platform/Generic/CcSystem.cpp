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
#include "IDevice.h"
#include "Network/CcSocket.h"
#include "CcGlobalStrings.h"
#include "CcUserList.h"
#include "CcObject.h"
#include "CcThreadManager.h"
#include "Devices/ITimer.h"
#include "Devices/ICpu.h"
#include "CcThreadContext.h"


class CcSystemPrivate : public CcObject
{
public:
  CcSystemPrivate()
  {
    m_pCpu = CcKernel::getDevice(EDeviceType::Cpu).cast<ICpu>();
    if(m_pCpu != nullptr)
    {
      m_oThreads.append(m_pCpu->mainThread());
    }
  }

  void SystemTick(CcDeviceHandle*)
  {
    uiUpTime += 1000;
    m_uiThreadCount++;
    if(m_uiThreadCount >= 10)
    {
      m_uiThreadCount = 0;
      if(m_oThreads.size())
      {
        CcThreadContext* pThreadContext = m_oThreads[0];
        m_oThreads.remove(0);
        m_pCpu->loadThread(pThreadContext);
        m_oThreads.append(pThreadContext);
      }
    }
  }

  void appendThread(IThread* pThread)
  {
    CcThreadContext* pThreadContext = m_pCpu->createThread(pThread);
    m_oThreads.prepend(pThreadContext);
  }

  volatile uint64 uiUpTime = 0;
private:
  CcHandle<ICpu> m_pCpu = nullptr;
  CcList<CcThreadContext*> m_oThreads;
  uint64 m_uiThreadCount = 0;
};

CcSystem::CcSystem()
{
  m_pPrivateData = new CcSystemPrivate();
  CcDeviceHandle hTimer = CcKernel::getDevice(EDeviceType::Timer);
  if(hTimer.isValid())
  {
    hTimer.cast<ITimer>()->onTimeout(NewCcEvent(CcSystemPrivate,CcDeviceHandle,CcSystemPrivate::SystemTick,m_pPrivateData));
  }
}

CcSystem::~CcSystem() {
  delete m_pPrivateData;
}

void CcSystem::init()
{
}

bool CcSystem::initGUI()
{
  return false; // we do not have a gui on a generic system
}

bool CcSystem::initCLI()
{
  return false; // we do not have a cli on a generic system
}

int CcSystem::initService()
{
  return 1;
}

bool CcSystem::createThread(IThread &oThread)
{
  m_pPrivateData->appendThread(&oThread);
  return false;
}

bool CcSystem::createProcess(CcProcess &oProcessToStart)
{
  CCUNUSED(oProcessToStart);
  return false;
}

CcDateTime CcSystem::getDateTime()
{
  return CcDateTime(m_pPrivateData->uiUpTime);
}

void CcSystem::sleep(uint32 timeoutMs)
{
  CcDateTime oCurrentTime(m_pPrivateData->uiUpTime);
  oCurrentTime.addMSeconds(timeoutMs);
  while(m_pPrivateData->uiUpTime < static_cast<uint64>(oCurrentTime.getTimestampUs()))
  {

  }
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{ 
  CCUNUSED(Type); 
  CCUNUSED(Name); 
  return nullptr; 
}

CcStringMap CcSystem::getEnvironmentVariables() const
{
  return CcStringMap();
}

CcString CcSystem::getEnvironmentVariable(const CcString& sName) const
{
  CCUNUSED(sName);
  return "";
}

bool CcSystem::getEnvironmentVariableExists(const CcString& sName) const
{
  CCUNUSED(sName);
  return false;
}

bool CcSystem::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  CCUNUSED(sName);
  CCUNUSED(sValue);
  return false;
}


ISocket* CcSystem::getSocket(ESocketType type)
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

CcString CcSystem::getWorkingDir() const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getTemporaryDir() const
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

CcSharedMemoryAbstract* CcSystem::getSharedMemory(const CcString& sName, size_t uiSize)
{
  CCUNUSED(sName);
  CCUNUSED(uiSize);
  return nullptr;
}

CcGroupList CcSystem::getGroupList()
{
  return CcGroupList();
}
