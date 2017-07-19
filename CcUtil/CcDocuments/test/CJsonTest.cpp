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
 * @brief     Implemtation of class CJsonTest
 */
#include "CJsonTest.h"
#include "CcKernel.h"
#include "CcString.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"

const char* c_cJsonSample = 
"{                                                            \n \
\"id\": \"0001\",                                             \n \
\"type\" : \"donut\",                                         \n \
\"name\" : \"Ca\\\"ke\",                                        \n \
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


CJsonTest::CJsonTest( void )
{
}

CJsonTest::~CJsonTest( void )
{
}

bool CJsonTest::test()
{
  bool bSuccess = true;
  bSuccess &= JsonToCompact();
  bSuccess &= JsonAppendMove();
  bSuccess &= JsonBugNr1();
  return bSuccess;
}

bool CJsonTest::JsonToCompact()
{
  CcJsonDocument oJsonDoc;
  oJsonDoc.parseDocument(c_cJsonSample);
  CcJsonData oJson = oJsonDoc.getJsonData();
  CcString sJsonString = oJsonDoc.getJsonDocument();
  for (size_t i = 0; i < sJsonString.length(); i++)
  {
    if (sJsonString[i] != c_sJsonSampleCompact[i])
      CCDEBUG("asdf");
  }
  return (sJsonString == c_sJsonSampleCompact);
}

bool CJsonTest::JsonAppendMove()
{
  CcJsonDocument oJsonDoc;
  oJsonDoc.parseDocument(c_cJsonSample);
  CcJsonObject oJson1 = oJsonDoc.getJsonData().getJsonObject();
  CcJsonObject oJson2 = oJsonDoc.getJsonData().getJsonObject();
  size_t uiSizeBefore = oJson1.size();
  oJson1.append(std::move(oJson2));
  if (oJson1.size() == uiSizeBefore*2 &&
    oJson2.size() == 0)
    return true;
  return false;
}

bool CJsonTest::JsonBugNr1()
{
  CcString sReadData("{\"result\":{\"version\":130000,\"protocolversion\":70014,\"walletversion\":130000,\"balance\":0.0124321,\"blocks\":436329,\"timeoffset\":0,\"connections\":15,\"proxy\":\"\",\"difficulty\":253611.4895,\"testnet\":false,\"keypoololdest\":1475522788,\"keypoolsize\":100,\"unlocked_until\":0,\"paytxfee\":0.0,\"relayfee\":0.1,\"errors\":\"\"},\"error\":null,\"id\":1}");
  CcJsonDocument oDocument(sReadData);
  CcString sJsonReAranged = oDocument.getJsonDocument(true);
  if (sReadData == sJsonReAranged)
    return true;
  return false;
}
