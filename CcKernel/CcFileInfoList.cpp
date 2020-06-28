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
 * @brief     Class CcFileInfo
 */

#include "CcFileInfoList.h"
#include "CcStringList.h"
#include "CcStatic.h"
#include "CcGlobalStrings.h"

bool CcFileInfoList::containsFile(const CcString& sName) const
{
  bool bRet = false;
  for (CcFileInfo& oFileInfo : *this)
  {
    if (oFileInfo.getName() == sName)
    {
      bRet = true;
      break;
    }
  }
  return bRet;
}

CcFileInfo& CcFileInfoList::getFile(const CcString& sFilename)
{
  for (CcFileInfo& oFileInfo : *this)
  {
    if (oFileInfo.getName() == sFilename)
      return oFileInfo;
  }
  return CcStatic::getNullRef<CcFileInfo>();
}

const CcFileInfo& CcFileInfoList::getFile(const CcString& sFilename) const
{
  for (const CcFileInfo& oFileInfo : *this)
  {
    if (oFileInfo.getName() == sFilename)
      return oFileInfo;
  }
  return CcStatic::getConstNullRef<CcFileInfo>();
}

bool CcFileInfoList::removeFile(const CcString& sFilename)
{
  bool bRet = false;
  int i = 0;
  for (const CcFileInfo& oFileInfo : *this)
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

CcStringList CcFileInfoList::getFormatedList(EFileInfoListFormats uiShowFlags) const
{
  CcStringList slRet;
  switch(uiShowFlags)
  {
    case EFileInfoListFormats::NamesOnly:
      for (CcFileInfo& oFileInfo : *this)
      {
        slRet.append(oFileInfo.getName());
      }
      break;
    case EFileInfoListFormats::Hidden:
      for (CcFileInfo& oFileInfo : *this)
      {
        slRet.append(oFileInfo.getName());
      }
      break;
    case EFileInfoListFormats::ExtendedLs:
      for (CcFileInfo& oFileInfo : *this)
      {
        CcString appendData(oFileInfo.getAttributesString());
        appendData.append("  1");
        appendData.append(CcGlobalStrings::Space);
        appendData.append(CcString::fromNumber(oFileInfo.getUserId()));
        appendData.append(CcGlobalStrings::Space);
        appendData.append(CcString::fromNumber(oFileInfo.getGroupId()));
        appendData.append(CcGlobalStrings::Space);
        appendData.appendNumber(oFileInfo.getFileSize());
        appendData.append(oFileInfo.getModified().getString(" MM dd hh:mm "));
        //appendData.append(" Jan 1 00:00 ");
        appendData.append(oFileInfo.getName());
        slRet.append(appendData);
      }
      break;
  }
  return slRet;
}
