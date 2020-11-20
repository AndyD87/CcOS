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
 * @brief     Implementation of Class CcRestApiDeviceGpioPin
 */
#include "CcRestApiDeviceGpioPin.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiDevice.h"
#include "CcRestApiDevices.h"
#include "Devices/IGpioPin.h"
#include "Devices/IGpioPort.h"

CcRestApiDeviceGpioPin::CcRestApiDeviceGpioPin(CcRestApiDevices *pParent, const CcDevice& oDeviceHandle) :
  CcRestApiDevice(pParent, m_oDevice),
  m_oDevice(oDeviceHandle)
{
  if(pParent != nullptr)
  {
    pParent->appendProvider(this);
  }
}

CcRestApiDeviceGpioPin::CcRestApiDeviceGpioPin(CcRestApiDevices* pParent, uint8 uiPortNr, uint8 uiPinNr) :
  CcRestApiDevice(pParent, m_oDevice),
  m_oDevice(CcDevice(nullptr, EDeviceType::All))
{
  if (pParent != nullptr)
  {
    pParent->appendProvider(this);
  }
  CcDevice pPort = CcKernel::getDevice(EDeviceType::GpioPort, uiPortNr);
  if (pPort.isValid())
  {
    CcDevice oHandle(pPort.getDevice<IGpioPort>()->getPin(uiPinNr), EDeviceType::GpioPin);
    if (oHandle.isValid())
    {
      m_oDevice = oHandle;
      setPath(CcString::fromNumber(m_oDevice.getId()));
    }
  }
}

CcRestApiDeviceGpioPin::~CcRestApiDeviceGpioPin()
{
}

bool CcRestApiDeviceGpioPin::get(CcHttpWorkData& oData)
{
  return getGpioPinInfo(oData);
}

bool CcRestApiDeviceGpioPin::post(CcHttpWorkData& oData)
{
  return postGpioDeviceInfo(oData);
}

bool CcRestApiDeviceGpioPin::getGpioPinInfo(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.sendHeader();
  CcJsonDocument oDoc;
  oDoc.getJsonNode() = getDeviceNode();
  oData.write(oDoc.getDocument());
  return bSuccess;
}

CcJsonNode CcRestApiDeviceGpioPin::getDeviceNode()
{
  CcJsonNode oNode;
  oNode.setJsonObject();
  oNode.object().append(CcJsonNode("Id", m_oDevice.getId()));
  oNode.object().append(CcJsonNode("Type", m_oDevice.getTypeString()));
  
  oNode.object().append(CcJsonNode("Name", getName()));
  oNode.object().append(CcJsonNode("Value", static_cast<uint8>(m_oDevice.getValue())));
  oNode.object().append(CcJsonNode("Direction", static_cast<uint8>(m_oDevice.getDirection())));
  CcJsonNode oMethods(EJsonDataType::Array);
  oMethods.setName("methods");
  oMethods.array().add(CcJsonNode("", "rename"));
  oMethods.array().add(CcJsonNode("", "toggle"));
  oNode.object().append(oMethods);
  return oNode;
}

bool CcRestApiDeviceGpioPin::postGpioDeviceInfo(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  size_t uiAllContent = oData.readAllContent();
  if (uiAllContent > 0)
  {
    CcString sData = oData.getRequest().getContent();
    CcStringMap oMap = oData.parseQueryLine(sData);
    CcString sMethod = oMap["method"];
    CCDEBUG(oMap["method"]);
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
        CCDEBUG("toogle");
        m_oDevice.toggle();
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
  oData.sendHeader();
  return bSuccess;
}
