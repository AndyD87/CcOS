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
 * @brief     implementations for Class CcServiceSystem
 **/

#include "CcServiceSystem.h"
#include "CcVector.h"
#include "Components/SystemdService.h"

#include <unistd.h>

class CcServiceSystem::CPrivate
{
public:
  CcService* pService;
};

CcServiceSystem::CcServiceSystem()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcServiceSystem::~CcServiceSystem()
{
  stop();
  CCDELETE(m_pPrivate);
}

void CcServiceSystem::stop()
{
  if(m_pPrivate &&
     m_pPrivate->pService)
  {
    CcKernel::getServiceSystem().deinit(*m_pPrivate->pService);
  }
}

CcStatus CcServiceSystem::init(CcService& pService)
{
  // Daemonizing:
  //CcStatus oSuccess = EStatus::Error;
  //pid_t pid = fork();
  //
  //if(pid < 0 )
  //{
  //  CCDEBUG("Failed to fork process.");
  //}
  //else if(pid > 0)
  //{
  //  // We are in parent
  //  oSuccess = true;
  //}
  //else
  //{
  //  // We are in child
  //  m_pPrivate->pService = &pService;
  //  oSuccess   = m_pPrivate->pService->run();
  //}
  //return oSuccess;
  if(m_pPrivate)
  {
    m_pPrivate->pService = &pService;
  }
  CcStatus oRunStatus = pService.run();
  if(m_pPrivate &&
     m_pPrivate->pService == &pService)
  {
    m_pPrivate->pService = nullptr;
  }
  return oRunStatus;
}

CcStatus CcServiceSystem::deinit(CcService& pService)
{
  CcStatus oStatus(false);
  CCDEBUG("Deinit Service: " + pService.getName());
  pService.stop();
  if(m_pPrivate->pService == &pService)
  {
    oStatus = true;
    m_pPrivate->pService = nullptr;
  }
  return oStatus;
}

CcStatus CcServiceSystem::create(CcService& pService)
{
  SystemdService oService(pService);
  return oService.create();
}

CcStatus CcServiceSystem::remove(CcService& pService)
{
  SystemdService oService(pService);
  return oService.remove();
}

CcStatus CcServiceSystem::stop(CcService& pService)
{
  SystemdService oService(pService);
  return oService.stop();
}

CcStatus CcServiceSystem::start(CcService& pService)
{
  SystemdService oService(pService);
  return oService.start();
}

CcStatus CcServiceSystem::setExectuable(CcService& pService, const CcString& sExePath)
{
  SystemdService oService(pService);
  return oService.setExectuable(sExePath);
}

CcStatus CcServiceSystem::setWorkingDir(CcService& pService, const CcString& sWorkingDir)
{
  SystemdService oService(pService);
  return oService.setWorkingDir(sWorkingDir);
}

CcStatus CcServiceSystem::setArguments(CcService& pService, const CcArguments& oArguments)
{
  SystemdService oService(pService);
  return oService.setArguments(oArguments);
}

CcStatus CcServiceSystem::setAutoStart(CcService& pService, bool bOnOff)
{
  SystemdService oService(pService);
  return oService.setAutoStart(bOnOff);
}

CcStringList CcServiceSystem::getAllServices()
{
  return SystemdService::getAllServices();
}
