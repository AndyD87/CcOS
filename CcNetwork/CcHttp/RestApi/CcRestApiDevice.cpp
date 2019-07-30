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
#include "Devices/IGpioPin.h"
#include "Devices/IGpioPort.h"

CcRestApiDevice::CcRestApiDevice(CcRestApiDevices *pParent, const CcDeviceHandle& oDeviceHandle) :
  IRestApi(nullptr, CcString::fromNumber(oDeviceHandle.getId())),
  m_oDevice(oDeviceHandle)
{
  if(pParent != nullptr)
  {
    pParent->appendProvider(this);
  }
}

CcRestApiDevice::CcRestApiDevice(CcRestApiDevices* pParent, uint8 uiPortNr, uint8 uiPinNr) :
  IRestApi(nullptr, "invalid")
{
  if (pParent != nullptr)
  {
    pParent->appendProvider(this);
  }
  CcDeviceHandle pPort = CcKernel::getDevice(EDeviceType::GpioPort, uiPortNr);
  if (pPort.isValid())
  {
    CcDeviceHandle oHandle(pPort.cast<IGpioPort>()->getPin(uiPinNr), EDeviceType::GpioPin);
    if (oHandle.isValid())
    {
      m_oDevice = oHandle;
      setPath(CcString::fromNumber(m_oDevice.getId()));
    }
  }
}

CcRestApiDevice::~CcRestApiDevice()
{
}

bool CcRestApiDevice::get(CcHttpWorkData& oData)
{
  switch(m_oDevice.getType())
  {
    case EDeviceType::GpioPin:
      return getGpioDeviceInfo(oData);
    default:
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
      oData.sendHeader();
  }
  return false;
}

bool CcRestApiDevice::post(CcHttpWorkData& oData)
{
  switch (m_oDevice.getType())
  {
    case EDeviceType::GpioPin:
      return postGpioDeviceInfo(oData); 
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
  oInfo.object().append(CcJsonData("Type", m_oDevice.getTypeString()));
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
  CcHandle<IGpioPin> hPin = m_oDevice.cast<IGpioPin>();

  rRootNode.append(CcJsonData("Name", getName()));
  rRootNode.append(CcJsonData("Value", static_cast<uint8>(hPin->getValue())));
  rRootNode.append(CcJsonData("Direction", static_cast<uint8>(hPin->getDirection())));
  CcJsonData oMethods(EJsonDataType::Array);
  oMethods.setName("methods");
  oMethods.array().add(CcJsonData("", "rename"));
  oMethods.array().add(CcJsonData("", "toggle"));
  rRootNode.append(oMethods);

  oData.writeChunked(oDoc.getDocument());
  return bSuccess;
}

bool CcRestApiDevice::postGpioDeviceInfo(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  size_t uiAllContent = oData.readAllContent();
  if (uiAllContent > 0)
  {
    CcHandle<IGpioPin> hPin = m_oDevice.cast<IGpioPin>();
    CcString sData = oData.getRequest().getContent();
    CcStringMap oMap = oData.parseQueryLine(sData);
    CcString sMethod = oMap["method"];
    if (sMethod.length() > 0)
    {
      if (sMethod == "rename")
      {
        CcString sName = oMap["name"];
        if (sName == "rename")
        {
          setName(sName);
        }
      }
      else if (sMethod == "toggle")
      {
        hPin->toggle();
      }
    }
    else
    {
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
    }
  }
  else
  {
    oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
  }
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  return bSuccess;
}
