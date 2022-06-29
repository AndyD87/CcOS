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
 * @brief     Class IShell
 */
#include "CcShellExport.h"
#include "IShell.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"

CcShellExport::CcShellExport() :
  IShellCommand("export")
{
}

CcShellExport::~CcShellExport()
{
}

CcStatus CcShellExport::exec(IShell& oBasicShell, const CcStringList& oArguments)
{
  CcStatus oSuccess;
  if(oArguments.size() == 0)
  {
      oBasicShell.writeLine("Additional Parameter required");
      oSuccess = EStatus::CommandInvalidParameter;
  }
  else
  {
    CcString sName;
    CcString sValue;
    if(CcStringUtil::extractKeyValue(oArguments[0], sName, sValue))
    {
      oBasicShell.setEnvironmentVariable(sName, sValue);
    }
    else
    {
      oBasicShell.writeLine("Additional Parameter required");
      oSuccess = EStatus::CommandInvalidParameter;
    }
  }
  return oSuccess;
}
