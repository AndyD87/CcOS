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
* @brief     Implemtation of class CcSyncLocation
*/
#include "CcSyncFileInfoList.h"
#include "CcSyncGlobals.h"

bool CcSyncFileInfoList::containsFile(const CcString& sFilename) const
{
  bool bRet = false;
  for (CcSyncFileInfo& oFileInfo : *this)
  {
    if (oFileInfo.name() == sFilename)
      return true;
  }
  return bRet;
}

const CcSyncFileInfo& CcSyncFileInfoList::getFile(const CcString& sFilename) const
{
  for (const CcSyncFileInfo& oFileInfo : *this)
  {
    if (oFileInfo.getName() == sFilename)
      return oFileInfo;
  }
  return CCNULLREF(CcSyncFileInfo);
}

bool CcSyncFileInfoList::removeFile(const CcString& sFilename)
{
  bool bRet = false;
  int i = 0;
  for (const CcSyncFileInfo& oFileInfo : *this)
  {
    if (oFileInfo.getName() == sFilename)
    {
      remove(i);
      bRet = true;
      break;
    }
    i++;
  }
  return bRet;
}