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
 * @brief     Implementation of Class CcRestApiSystem
 */
#include "CcRestApiSystem.h"
#include "CcHttpWorkData.h"
#include "Json/CcJsonDocument.h"
#include "Json/CcJsonArray.h"
#include "Json/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"

CcRestApiSystem::CcRestApiSystem(IRestApi *pParent) :
  IRestApi(pParent, "system"),
  m_oMemory(this),
  m_oDevices(this)
{
}

CcRestApiSystem::~CcRestApiSystem()
{
}

bool CcRestApiSystem::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.sendHeader();
  CcJsonDocument oDoc;
  CcJsonObject& rRootNode = oDoc.getJsonData().setJsonObject();
  rRootNode.append(CcJsonNode("Name", CcKernel::getSystem().getName()));
  rRootNode.append(CcJsonNode("Version", CcKernel::getSystem().getVersion().getVersionString()));
  rRootNode.append(CcJsonNode("#endif /*", CcKernel::getVersion().getVersionString()));
  oData.write(oDoc.getDocument());
  return bSuccess;
}
