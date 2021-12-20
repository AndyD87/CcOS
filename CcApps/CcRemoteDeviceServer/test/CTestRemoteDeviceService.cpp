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
 * @brief     Implemtation of class CTestRemoteDeviceService
 */
#include "CTestRemoteDeviceService.h"
#include "CcTestFramework.h"
#include "CcTestUtility.h"
#include "CcProcess.h"
#include "CcFile.h"
#include "CcKernel.h"

#define SERVCE_APPLICATION_NAME "CcTestServiceInstallation"

CTestRemoteDeviceService::CTestRemoteDeviceService() :
  CcTest<CTestRemoteDeviceService>("CTestRemoteDeviceService")
{
  m_sApplicationDir = CcKernel::getCurrentExecutablePath().appendPath("..").normalizePath();
  m_sApplication = m_sApplicationDir;
  m_sApplication.appendPath("CcRemoteDeviceServer");
  appendTestMethod("Add service to system", &CTestRemoteDeviceService::createService);
  appendTestMethod("Remove service from system", &CTestRemoteDeviceService::removeService);
}

CTestRemoteDeviceService::~CTestRemoteDeviceService()
{
}

bool CTestRemoteDeviceService::createService()
{
  bool bSuccess = false;
  CcProcess oRemoteDeviceService(m_sApplication);
  oRemoteDeviceService.addArgument("Remove");
  oRemoteDeviceService.addArgument(SERVCE_APPLICATION_NAME);
  bSuccess = oRemoteDeviceService.exec(CcDateTimeFromSeconds(1));

  oRemoteDeviceService.addArgument("Create");
  oRemoteDeviceService.addArgument(SERVCE_APPLICATION_NAME);
  bSuccess = oRemoteDeviceService.exec(CcDateTimeFromSeconds(1)); 
  return bSuccess;
}

bool CTestRemoteDeviceService::removeService()
{
  bool bSuccess = false;
  CcProcess oRemoteDeviceService(m_sApplication);
  oRemoteDeviceService.addArgument("Remove");
  oRemoteDeviceService.addArgument(SERVCE_APPLICATION_NAME);
  bSuccess = oRemoteDeviceService.exec(CcDateTimeFromSeconds(1));
  return bSuccess;
}
