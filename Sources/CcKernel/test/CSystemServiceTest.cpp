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
 * @brief     Implemtation of class CSystemServiceTest
 */
#include "CSystemServiceTest.h"
#include "CcServiceSystem.h"
#include "CcKernel.h"
#include "CcService.h"
#include "CcFile.h"

CSystemServiceTest::CSystemServiceTest() :
  CcTest("CSystemServiceTest")
{
  appendTestMethod("Test list services", &CSystemServiceTest::testServiceListing);
  if (CcKernel::isAdmin())
  {
    appendTestMethod("Test list services", &CSystemServiceTest::testServiceCreate);
    appendTestMethod("Test list services", &CSystemServiceTest::testServiceSetExecutable);
    appendTestMethod("Test list services", &CSystemServiceTest::testServiceRemove);
  }
}

CSystemServiceTest::~CSystemServiceTest()
{
}

void CSystemServiceTest::cleanup()
{
  // Remove testservice
  CcService oService("CcOSTestService", nullptr);
  CcKernel::getServiceSystem().remove(oService);
  if(m_sTempDir.size() > 0)
  {
    // Remove temporary executable
    CcFile::remove(m_sTempDir);
  }
}

bool CSystemServiceTest::testServiceListing()
{
  bool bRet = false;
  CcStringList oServices = CcKernel::getServiceSystem().getAllServices();
  if(oServices.size() > 0)
  {
    bRet = true;
  }
  return bRet;
}

bool CSystemServiceTest::testServiceCreate()
{
  bool bRet = false;
  CcService oService("CcOSTestService", nullptr);
  m_sCurrentBinPath = CcKernel::getCurrentExecutablePath();
  m_sTempDir        = CcString(CcTestFramework::getTemporaryDir()).appendPath("CcOSTestService");
  if (CcFile::copy(m_sCurrentBinPath, m_sTempDir))
  {
    if (CcKernel::getServiceSystem().create(oService))
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CSystemServiceTest::testServiceSetExecutable()
{
  bool bRet = false;
  CcService oService("CcOSTestService", nullptr);
  if (CcKernel::getServiceSystem().setExectuable(oService, m_sTempDir))
  {
    bRet = true;
  }
  return bRet;
}

bool CSystemServiceTest::testServiceRemove()
{
  bool bRet = false;
  CcService oService("CcOSTestService", nullptr);
  if (CcKernel::getServiceSystem().remove(oService))
  {
    bRet = CcFile::remove(m_sTempDir);
  }
  return bRet;
}