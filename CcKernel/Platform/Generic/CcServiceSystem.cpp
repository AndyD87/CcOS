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

class CcServiceSystem::CPrivate
{
public:
};

CcServiceSystem::CcServiceSystem()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcServiceSystem::~CcServiceSystem()
{
  CCDELETE(m_pPrivate);
}

CcStatus CcServiceSystem::init(CcService& pService)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  return oStatus;
}

CcStatus CcServiceSystem::deinit(CcService& pService)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  return oStatus;
}

CcStatus CcServiceSystem::create(CcService& pService)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  return oStatus;
}

CcStatus CcServiceSystem::remove(CcService& pService)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  return oStatus;
}

CcStatus CcServiceSystem::stop(CcService& pService)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  return oStatus;
}

CcStatus CcServiceSystem::start(CcService& pService)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  return oStatus;
}

CcStatus CcServiceSystem::setArguments(CcService& pService, const CcArguments& oArugments)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  CCUNUSED(oArugments);
  return oStatus;
}

CcStatus CcServiceSystem::setAutoStart(CcService& pService, bool bOnOff)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  CCUNUSED(bOnOff);
  return oStatus;
}

CcStatus CcServiceSystem::setExectuable(CcService& pService, const CcString& sExePath)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  CCUNUSED(sExePath);
  return oStatus;
}

CcStatus CcServiceSystem::setWorkingDir(CcService& pService, const CcString& sWorkingDir)
{
  CcStatus oStatus(false);
  CCUNUSED(pService);
  CCUNUSED(sWorkingDir);
  return oStatus;
}

CcStringList CcServiceSystem::getAllServices()
{
  return CcStringList();
}

