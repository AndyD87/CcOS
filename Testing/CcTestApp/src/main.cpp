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
 * @par       Web: 
 * @version    0.01
 * @date       2016-04
 * @par        Language   C++ ANSI V3
 * @file     main.cpp
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CcSqlDatabase.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "Devices/CcGPIOPort.h"
#include "CcHttpClient.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "MainApp.h"
#include "CcFileInfoList.h"
#include "CcDirectory.h"
#include <list>

// Application entry point. 
int main(int argc, char **argv)
{
  CcKernel::setArg(argc, argv);
  CcKernel::initCLI();

  CcFileInfoList oFileList = CcDirectory::getFileList("E:/");

  for (char i = 0; i < 100; i++)
  {
    CcByteArray oTestArray("a");
    CcString sBase64;
    CcByteArray oDecodedArray;
    for (char c = 0; c < i; c++) oTestArray.append(&c, 1);
    //sBase64 = CcStringUtil::encodeBase64(oTestArray);
    //CCDEBUG(sBase64);
    //oDecodedArray = CcStringUtil::decodeBase64(sBase64);
    if (oDecodedArray != oTestArray)
    {
      CCDEBUG(oTestArray);
      CCDEBUG(oDecodedArray);
    }
  }
  return 0;
}
