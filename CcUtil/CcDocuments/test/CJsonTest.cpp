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
 * @brief     Implemtation of class CJsonTest
 */
#include "CJsonTest.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonFile.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcByteArray.h"

const char* c_cJsonSample = 
"{                                                            \n \
\"id\": \"0001\",                                             \n \
\"type\" : \"donut\",                                         \n \
\"name\" : \"Ca\\\"ke\",                                      \n \
\"ppu\" : 0.55,                                               \n \
\"batters\" :                                                 \n \
{                                                             \n \
  \"batter\":                                                 \n \
  [                                                           \n \
  { \"id\": \"1001\", \"type\" : \"Regular\" },               \n \
  {\"id\": \"1002\", \"type\" : \"Chocolate\"},               \n \
  {\"id\": \"1003\", \"type\" : \"Blueberry\"},               \n \
  {\"id\": \"1004\", \"type\" : \"Devil's Food\"}             \n \
  ]                                                           \n \
},                                                            \n \
\"topping\":                                                  \n \
[                                                             \n \
{\"id\": \"5001\", \"type\" : \"None\" },                     \n \
{\"id\": \"5002\", \"type\" : \"Glazed\"},                    \n \
{\"id\": \"5005\", \"type\" : \"Sugar\"},                     \n \
{\"id\": \"5007\", \"type\" : \"Powdered Sugar\"},            \n \
{\"id\": \"5006\", \"type\" : \"Chocolate with Sprinkles\"},  \n \
{\"id\": \"5003\", \"type\" : \"Chocolate\"},                 \n \
{\"id\": \"5004\", \"type\" : \"Maple\"}                      \n \
]                                                             \n \
}                                                             \n ";
CcString c_sJsonSampleCompact = "{\"id\":\"0001\",\"type\":\"donut\",\"name\":\"Ca\\\"ke\",\"ppu\":0.55,\"batters\":{\"batter\":[{\"id\":\"1001\",\"type\":\"Regular\"},{\"id\":\"1002\",\"type\":\"Chocolate\"},{\"id\":\"1003\",\"type\":\"Blueberry\"},{\"id\":\"1004\",\"type\":\"Devil's Food\"}]},\"topping\":[{\"id\":\"5001\",\"type\":\"None\"},{\"id\":\"5002\",\"type\":\"Glazed\"},{\"id\":\"5005\",\"type\":\"Sugar\"},{\"id\":\"5007\",\"type\":\"Powdered Sugar\"},{\"id\":\"5006\",\"type\":\"Chocolate with Sprinkles\"},{\"id\":\"5003\",\"type\":\"Chocolate\"},{\"id\":\"5004\",\"type\":\"Maple\"}]}";
CcString c_sJsonSampleCompactChanged = "{\"id\":\"0002\",\"type\":\"donut\",\"name\":\"Ca\\\"ke\",\"ppu\":0.55,\"batters\":{\"batter\":[{\"id\":\"1002\",\"type\":\"Regular\"},{\"id\":\"1002\",\"type\":\"Chocolate\"},{\"id\":\"1003\",\"type\":\"Blueberry\"},{\"id\":\"1004\",\"type\":\"Devil's Food\"}]},\"topping\":[{\"id\":\"5001\",\"type\":\"None\"},{\"id\":\"5002\",\"type\":\"Glazed\"},{\"id\":\"5005\",\"type\":\"Sugar\"},{\"id\":\"5007\",\"type\":\"Powdered Sugar\"},{\"id\":\"5006\",\"type\":\"Chocolate with Sprinkles\"},{\"id\":\"5003\",\"type\":\"Chocolate\"},{\"id\":\"5004\",\"type\":\"Maple\"}]}";


CJsonTest::CJsonTest() :
  CcTest<CJsonTest>("CJsonTest")
{
  appendTestMethod("json compact test",&CJsonTest::JsonToCompact);
  appendTestMethod("json append and move nodes",&CJsonTest::JsonAppendMove);
  appendTestMethod("json read and write file",&CJsonTest::JsonFileTest);
  appendTestMethod("json reuse of json variables",&CJsonTest::JsonDocumentTestReuse);
  appendTestMethod("json bug1 test", &CJsonTest::JsonBugNr1);
}

CJsonTest::~CJsonTest()
{
}

