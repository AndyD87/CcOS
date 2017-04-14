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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcFileSystemManager
 */
#include "CcFileSystemManager.h"
#include "CcKernel.h"
#include "CcDirectory.h"


CcFileSystemManager::CcFileSystemManager(void)
{
}

CcFileSystemManager::~CcFileSystemManager(void)
{
}

CcFilePointer CcFileSystemManager::getFile(const CcString& Path) const
{
  return getFileSystemManagerByPath(Path)->getFile(Path);
}

bool CcFileSystemManager::mkdir(const CcString& Path) const
{
  return getFileSystemManagerByPath(Path)->mkdir(Path);
}

bool CcFileSystemManager::remove(const CcString& Path) const
{
  return getFileSystemManagerByPath(Path)->remove(Path);
}

const CcString& CcFileSystemManager::getWorkingDir(void) const
{
  return m_WorkingDir;
}

void CcFileSystemManager::setWorkingDir(const CcString& Path)
{
  //todo: check for existense
  m_WorkingDir = Path;
}

void CcFileSystemManager::addMountPoint(const CcString& Path, CcHandle<CcFileSystem> Filesystem)
{
  CcFileSystemListItem newItem(Path, Filesystem);
  m_FSList.append(newItem);
}

CcHandle<CcFileSystem> CcFileSystemManager::getFileSystemManagerByPath(const CcString& sPath) const
{
  if ( (sPath.length() > 0 &&
        sPath.at(0) == '/'    ) ||
       (sPath.length() > 1 &&
        sPath.at(1) == ':')        )
  {
    return m_FSList.at(0).getFileSystem();
  }
  else
  {
    for (size_t i = 0; i < m_FSList.size(); i++)
    {
      if (sPath.startWith(m_FSList.at(i).getPath()))
      {
        return m_FSList.at(i).getFileSystem();
      }
    }
  }
  return m_FSList.at(0).getFileSystem();
}
