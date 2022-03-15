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
 * @brief     Implementation of Class CcRestApiUser
 */
#include "CcRestApiUser.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "IHttpUser.h"

CcRestApiUser::CcRestApiUser(IRestApi *pParent) :
  IRestApi(pParent, "user")
{
}

CcRestApiUser::~CcRestApiUser()
{
}

bool CcRestApiUser::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  IHttpUser* pUser = getUser(oData);
  if (!pUser)
  {
    oData.getResponse().setError(CcHttpGlobals::EError::ErrorAuthRequired);
    oData.sendHeader();
  }
  else
  {
    CcJsonDocument oDoc;
    oData.sendHeader();
    oDoc.getJsonData().setJsonObject().add(CcJsonNode("Name", pUser->getName()));
    oData.write(oDoc.getDocument());
  }
  return bSuccess;
}

bool CcRestApiUser::put(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  IHttpUser* pUser = getUser(oData);
  if (!pUser)
  {
    oData.getResponse().setError(CcHttpGlobals::EError::ErrorUnauthorized);
    oData.sendHeader();
  }
  else
  {
    CcJsonDocument oDoc;
    oData.sendHeader();
    oData.write(oDoc.getDocument());
  }
  return bSuccess;
}
