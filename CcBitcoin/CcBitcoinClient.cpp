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
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcBitcoinClient
 */
#include "CcBitcoinClient.h"
#include "CcByteArray.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonNode.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"

CcBitcoinClient::CcBitcoinClient() :
  m_sUser("Testnet"),
  m_oPassword("Testpass")
{
}

CcBitcoinClient::~CcBitcoinClient()
{
}

CcByteArray CcBitcoinClient::getWork()
{
  CcString sTemplate = getBlocktemplate();
  CcJsonDocument oDoc(sTemplate);
  if(oDoc.getJsonNode().isObject())
  {
    CcJsonNode& rResult= oDoc.getJsonNode()["result"];
    if(rResult.isNotNull() &&
       rResult.isObject())
    {
      CcJsonNode& rNode= rResult["transactions"];
      if(rNode.isNotNull() &&
         rNode.isArray())
      {
        while(rNode.array().size() > 1)
          rNode.array().remove(1);
      }
    }
  }
  CCDEBUG(oDoc.getDocument(false));
  CcByteArray oData;
  return oData;
}

CcString CcBitcoinClient::getInfo()
{
  m_oClient.setRequestString("{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getblockchaininfo\", \"params\": []}");
  m_oClient.headerRequest().setContentType("text/plain");
  m_oClient.headerRequest().setAuthorization(m_sUser, m_oPassword.getString());
  if(m_oClient.execPost())
  {
    return m_oClient.getByteArray();
  }
  return "";
}

CcString CcBitcoinClient::getBlocktemplate()
{
  CcString sRequest = "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getblocktemplate\", \"params\": [{"
                        "\"mode\": \"template\","
                        "\"capabilities\": [\"coinbasetxn\", \"workid\", \"coinbase/append\"],"
                        "\"rules\": [\"segwit\"]"
                      "}]}";
  m_oClient.setRequestString(sRequest);
  m_oClient.headerRequest().setContentType("text/plain");
  m_oClient.headerRequest().setAuthorization(m_sUser, m_oPassword.getString());
  if(m_oClient.execPost())
  {
    return m_oClient.getByteArray();
  }
  return "";
}
