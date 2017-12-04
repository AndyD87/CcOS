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
 * @brief     Implementation of Class CcFileSystem
 */
#include "CcFileSystem.h"
#include "CcKernel.h"
#include "CcDirectory.h"

CcString* CcFileSystem::m_WorkingDir                 = nullptr;
CcList<CcFileSystemListItem>* CcFileSystem::m_FSList = nullptr;

void CcFileSystem::init()
{
  m_FSList = new CcList<CcFileSystemListItem>();
  CCMONITORNEW(m_FSList);
  m_WorkingDir = new CcString();
  CCMONITORNEW(m_WorkingDir);
}

CcFilePointer CcFileSystem::getFile(const CcString& Path)
{
  return getFileSystemByPath(Path)->getFile(Path);
}

bool CcFileSystem::mkdir(const CcString& Path)
{
  return getFileSystemByPath(Path)->mkdir(Path);
}

bool CcFileSystem::remove(const CcString& Path)
{
  return getFileSystemByPath(Path)->remove(Path);
}

void CcFileSystem::addMountPoint(const CcString& Path, CcFileSystemHandle Filesystem)
{
  CcFileSystemListItem newItem(Path, Filesystem);
  m_FSList->append(newItem);
}

CcFileSystemHandle CcFileSystem::getFileSystemByPath(const CcString& sPath)
{
  if ( (sPath.length() > 0 &&
        sPath.at(0) == '/'    ) ||
       (sPath.length() > 1 &&
        sPath.at(1) == ':')        )
  {
    return m_FSList->at(0).getFileSystem();
  }
  else
  {
    for (size_t i = 0; i < m_FSList->size(); i++)
    {
      if (sPath.startsWith(m_FSList->at(i).getPath()))
      {
        return m_FSList->at(i).getFileSystem();
      }
    }
  }
  return m_FSList->at(0).getFileSystem();
}
