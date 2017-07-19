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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcDevice
 */

#include "CcFileInfoList.h"
#include "CcStringList.h"

CcFileInfoList::CcFileInfoList()
{
}

CcFileInfoList::~CcFileInfoList() 
{
}

bool CcFileInfoList::contains(const CcString& sName)
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

CcStringList CcFileInfoList::getFormatedList(uint8 uiShowFlags) const
{
  CcStringList slRet;
  if(uiShowFlags > 0)
  {
    for (CcFileInfo& oFileInfo : *this)
    {
      slRet.append(oFileInfo.getName());
    }
  }
  else
  {
    for (CcFileInfo& oFileInfo : *this)
    {
      CcString appendData(oFileInfo.getFlagsString());
      appendData.append("  1");
      appendData.append(" ");
      appendData.append(CcString::fromNumber(oFileInfo.getUserId()));
      appendData.append(" ");
      appendData.append(CcString::fromNumber(oFileInfo.getGroupId()));
      appendData.append(" ");
      appendData.appendNumber(oFileInfo.getFileSize());
      appendData.append(oFileInfo.getModified().getString(" MM dd hh:mm "));
      //appendData.append(" Jan 1 00:00 ");
      appendData.append(oFileInfo.getName());
      slRet.append(appendData);
    }
  }
  return slRet;
}
