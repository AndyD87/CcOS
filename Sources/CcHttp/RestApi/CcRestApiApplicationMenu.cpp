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
 * @brief     Implementation of Class CcRestApiApplicationMenu
 */
#include "CcRestApiApplicationMenu.h"
#include "CcHttpWorkData.h"
#include "Json/CcJsonDocument.h"
#include "Json/CcJsonArray.h"
#include "Json/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiApplication.h"

CcRestApiApplicationMenu::CcRestApiApplicationMenu(CcRestApiApplication *pParent) :
  IRestApi(pParent, "menu")
{
}

CcRestApiApplicationMenu::~CcRestApiApplicationMenu()
{
}

bool CcRestApiApplicationMenu::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.sendHeader();
  CcJsonDocument oDoc;
  CcJsonArray& rRootNode = oDoc.getJsonData().setJsonArray();

  for (CEntry& CEntry : m_oItems)
  {
    CcJsonNode oEntry(EJsonDataType::Object);
    oEntry.object().append(CcJsonNode("Name", CEntry.sName));
    oEntry.object().append(CcJsonNode("Link", CEntry.sLink));
    rRootNode.append(oEntry);
  }

  oData.write(oDoc.getDocument());
  return bSuccess;
}

void CcRestApiApplicationMenu::append(const CcString& sName, const CcString& sLink)
{
  CEntry oEntry;
  oEntry.sName = sName;
  oEntry.sLink = sLink;
  m_oItems.append(oEntry);
}
