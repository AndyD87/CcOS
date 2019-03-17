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
 * @brief     Implemtation of class CcTestFramework
 */
#include "CcTestFramework.h"
#include "CcArguments.h"
#include "CcString.h"
#include "CcFile.h"
#include "CcConsole.h"
#include "CcLog.h"
#include "CcDirectory.h"
#include "CcKernel.h"
#include "CcList.h"

class CcTestFrameworkPrivate
{
public:
  CcArguments m_oArguments;
  CcString    m_sTempDir;
  CcList<CcTestFramework::FTestCreate> m_oTestlist;
  CcLog       m_oLogFile;
  bool        m_bLogFileEnabled = false;
};

CcTestFrameworkPrivate* CcTestFramework::s_pPrivate = nullptr;
bool CcTestFramework::m_bSuccess = true;
bool CcTestFramework::m_bInstantStart = false;

CcTestFrameworkPrivate& CcTestFramework::getPrivate()
{
  if (s_pPrivate == nullptr)
  {
    s_pPrivate = new CcTestFrameworkPrivate();
    CCMONITORNEW(s_pPrivate);
  }
  return *s_pPrivate;
}

bool CcTestFramework::init(int iArgc, char** ppArgv)
{
  getPrivate().m_oArguments.init(iArgc, ppArgv);
  return true;
}

int CcTestFramework::deinit()
{
  removeTemporaryDir();
  CCDELETE(s_pPrivate);
  if (m_bSuccess)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

void CcTestFramework::writeDebug(const CcString& sMessage)
{
  CcString sFormatedMessage = CcLog::formatDebugMessage(sMessage);
  writeLine(sFormatedMessage);
}

void CcTestFramework::writeInfo(const CcString& sMessage)
{
  CcString sFormatedMessage = CcLog::formatInfoMessage(sMessage);
  writeLine(sFormatedMessage);
}

void CcTestFramework::writeError(const CcString& sMessage)
{
  CcString sFormatedMessage = CcLog::formatErrorMessage(sMessage);
  writeLine(sFormatedMessage);
}

bool CcTestFramework::enableLog(const CcString& sPathToLogFile)
{
  getPrivate().m_bLogFileEnabled = false;
  if(getPrivate().m_oLogFile.setFilePath(sPathToLogFile))
  {
    getPrivate().m_bLogFileEnabled = true;
  }
  return getPrivate().m_bLogFileEnabled;
}

CcString CcTestFramework::getBinaryDir()
{
  return getPrivate().m_oArguments.getDirectory();
}

const CcString& CcTestFramework::getTemporaryDir()
{
  if (getPrivate().m_sTempDir.length() == 0)
  {
    CcString sTempDir = CcKernel::getTempDir();
    sTempDir.appendPath("CcTestFramework");
    CcString sTempDirTest = sTempDir;
    int i = 0;
    while (CcDirectory::exists(sTempDirTest))
    {
      i++;
      sTempDirTest = sTempDir + "_" + CcString::fromNumber(i);
    }
    if (CcDirectory::create(sTempDirTest, true, true))
    {
      getPrivate().m_sTempDir = sTempDirTest;
    }
  }
  return getPrivate().m_sTempDir;
}

void CcTestFramework::removeTemporaryDir()
{
  if (getPrivate().m_sTempDir.length() > 0 &&
    CcDirectory::exists(getPrivate().m_sTempDir))
  {
    CcDirectory::remove(getPrivate().m_sTempDir, true);
  }
  getPrivate().m_sTempDir.clear();
}

const CcString& CcTestFramework::regenerateTemporaryDir()
{
  removeTemporaryDir();
  return getTemporaryDir();
}

void CcTestFramework::addTest(FTestCreate fTestCreate)
{
  if(m_bInstantStart == false)
  {
    getPrivate().m_oTestlist.append(fTestCreate);
  }
  else if(m_bSuccess)
  {
    runTest(fTestCreate);
  }
}

bool CcTestFramework::runTests()
{
  m_bSuccess = true;
  for (FTestCreate& fTestCreate : getPrivate().m_oTestlist)
  {
    if (runTest(fTestCreate) == false)
      break;
  }
  return m_bSuccess;
}

void CcTestFramework::writeLine(const CcString& sMessage)
{
  if( getPrivate().m_bLogFileEnabled)
  {
    getPrivate().m_oLogFile.writeLine(sMessage);
  }
  CcConsole::writeLine(sMessage);
}

bool CcTestFramework::runTest(FTestCreate fTestCreate)
{
  m_bSuccess = true;
  ITest* oTest = fTestCreate();
  if(oTest->getName().length() > 0)
    CcTestFramework::writeInfo("Start Test: " + oTest->getName());
  else
    CcTestFramework::writeInfo("Start next Test");
  m_bSuccess &= oTest->test();
  if(m_bSuccess)
  {
    if(oTest->getName().length() > 0)
      CcTestFramework::writeInfo("Test succeeded: " + oTest->getName());
    else
      CcTestFramework::writeInfo("Test succeeded");
  }
  else
  {
    if(oTest->getName().length() > 0)
      CcTestFramework::writeInfo("Test failed: " + oTest->getName());
    else
      CcTestFramework::writeInfo("Test failed");
  }
  CCDELETE(oTest);
  return m_bSuccess;
}
