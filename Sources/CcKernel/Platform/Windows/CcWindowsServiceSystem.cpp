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

#include "CcWindowsGlobals.h"
#include "CcWindowsService.h"
#include "CcWindowsServiceControl.h"
#include "CcServiceSystem.h"
#include "CcVector.h"

class CcServiceSystem::CPrivate
{
public:
  CcVector<CcWindowsService*> m_oServices;
} g_oInstance;

CcServiceSystem::CcServiceSystem()
{
  m_pPrivate = &g_oInstance;
}

CcServiceSystem::~CcServiceSystem()
{
  for (CcWindowsService* pWinService : m_pPrivate->m_oServices)
  {
    pWinService->deinit();
    CCDELETE(pWinService);
  }
  m_pPrivate->m_oServices.clear();
}

CcStatus CcServiceSystem::init(CcService& pService)
{
  CCNEWTYPE(pWinService, CcWindowsService, pService);
  m_pPrivate->m_oServices.append(pWinService);
  return pWinService->init();
}

CcStatus CcServiceSystem::deinit(CcService& pService)
{
  CcStatus oStatus(EStatus::NotFound);
  for (CcWindowsService* pWinService : m_pPrivate->m_oServices)
  {
    if (*pWinService == pService)
    {
      m_pPrivate->m_oServices.removeItem(pWinService);
      pWinService->deinit();
      CCDELETE(pWinService);
    }
  }
  return oStatus;
}

void CcServiceSystem::stop()
{
  for (CcWindowsService* pService : m_pPrivate->m_oServices)
  {
    CCDELETE(pService);
  }
}

CcStatus CcServiceSystem::create(CcService& pService)
{
  CcWindowsServiceControl oControl(pService.getName());
  return oControl.create();
}

CcStatus CcServiceSystem::remove(CcService& pService)
{
  CcWindowsServiceControl oControl(pService.getName());
  return oControl.remove();
}

CcStatus CcServiceSystem::stop(CcService& pService)
{
  CcWindowsServiceControl oControl(pService.getName());
  return oControl.stop();
}

CcStatus CcServiceSystem::start(CcService& pService)
{
  CcWindowsServiceControl oControl(pService.getName());
  return oControl.start();
}

CcStatus CcServiceSystem::setArguments(CcService& pService, const CcArguments& oArugments)
{
  CcWindowsServiceControl oControl(pService.getName());
  return oControl.setArguments(oArugments);
}

CcStatus CcServiceSystem::setAutoStart(CcService& pService, bool bOnOff)
{
  CcWindowsServiceControl oControl(pService.getName());
  if(bOnOff)
    return oControl.setStartType(CcWindowsServiceControl::EStartType::AutoStart);
  else
    return oControl.setStartType(CcWindowsServiceControl::EStartType::DemandStart);
}

CcStatus CcServiceSystem::setExectuable(CcService& pService, const CcString& sExePath)
{
  CcWindowsServiceControl oControl(pService.getName());
  return oControl.setExectuable(sExePath);
}

CcStatus CcServiceSystem::setWorkingDir(CcService& pService, const CcString& sWorkingDir)
{
  CcStatus oStatus(EStatus::NotSupported);
  CCUNUSED(pService);
  CCUNUSED(sWorkingDir);
  return oStatus;
}

CcStringList CcServiceSystem::getAllServices()
{
  return CcWindowsServiceControl::getAllServices();
}

