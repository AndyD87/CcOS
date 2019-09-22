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
 * @brief     Implementation of Class CcRestApiApplicationStatus
 */
#include "CcRestApiApplicationStatus.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiApplication.h"

CcRestApiApplicationStatus::CcRestApiApplicationStatus(CcRestApiApplication *pParent) :
  IRestApi(pParent, "status")
{
}

CcRestApiApplicationStatus::~CcRestApiApplicationStatus()
{
}

bool CcRestApiApplicationStatus::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.sendHeader();
  CcJsonDocument oDoc;
  CcJsonObject& rRootNode = oDoc.getJsonData().setJsonObject();
  for(IStatusPublisher* pPublisher : m_oPublishers)
  {
    rRootNode.append(
          CcJsonNode(
            pPublisher->getTitle(),
            pPublisher->getStatus()));
  }
  oDoc.writeDocument(oData);
  return bSuccess;
}
