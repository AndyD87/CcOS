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
 * @brief     Implemtation of class CJsonFixes
 */
#include "CJsonFixes.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonFile.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcByteArray.h"

CJsonFixes::CJsonFixes() :
  CcTest<CJsonFixes>("CJsonFixes")
{
  appendTestMethod("Check for double entries in list",&CJsonFixes::doubleEntries);
}

CJsonFixes::~CJsonFixes()
{
}

bool CJsonFixes::doubleEntries()
{
  bool bSuccess = false;
  const char pTestData[] = "{\"Command\":768,\"Directories\":[{\"Id\":5722,\"DirId\":5712,\"Name\":\"dir1\","
                           "\"Modified\":1553498064,\"MD5\":\"7175263a4b921ac935b6831ed4d2641d\",\"IsFile\":false},"
                           "{\"Id\":5731,\"DirId\":5712,\"Name\":\"dir2\",\"Modified\":1553498265,\"MD5\":\"5f0911c5c8ef393f0ab768e90842c89b\","
                           "\"IsFile\":false},{\"Id\":5713,\"DirId\":5712,\"Name\":\"dir3\",\"Modified\":1553498061,\"MD5\":"
                           "\"1eee6b637b4afc5165796e5a899f996c\",\"IsFile\":false}],\"Files\":[]}";
  CcString sJsonString(pTestData);
  CcJsonDocument oJsonDoc(sJsonString);
  if (oJsonDoc.getJsonData().isObject())
  {
    CcJsonObject oObject = oJsonDoc.getJsonData().getJsonObject();
    if (oObject.contains("Files") &&
        oObject["Files"].isArray())
    {
      CcJsonArray oFiles = oObject["Files"].getJsonArray();
      if (oFiles.size() == 0)
      {
        bSuccess = true;
      }
    }
  }
  return bSuccess;
}
