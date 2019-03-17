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

CcVector<CcFileSystemListItem>* CcFileSystem::m_FSList = nullptr;

void CcFileSystem::init()
{
  m_FSList = new CcVector<CcFileSystemListItem>();
  CCMONITORNEW(m_FSList);
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

void CcFileSystem::addMountPoint(const CcString& sPath, CcFileSystemHandle hFilesystem)
{
  CcFileSystemListItem newItem(sPath, hFilesystem);
  bool bAdded = false;
  for (size_t i = 0; i < m_FSList->size() && bAdded == false; i++)
  {
    if (sPath.startsWith(m_FSList->at(i).getPath()))
    {
      m_FSList->insert(i, newItem);
      bAdded = true;
    }
  }
  if (bAdded == false)
  {
    m_FSList->append(newItem);
  }
}

CcFileSystemHandle CcFileSystem::getFileSystemByPath(const CcString& sPath)
{
  CcFileSystemHandle pFileSystem;
  // search in registered providers
  for (size_t i = 0; i < m_FSList->size(); i++)
  {
    if (sPath.startsWith(m_FSList->at(i).getPath()))
    {
      pFileSystem = m_FSList->at(i).getFileSystem();
      break;
    }
  }

  if(!pFileSystem.isValid() && m_FSList->size() > 0)
  {
    // Take root file system if nothing es found
    pFileSystem = m_FSList->at(0).getFileSystem();
  }
  return pFileSystem;
}
