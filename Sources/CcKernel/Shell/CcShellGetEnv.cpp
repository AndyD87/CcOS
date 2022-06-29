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
#include "CcShellGetEnv.h"
#include "IShell.h"
#include "CcStringList.h"
#include "CcDirectory.h"
#include "CcFile.h"
#include "CcGlobalStrings.h"

CcShellGetEnv::CcShellGetEnv() :
  IShellCommand("getenv")
{
}

CcShellGetEnv::~CcShellGetEnv()
{
}

CcStatus CcShellGetEnv::exec(IShell& oBasicShell, const CcStringList& oArguments)
{
  CcStatus oSuccess;
  if(oArguments.size() > 0)
  {
    size_t uiOffset = oBasicShell.getEnvironmentVariables().findKey(oArguments[0]);
    if(uiOffset < oBasicShell.getEnvironmentVariables().size())
    {
      const CcStringPair& oPair = oBasicShell.getEnvironmentVariables()[uiOffset];
      oBasicShell.writeLine(oPair.getKey() + CcGlobalStrings::Seperators::Equal + oPair.getValue());
    }
    else
    {
      oBasicShell.writeLine("Environment variable not found: " + oArguments[0]);
      oSuccess = EStatus::FSFileNotFound;
    }
  }
  else
  {
    for(const CcStringPair& oPair : oBasicShell.getEnvironmentVariables())
    {
      oBasicShell.writeLine(oPair.getKey() + CcGlobalStrings::Seperators::Equal + oPair.getValue());
    }
  }
  return oSuccess;
}
