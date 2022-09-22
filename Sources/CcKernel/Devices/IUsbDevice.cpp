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
 * @brief     Create Access to an IUsbDevice
 */

#include "IUsbDevice.h"
#include "CcStatic.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "CcWString.h"

IUsbDevice::CDeviceDescriptor::CDeviceDescriptor()
{
}

IUsbDevice::CDeviceDescriptor::~CDeviceDescriptor()
{
  for(SStringDescriptor* oString : oStrings)  
  {
    CCDELETE(oString);
  }
}

uint8 IUsbDevice::CDeviceDescriptor::findEndpoint(uint8 uiEndpoint, CConfigDescriptor** pConfig)
{ 
  uint8 uiIndex = UINT8_MAX;
  CConfigDescriptor& oConfig = getActiveConfig();
  for(uint8 uiIdx=0; uiIdx<oConfig.getEndpointCount(); uiIdx++)  
  {
    if(oConfig.getEndpoint(uiIdx)->uiEndpointAddress == uiEndpoint) 
    {
      uiIndex = uiIdx;
      if(pConfig != nullptr)
      {
        *pConfig = &oConfig;
      }
    }
  }
  return uiIndex;
}

uint8 IUsbDevice::CDeviceDescriptor::findInterface(uint8 uiInterfaceId, CConfigDescriptor** pConfig)
{ 
  uint8 uiIndex = UINT8_MAX;
  CConfigDescriptor& oConfig = getActiveConfig();
  for(uint8 uiIdx=0; uiIdx<oConfig.getInterfaceCount(); uiIdx++)  
  {
    if(oConfig.getInterface(uiIdx)->uiInterfaceNumber == uiInterfaceId) 
    {
      uiIndex = uiIdx;
      if(pConfig != nullptr)
      {
        *pConfig = &oConfig;
      }
    }
  }
  return uiIndex;
}

IUsbDevice::CConfigDescriptor& IUsbDevice::CDeviceDescriptor::createConfig()
{
  oConfigs.append(CConfigDescriptor());
  uiActiveConfig = oConfigs.size()-1;
  uiNumConfigurations++;
  return oConfigs.last();
}

IUsbDevice::SStringDescriptor& IUsbDevice::CDeviceDescriptor::createString(const CcString& sValue)
{
  CcWString oWString(sValue);
  uint8 uiStringLength = static_cast<uint8>((sizeof(SStringDescriptor) - (sizeof(SStringDescriptor().pString))) + (oWString.length()*sizeof(uint16)));
  CCNEWARRAYTYPE(pBuffer, uint8, uiStringLength);
  SStringDescriptor* pCasted = reinterpret_cast<SStringDescriptor*>(pBuffer);
  pCasted->uiDescriptorType = 0x03;
  pCasted->uiLength = uiStringLength;
  oWString.toUtf16(&pCasted->pString[0], oWString.length());
  oStrings.append(pCasted);
  return *pCasted;
}

IUsbDevice::CConfigDescriptor::CConfigDescriptor()
{
  m_oBuffer.resize(sizeof(SConfigDescriptor));
  getConfig()->init();
}

IUsbDevice::CConfigDescriptor::~CConfigDescriptor()
{
  m_oBuffer.clear();
}

void IUsbDevice::CConfigDescriptor::CEndpointInfo::reset()
{
  oBufferList.clear();
  eState = EEnpointState::Idle;
}

IUsbDevice::SInterfaceAssociationDescriptor* IUsbDevice::CConfigDescriptor::createInterfaceAssociation(uint8 uiInterfaceCount)
{
  uint32 uiCurrentOffset = static_cast<uint32>(m_oBuffer.size());
  m_oBuffer.resize(m_oBuffer.size() + sizeof(SInterfaceAssociationDescriptor));
  IUsbDevice::SInterfaceAssociationDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SInterfaceAssociationDescriptor>(uiCurrentOffset);
  pInterface->init();
  pInterface->uiFirstInterface =  getNextInterfaceId();
  pInterface->uiInterfaceCount = uiInterfaceCount;
  getConfig()->uiTotalLength = static_cast<uint16_t>(m_oBuffer.size());  
  
  setupInterfaces();

  return m_oBuffer.cast<IUsbDevice::SInterfaceAssociationDescriptor>(uiCurrentOffset);
}

