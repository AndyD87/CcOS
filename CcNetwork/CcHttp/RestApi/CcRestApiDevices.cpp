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

bool CcRestApiDevices::execPath(CcStringList& oPath, CcHttpWorkData& oData)
{
  bool bSuccess = false;
  uint64 uiId = 0;
  if (oPath.size() > 0)
  {
    EDeviceType eDeviceType = CcDevice::getTypeFromString(oPath[0], &bSuccess);
    if (bSuccess)
    {
      oPath.remove(0);
      if (oPath.size() > 0)
      {
        bSuccess = IRestApi::execPath(oPath, oData);
      }
      else if (oData.getRequestType() == EHttpRequestType::Get)
      {
        bSuccess = getDeviceList(oData, eDeviceType);
      }
      else
      {
        oData.getResponse().setError(CcHttpGlobals::EError::ErrorInvalidParamter);
      }
    }
    else if((uiId = oPath[0].toUint64(&bSuccess)) > 0 && bSuccess)
    {
      bSuccess = IRestApi::execPath(oPath, oData);
    }
    else
    {
#ifdef DEBUG
      if (oPath.size() != 0 && oPath[0] == "list")
      {
        sendList(oData);
      }
      else
      {
#endif // DEBUG
        sendMethodNotFound(oData);
#ifdef DEBUG
      }
#endif // DEBUG
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorInvalidParamter);
    }
  }
  else
  {
    bSuccess = IRestApi::execPath(oPath, oData);
  }
  return bSuccess;
}

bool CcRestApiDevices::get(CcHttpWorkData& oData)
{
  return getDeviceList(oData, EDeviceType::All);
}

void CcRestApiDevices::appendProvider(CcRestApiDevice* pDeviceApi)
{
  IRestApi::appendProvider(pDeviceApi);
}

bool CcRestApiDevices::getDeviceList(CcHttpWorkData& oData, EDeviceType eFilterType)
{
  CCUNUSED(oData);
  bool bSuccess = true;
  if (bSuccess)
  {
    oData.sendHeader();
    CcJsonDocument oDoc;
    CcJsonObject& rRootNode = oDoc.getJsonData().setJsonObject();
    CcJsonNode oDevices(EJsonDataType::Array);
    oDevices.setName("Devices");
    for (IRestApi* piDevice : getChilds())
    {
      CcRestApiDevice* pDevice = static_cast<CcRestApiDevice*>(piDevice);
      if (eFilterType == EDeviceType::All ||
          eFilterType == pDevice->getDevice().getType())
      {
        oDevices.array().append(pDevice->getGpioPinInfoNode());
      }
    }
    rRootNode.append(oDevices);
    oData.write(oDoc.getDocument());
  }
  return bSuccess;
}
