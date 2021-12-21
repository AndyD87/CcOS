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

class CcTestFramework::CPrivate
{
public:
  CcArguments m_oArguments;
  CcString    m_sTempDir;
  CcList<CcTestFramework::FTestCreate> m_oTestlist;
  CcLog       m_oLogFile;
  bool        m_bLogFileEnabled = false;
  size_t      m_uiRepeats = 1;
};

CcTestFramework::CPrivate* CcTestFramework::s_pPrivate = nullptr;
bool CcTestFramework::s_bSuccess = true;
bool CcTestFramework::s_bInstantStart = false;

CcTestFramework::CPrivate& CcTestFramework::getPrivate()
{
  if (s_pPrivate == nullptr)
  {
    CCNEW(s_pPrivate, CPrivate);
  }
  return *s_pPrivate;
}

bool CcTestFramework::init(int iArgc, char** ppArgv)
{
  bool bRet = true;
  getPrivate().m_oArguments.init(iArgc, ppArgv);
  for (size_t i = 0; i < getPrivate().m_oArguments.size(); i++)
  {
    if (getPrivate().m_oArguments[i].compareInsensitve("repeats"))
    {
      getPrivate().m_oArguments.remove(i);
      if (i < getPrivate().m_oArguments.size())
      {
        getPrivate().m_uiRepeats = getPrivate().m_oArguments[i].toUint32(&bRet);
        if (!bRet)
          CCERROR("Failed to parse repeats value");
      }
      else
      {
        CCERROR("integer value required after repeates argument");
        bRet = false;
      }
    }
  }
  CcTestFramework::s_bSuccess &= bRet;
  return bRet;
}

int CcTestFramework::deinit(bool bStopKernel)
{
  // Remove Test dir
  removeTemporaryDir();
  // Cleanup private
  CCDELETE(s_pPrivate);

  // Send stop kernel if test for shutdown is required.
  if(bStopKernel) CcKernel::shutdown();

  if (s_bSuccess)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

void CcTestFramework::setInstantStart()
{
  s_bInstantStart = true;
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
      sTempDirTest = sTempDir + "_" + CcString::fromInt(i);
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
  if(s_bInstantStart == false)
  {
    getPrivate().m_oTestlist.append(fTestCreate);
  }
  else if(s_bSuccess)
  {
    runTest(fTestCreate);
  }
}

bool CcTestFramework::runTests()
{
  s_bSuccess = true;
  while (getPrivate().m_uiRepeats-- && CcTestFramework::s_bSuccess)
  {
    for (FTestCreate& fTestCreate : getPrivate().m_oTestlist)
    {
      if (runTest(fTestCreate) == false)
        break;
    }
  }
  return s_bSuccess;
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
  s_bSuccess = true;
  ITest* pTest = fTestCreate(nullptr);
  if(pTest->getName().length() > 0)
    CcTestFramework::writeInfo("Start Test: " + pTest->getName());
  else
    CcTestFramework::writeInfo("Start next Test");
  s_bSuccess &= pTest->test();
  if(s_bSuccess)
  {
    if(pTest->getName().length() > 0)
      CcTestFramework::writeInfo("Test succeeded: " + pTest->getName());
    else
      CcTestFramework::writeInfo("Test succeeded");
  }
  else
  {
    if(pTest->getName().length() > 0)
      CcTestFramework::writeInfo("Test failed: " + pTest->getName());
    else
      CcTestFramework::writeInfo("Test failed");
  }
  // Cleanup test
  pTest->cleanup();
  fTestCreate(pTest);
  return s_bSuccess;
}