IUsbDevice::SInterfaceDescriptor* IUsbDevice::CConfigDescriptor::createInterface(const CcEvent& oOnRequest, const CcEvent& oOnReadDone)
{
  uint8 uiNextInterfaceId = getNextInterfaceId();
  uint32 uiCurrentOffset = static_cast<uint32>(m_oBuffer.size());
  m_oBuffer.resize(m_oBuffer.size() + sizeof(SInterfaceDescriptor));
  IUsbDevice::SInterfaceDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SInterfaceDescriptor>(uiCurrentOffset);
  pInterface->init();
  pInterface->uiInterfaceNumber = uiNextInterfaceId;
  m_oInterfaces.append(uiCurrentOffset);
  getConfig()->uiTotalLength = static_cast<uint16_t>(m_oBuffer.size());  
  
  CInterfaceInfo oInterface;
  oInterface.oOnReadDone = oOnReadDone;
  oInterface.oOnRequest = oOnRequest;
  oInterface.pDescriptor = pInterface;
  m_oInterfaceConfigs.append(oInterface);

  getConfig()->uiNumInterfaces++;
  setupInterfaces();

  return m_oBuffer.cast<IUsbDevice::SInterfaceDescriptor>(uiCurrentOffset);
}

IUsbDevice::SFunctionalDescriptor* IUsbDevice::CConfigDescriptor::createFunctional(uint8 uiSize)
{
  uint32 uiCurrentOffset = static_cast<uint32>(m_oBuffer.size());
  m_oBuffer.resize(m_oBuffer.size() + uiSize);
  IUsbDevice::SFunctionalDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(uiCurrentOffset);
  pInterface->init(uiSize);
  m_oFunctions.append(uiCurrentOffset);
  getConfig()->uiTotalLength = static_cast<uint16>(m_oBuffer.size());

  setupInterfaces();


  return m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(uiCurrentOffset);
}

void IUsbDevice::CConfigDescriptor::setupInterfaces()
{
  for(size_t i = 0; i < getEndpointCount(); i++)
  {
    getEndpointInfo(i).pDescriptor = getEndpoint(i);
  }
  for(size_t i = 0; i < getInterfaceCount(); i++)
  {
    getInterfaceInfo(i).pDescriptor = getInterface(i);
  }
}

uint8 IUsbDevice::CConfigDescriptor::getNextInterfaceId()
{
  return static_cast<uint8>(getInterfaceCount());
}

uint8 IUsbDevice::CConfigDescriptor::getNextEndpointId()
{
  uint8 uiId  = 1;
  for(; uiId < 16; uiId++)
  {
    bool bFound = false;
    for(CEndpointInfo& oEpInfo : m_oEndPointConfigs)
    {
      if(oEpInfo.pDescriptor->uiEndpointAddress == uiId ||
         oEpInfo.pDescriptor->uiEndpointAddress == (uiId | 0x80))
      {
        bFound = true;
        break;
      }
    }
    if(bFound == false)
    {
      break;
    }
  }
  return uiId;
}

uint8 IUsbDevice::CConfigDescriptor::getLastEndpointId()
{
  return getEndpoint(getEndpointCount()-1)->uiEndpointAddress & 0x7f;
}

IUsbDevice::SEndpointDescriptor* IUsbDevice::CConfigDescriptor::createEndpoint(
          bool bInOut, 
          uint8 uiAttributes, 
          uint16 wMaxPacketSize, 
          uint8 uInterval,
          const CcEvent& oOnChange,
          bool bCreateNewId
)
{
  uint8 uiNextEndpointId = bInOut ? 0 : 0x80;
  if(bCreateNewId)
  {
    uiNextEndpointId |= getNextEndpointId();
  }
  else
  {
    uiNextEndpointId |= getLastEndpointId();
  }
  uint32 uiCurrentOffset = static_cast<uint32>(m_oBuffer.size());
  m_oBuffer.resize(m_oBuffer.size() + sizeof(SEndpointDescriptor));
  IUsbDevice::SEndpointDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SEndpointDescriptor>(uiCurrentOffset);
  pInterface->init();
  pInterface->uiEndpointAddress = uiNextEndpointId;
  pInterface->uiAttributes      = uiAttributes; 
  pInterface->wMaxPacketSize    = wMaxPacketSize;
  pInterface->uInterval         = uInterval;

  m_oEndPoints.append(uiCurrentOffset);
  m_oEndPointConfigs.appendDefault();
  m_oEndPointConfigs.last().oInBuffer.resize(wMaxPacketSize);
  m_oEndPointConfigs.last().oInBuffer.memset(0);
  m_oEndPointConfigs.last().oOnChange = oOnChange;
  getConfig()->uiTotalLength = static_cast<uint16>(m_oBuffer.size());
  
  setupInterfaces();

  return m_oBuffer.cast<IUsbDevice::SEndpointDescriptor>(uiCurrentOffset);
}

IUsbDevice::IUsbDevice()
{
}

IUsbDevice::~IUsbDevice()
{
}

void IUsbDevice::sendError()
{
  stallEp(0);
  stallEp(0x80);
}

void IUsbDevice::debugMessage(const char* pMessage)
{
  CcConsole::writeLine(pMessage);
}
