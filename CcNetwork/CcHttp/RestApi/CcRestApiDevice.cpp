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
 * @brief     Implementation of Class CcRestApiDevice
 */
#include "CcRestApiDevice.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiDevices.h"

CcRestApiDevice::CcRestApiDevice(CcRestApiDevices *pParent, const CcDeviceHandle& oDeviceHandle) :
  IRestApi(nullptr, CcString::fromNumber(oDeviceHandle.getId())),
  m_oDevice(oDeviceHandle)
{
  if(pParent != nullptr)
  {
    pParent->appendProvider(this);
  }
}

CcRestApiDevice::~CcRestApiDevice()
{
}

bool CcRestApiDevice::get(CcHttpWorkData& oData)
{
  switch(m_oDevice.getType())
  {
    case EDeviceType::GPIOPin:
      return getGpioDeviceInfo(oData);
    default:
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
      oData.sendHeader();
  }
  return false;
}

CcJsonData CcRestApiDevice::getInfo()
{
  CcJsonData oInfo(EJsonDataType::Object);
  oInfo.object().append(CcJsonData("Id", m_oDevice.getId()));
  oInfo.object().append(CcJsonData("Type", IDevice::getString(m_oDevice.getType())));
  return oInfo;
}

bool CcRestApiDevice::getGpioDeviceInfo(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  CcJsonDocument oDoc(getInfo());
  CcJsonObject& rRootNode = oDoc.getJsonData().setJsonObject();

  rRootNode.append(CcJsonData("Test", "Value"));

  oData.writeChunked(oDoc.getDocument());
  return bSuccess;
}
