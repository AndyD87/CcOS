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

class CcServiceSystem::CPrivate
{
public:
  static CcWindowsService*  s_pService;
};

CcWindowsService* CcServiceSystem::CPrivate::s_pService(nullptr);

CcServiceSystem::CcServiceSystem()
{
}

CcServiceSystem::~CcServiceSystem()
{
  if (CPrivate::s_pService)
    CCDELETE(CPrivate::s_pService);
}

CcStatus CcServiceSystem::init(CcService* pService)
{
  CPrivate::s_pService = new CcWindowsService(pService);
  return CPrivate::s_pService->init();
}

CcStatus CcServiceSystem::create(CcService* pService)
{
  CcWindowsServiceControl oControl(pService->getName());
  return oControl.create();
}

CcStatus CcServiceSystem::remove(CcService* pService)
{
  CcWindowsServiceControl oControl(pService->getName());
  return oControl.remove();
}

CcStatus CcServiceSystem::stop(CcService* pService)
{
  CcWindowsServiceControl oControl(pService->getName());
  return oControl.stop();
}

CcStatus CcServiceSystem::start(CcService* pService)
{
  CcWindowsServiceControl oControl(pService->getName());
  return oControl.start();
}

CcStatus  CcServiceSystem::setAutoStart(CcService* pService, bool bOnOff)
{
  CcWindowsServiceControl oControl(pService->getName());
  if(bOnOff)
    return oControl.setStartType(CcWindowsServiceControl::EStartType::AutoStart);
  else
    return oControl.setStartType(CcWindowsServiceControl::EStartType::DemandStart);
}