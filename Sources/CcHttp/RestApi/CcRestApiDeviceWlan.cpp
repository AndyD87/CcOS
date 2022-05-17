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
 * @brief     Implementation of Class CcRestApiDeviceWlan
 */
#include "CcRestApiDeviceWlan.h"
#include "CcHttpWorkData.h"
#include "Json/CcJsonDocument.h"
#include "Json/CcJsonArray.h"
#include "Json/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiDevice.h"
#include "CcRestApiDevices.h"
#include "Devices/IGpioPin.h"
#include "Devices/IGpioPort.h"

CcRestApiDeviceWlan::CcRestApiDeviceWlan(CcRestApiDevices *pParent, const CcDevice& oDeviceHandle) :
  CcRestApiDevice(pParent, m_oDevice),
  m_oDevice(oDeviceHandle)
{
  if(pParent != nullptr)
  {
    pParent->appendProvider(this);
  }
}

CcRestApiDeviceWlan::~CcRestApiDeviceWlan()
{
}

bool CcRestApiDeviceWlan::get(CcHttpWorkData& oData)
{
  switch(m_oDevice.getType())
  {
    case EDeviceType::Wlan:
      return getWlanInfo(oData);
    default:
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
      oData.sendHeader();
      break;
  }
  return false;
}

bool CcRestApiDeviceWlan::post(CcHttpWorkData& oData)
{
  switch (m_oDevice.getType())
  {
    case EDeviceType::Wlan:
      return postWlanInfo(oData);
    default:
      oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
      oData.sendHeader();
      break;
  }
  return false;
}

bool CcRestApiDeviceWlan::getWlanInfo(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.sendHeader();
  CcJsonDocument oDoc;
  oDoc.getJsonNode() = getDeviceNode();
  oData.write(oDoc.getDocument());
  return bSuccess;
}

CcJsonNode CcRestApiDeviceWlan::getDeviceNode()
{
  CcJsonNode oNode;
  oNode.setJsonObject();
  oNode.object().append(CcJsonNode("Id", m_oDevice.getId()));
  oNode.object().append(CcJsonNode("Type", m_oDevice.getTypeString()));
  
  oNode.object().append(CcJsonNode("Name", getName()));
  CcJsonNode oMethods(EJsonDataType::Array);
  oMethods.setName("methods");
  oNode.object().append(oMethods);
  return oNode;
}

bool CcRestApiDeviceWlan::postWlanInfo(CcHttpWorkData& oData)
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
    CCDEBUG(oMap["method"]);
    if (sMethod.length() > 0)
    {
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
