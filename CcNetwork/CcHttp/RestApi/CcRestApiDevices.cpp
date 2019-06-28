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
 * @brief     Implementation of Class CcRestApiDevices
 */
#include "CcRestApiDevices.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiDevice.h"

CcRestApiDevices::CcRestApiDevices(IRestApi *pParent) :
  IRestApi(pParent, "devices")
{
}

CcRestApiDevices::~CcRestApiDevices()
{
}

bool CcRestApiDevices::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  CcJsonDocument oDoc;
  CcJsonObject& rRootNode = oDoc.getJsonData().setJsonObject();
  CcJsonData oDevices(EJsonDataType::Array);
  oDevices.setName("Devices");
  for(IRestApi* piDevice : getChilds())
  {
    CcRestApiDevice* pDevice = static_cast<CcRestApiDevice*>(piDevice);
    oDevices.array().append(pDevice->getInfo());
  }
  rRootNode.append(oDevices);
  oData.writeChunked(oDoc.getDocument());
  return bSuccess;
}

void CcRestApiDevices::appendProvider(CcRestApiDevice* pDeviceApi)
{
  IRestApi::appendProvider(pDeviceApi);
}