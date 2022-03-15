/*
 * This file is part of CUsers.
 *
 * CUsers is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CUsers is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CUsers.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CUsers
 */
#include "CUsers.h"
#include "NDocumentsGlobals.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
{

void CUsers::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if (rNode.isObject())
      {
        if (rNode.getName() == NDocumentsGlobals::NConfig::User)
        {
          const CcJsonNode& rName = rNode.getJsonObject()[NDocumentsGlobals::NConfig::Name];
          const CcJsonNode& rPassword = rNode.getJsonObject()[NDocumentsGlobals::NConfig::Password];
          if (rName.isValue()     && rName.getValue().isString() &&
              rPassword.isValue() && rPassword.getValue().isString())
          {
            CCredentials oCredential(rName.getValue().getString(), rPassword.getValue().getString());
            oUsers.append(oCredential);
          }
        }
      }
      else if (rNode.isValue())
      {
        if (rNode.getName() == NDocumentsGlobals::NConfig::Admin &&
            rNode.value().isString())
        {
          sAdmin = rNode.value().getString();
          CCDEBUG("CcRemoteDevice device admin: " + sAdmin);
        }
      }
    }
  }
}

void CUsers::writeJson(CcJsonNode& rNode)
{
  if (rNode.isObject())
  {
    if (sAdmin.length() > 0)
    {
      rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Admin, sAdmin));
    }
    for (CCredentials& oCred : oUsers)
    {
      CcJsonNode oUser(EJsonDataType::Object);
      oUser.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Name, oCred.sUsername));
      oUser.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Password, oCred.sPassword));
      rNode.object().append(oUser);
    }
  }
}

const CcConfigBinary::CItem* CUsers::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::Admin:
      {
        sAdmin = pItem->getString();
        break;
      }
      case CcConfigBinary::EType::User:
      {
        bAllOk = pItem->getInner(pItem, uiMaxSize);
        CcString sUser;
        CcString sPassword;
        while (pItem->isEnd() == false && bAllOk)
        {
          switch (pItem->getType())
          {
            case CcConfigBinary::EType::Name:
            {
              sUser = pItem->getString();
              break;
            }
            case CcConfigBinary::EType::Password:
            {
              sPassword = pItem->getString();
              break;
            }
            default:
              break;
          }
          if (bAllOk)
            bAllOk = pItem->getNext(pItem, uiMaxSize);
        }
        if (sUser.length() && sPassword.length())
        {
          oUsers.append(CCredentials(sUser, sPassword));
        }
        break;
      }
      default:
        CCERROR("Wrong config item");
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CUsers::writeBinary(IIo& pStream)
{
  size_t uiWrittenTemp = 0;
  size_t uiWritten = 0;
  if (sAdmin.length())
  {
    uiWrittenTemp = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Admin);
    if (uiWrittenTemp != SIZE_MAX) uiWritten += uiWrittenTemp;
    else uiWritten = SIZE_MAX;
  }
  if (oUsers.size())
  {
    size_t uiWritten2 = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::User);
    if (uiWritten2 != SIZE_MAX)
    {
      for (CCredentials& oCred : oUsers)
      {
        uiWrittenTemp += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Name, oCred.sUsername);
        if (uiWrittenTemp != SIZE_MAX)
        {
          uiWritten2 += uiWrittenTemp;
          uiWrittenTemp += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Password, oCred.sPassword);
          if (uiWrittenTemp != SIZE_MAX) uiWritten2 += uiWrittenTemp;
          else uiWritten2 = SIZE_MAX;
        }
        else uiWritten2 = SIZE_MAX;
      }
    }
    if (uiWritten2 != SIZE_MAX)
    {
      uiWrittenTemp = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
      if (uiWrittenTemp != SIZE_MAX)
      {
        uiWritten2 += uiWrittenTemp;
        uiWritten  += uiWritten2;
      }
      else uiWritten = SIZE_MAX;
    }
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWrittenTemp = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
    if (uiWrittenTemp != SIZE_MAX) uiWritten += uiWrittenTemp;
    else uiWritten = SIZE_MAX;
  }
  return uiWritten;
}

}
}
}
