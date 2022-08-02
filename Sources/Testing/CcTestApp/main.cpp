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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "MainApp.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcBufferList.h"
#include "CcArguments.h"
#include "CcMemoryMonitor.h"
#include "CcExportTest.h"

/**
 * @brief Default application entry point
 * @param iArgc:  Argument count in ppArgv
 * @param ppArgv: Passed arguments from callup
 * @return Exitcode, default 0 if no error occured
 */
int main(int iArgc, char** ppArgv)
{
  int iRet = 0;
  CcExportTestList oList;
  CcExportTestVector oVector;
  CcArguments oArguments({}, iArgc, ppArgv);
  CcString sPath = oArguments.getPath();
  CcString sApplication = oArguments.getApplication();
  CcString sDirectory = oArguments.getDirectory();
  MainApp oMainApp;
  iRet = oMainApp.exec().getErrorInt();
  return iRet;
}
