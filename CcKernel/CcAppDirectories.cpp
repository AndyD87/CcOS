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
 * @brief     Class CcAppDirectories
 */
#include "CcAppDirectories.h"
#include "CcDirectory.h"
#include "CcKernel.h"

CcAppDirectories::CcAppDirectories(const CcString& sSubDir, bool bUserContext) :
  m_sSubDir(sSubDir)
{
  setupPaths(bUserContext);
}

CcAppDirectories::~CcAppDirectories()
{

}

bool CcAppDirectories::createAllPaths()
{
  bool bSuccess = true;
  if(m_sConfigDir.length() > 0)
  {
    bSuccess &= CcDirectory::create(m_sConfigDir, true);
  }
  if(m_sDataDir.length() > 0)
  {
    bSuccess &= CcDirectory::create(m_sDataDir, true);
  }
  if(m_sLogDir.length() > 0)
  {
    bSuccess &= CcDirectory::create(m_sLogDir, true);
  }
  return bSuccess;
}

void CcAppDirectories::setupPaths(bool bUserContext)
{
  if(bUserContext)
  {
    m_sConfigDir = CcKernel::getUserDataDir();
    m_sConfigDir.appendPath("etc");
    m_sConfigDir.appendPath(m_sSubDir);
    m_sDataDir = CcKernel::getUserDataDir();
    m_sDataDir.appendPath("var/lib");
    m_sDataDir.appendPath(m_sSubDir);
    m_sLogDir = CcKernel::getUserDataDir();
    m_sLogDir.appendPath("var/log");
    m_sLogDir.appendPath(m_sSubDir);
  }
  else
  {
    m_sConfigDir = CcKernel::getConfigDir();
    m_sConfigDir.appendPath(m_sSubDir);
    m_sDataDir = CcKernel::getDataDir();
    m_sDataDir.appendPath(m_sSubDir);
    m_sLogDir = CcKernel::getDataDir();
    m_sLogDir.appendPath(m_sSubDir + "/log");
  }
}