bool CJsonTest::JsonToCompact()
{
  CcJsonDocument oJsonFile;
  oJsonFile.parseDocument(c_cJsonSample);
  CcJsonData oJson = oJsonFile.getJsonData();
  CcString sJsonString = oJsonFile.getDocument();
  return (sJsonString == c_sJsonSampleCompact);
}

bool CJsonTest::JsonAppendMove()
{
  CcJsonDocument oJsonFile;
  oJsonFile.parseDocument(c_cJsonSample);
  CcJsonObject oJson1 = oJsonFile.getJsonData().getJsonObject();
  CcJsonObject oJson2 = oJsonFile.getJsonData().getJsonObject();
  size_t uiSizeBefore = oJson1.size();
  oJson1.append(std::move(oJson2));
  if (oJson1.size() == uiSizeBefore*2 &&
    oJson2.size() == 0)
    return true;
  return false;
}

bool CJsonTest::JsonFileTest()
{
  bool bSuccess = false;
  CcJsonData oTempTestData;
  CcString sTempFile = CcKernel::getTempDir();
  sTempFile.appendPath("CJsonTestFile.tmp");
  if( !CcFile::exists(sTempFile) ||
      CcFile::remove(sTempFile)  )
  {
    CcFile oTestFile(sTempFile);
    if (oTestFile.open(EOpenFlags::Write))
    {
      if (oTestFile.writeString(c_sJsonSampleCompact))
      {
        oTestFile.close();
        CcJsonFile oJsonFile(sTempFile);
        CcJsonDocument oDocument(c_sJsonSampleCompact);
        if (oJsonFile.getDocument() == oDocument)
        {
          oTempTestData = oDocument.getJsonData();
          if(CcFile::remove(sTempFile))
          {
            bSuccess = true;
          }
          else
          {
            CCERROR("Could not delete test-file");
          }
        }
        else
        {
          CcFile::remove(sTempFile);
          CCERROR("Parsed data are not correct");
        }
      }
      else
      {
        oTestFile.close();
        CCERROR("Could not write to test-file");
      }
    }
    else
    {
      CCERROR("Could not open test-file");
    }
  }
  else
  {
    CCERROR("Could not delete test-file");
  }
  if (bSuccess && oTempTestData.isObject())
  {
    bSuccess = false;
    if (!CcFile::exists(sTempFile))
    {
      CcJsonFile oJsonFile(sTempFile);
      oJsonFile.document().getJsonData() = oTempTestData;
      if (oJsonFile.write())
      {
        CcFile oFile(sTempFile);
        if (oFile.open(EOpenFlags::Read))
        {
          CcString sData = oFile.readAll();
          if (sData == c_sJsonSampleCompact)
          {
            bSuccess = true;
          }
          else
          {
            CCERROR("Data written from JsonFile is wrong");
          }
          oFile.close();
        }
        else
        {
          CCERROR("Failed to open test file after json read");
        }
      }
      else
      {
        CCERROR("Failed to write json data");
      }
    }
    else
    {
      CCERROR("File was not deleted successfully");
    }
  }
  return bSuccess;
}

bool CJsonTest::JsonDocumentTestReuse()
{
  bool bSuccess = false;
  CcJsonDocument oJsonDoc(c_sJsonSampleCompact);
  if (oJsonDoc["id"].isValue() &&
      oJsonDoc["id"].getValue().getUint32() == 1)
  {
    oJsonDoc.parseDocument(c_sJsonSampleCompactChanged);
    if (oJsonDoc[0].isValue() &&
        oJsonDoc[0].getValue().getUint32() == 2)
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}

bool CJsonTest::JsonBugNr1()
{
  CcString sReadData("{\"result\":{\"version\":130000,\"protocolversion\":70014,\"walletversion\":130000,\"balance\":0.0124321,\"blocks\":436329,\"timeoffset\":0,\"connections\":15,\"proxy\":\"\",\"difficulty\":253611.4895,\"testnet\":false,\"keypoololdest\":1475522788,\"keypoolsize\":100,\"unlocked_until\":0,\"paytxfee\":0.0,\"relayfee\":0.1,\"errors\":\"\"},\"error\":null,\"id\":1}");
  CcJsonDocument oDocument(sReadData);
  CcString sJsonReAranged = oDocument.getDocument(true);
  if (sReadData == sJsonReAranged)
    return true;
  return false;
}
