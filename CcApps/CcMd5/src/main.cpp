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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version    0.01
 * @date       2016-04
 * @par        Language   C++ ANSI V3
 * @file     main.cpp
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "Types/CcArguments.h"
#include "Types/CcError.h"
#include "Types/CcByteArray.h"
#include "Hash/CcCrc32.h"

int main(int argc, char **argv)
{
  int iRet = 0;
  CcArguments oArguments(argc, argv);
  CcKernel::initCLI();

  if (oArguments.size() > 2)
  {
    if (oArguments[1].compareInsensitve("--file") ||
        oArguments[1].compareInsensitve("-f"))
    {
      CcFile oFile(oArguments[2]);
      if (oFile.exists())
      {
        CcString sCrcString = CcString::fromNumber(oFile.getCrc32().getValue(), 16);
        sCrcString.fillBeginUpToLength('0', 8);
      }
      else
      {
        CcConsole::writeLine("File not found");
        iRet = (int)EError::FSFileNotFound;
      }
    }
    else if (oArguments[1].compareInsensitve("--string") ||
             oArguments[1].compareInsensitve("-s"))
    {
      CcCrc32 oCrc;
      oCrc.generate(oArguments[2]);
      CcString sCrcString = CcString::fromNumber(oCrc.getValue(), 16);
      sCrcString.fillBeginUpToLength('0', 8);
    }
  }
  else if (oArguments.size() == 2)
  {
    CcFile oFile(oArguments[1]);
    if (oFile.exists())
    {
      CcString sCrcString = CcString::fromNumber(oFile.getCrc32().getValue(), 16);
      sCrcString.fillBeginUpToLength('0', 8);
    }
    else
    {
      CcConsole::writeLine("File not found");
      iRet = (int) EError::FSFileNotFound;
    }
  }
  else
  {
    // @todo print help
  }

  return iRet;
}
