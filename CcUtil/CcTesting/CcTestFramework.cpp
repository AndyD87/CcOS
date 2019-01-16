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

class CcTestFrameworkPrivate
{
public:
  CcArguments m_oArguments;
  CcString    m_sTempDir;
};

CcTestFrameworkPrivate* CcTestFramework::s_pPrivate = nullptr;

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

bool CcTestFramework::deinit()
{
  removeTemporaryDir();
  CCDELETE(s_pPrivate);
  return true;
}

void CcTestFramework::writeDebug(const CcString& sMessage)
{
}

void CcTestFramework::writeInfo(const CcString& sMessage)
{
}

void CcTestFramework::writeError(const CcString& sMessage)
{
}

void CcTestFramework::enableLog(const CcString& sPathToLogFile)
{
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
