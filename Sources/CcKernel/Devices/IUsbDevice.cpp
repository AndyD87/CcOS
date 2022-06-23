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
  for(CConfigDescriptor& oConfig : oConfigs)  
  {
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
  }
  return uiIndex;
}

IUsbDevice::CConfigDescriptor& IUsbDevice::CDeviceDescriptor::createConfig()
{
  oConfigs.append(CConfigDescriptor());
  return oConfigs.last();
}

IUsbDevice::SStringDescriptor& IUsbDevice::CDeviceDescriptor::createString(const CcString& sValue)
{
  uint8 uiLength = (sizeof(SStringDescriptor) - 3) + sValue.length();
  CCNEWARRAYTYPE(pBuffer, uint8, uiLength);
  SStringDescriptor* pCasted = reinterpret_cast<SStringDescriptor*>(pBuffer);
  pCasted->uiDescriptorType = 0x03;
  pCasted->uiLength = uiLength;
  CcStatic::memcpy(&pCasted->pString, sValue.getCharString(), sValue.length());
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

IUsbDevice::SInterfaceDescriptor* IUsbDevice::CConfigDescriptor::createInterface()
{
  uint32 uiCurrentOffset = m_oBuffer.size();
  m_oBuffer.resize(m_oBuffer.size() + sizeof(SInterfaceDescriptor));
  IUsbDevice::SInterfaceDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SInterfaceDescriptor>(uiCurrentOffset);
  pInterface->init();
  m_oInterfaces.append(uiCurrentOffset);
  getConfig()->uiTotalLength = static_cast<uint16_t>(m_oBuffer.size());
  return pInterface;
}

IUsbDevice::SFunctionalDescriptor* IUsbDevice::CConfigDescriptor::createFunctional(uint8 uiSize)
{
  uint32 uiCurrentOffset = m_oBuffer.size();
  m_oBuffer.resize(m_oBuffer.size() + uiSize);
  IUsbDevice::SFunctionalDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(uiCurrentOffset);
  pInterface->init(uiSize);
  m_oFunctions.append(uiCurrentOffset);
  getConfig()->uiTotalLength = static_cast<uint16>(m_oBuffer.size());
  return pInterface;
}

void IUsbDevice::CConfigDescriptor::setupInterfaces()
{
  for(size_t i = 0; i < getEndpointCount(); i++)
  {
    getEndpointInfo(i).pDescriptor = getEndpoint(i);
  }
}

IUsbDevice::SEndpointDescriptor* IUsbDevice::CConfigDescriptor::createEndpoint(
          uint8 uiEndpointAddress, 
          uint8 uiAttributes, 
          uint16 wMaxPacketSize, 
          uint8 uInterval,
          CcEvent oOnChange
)
{
  uint32 uiCurrentOffset = m_oBuffer.size();
  m_oBuffer.resize(m_oBuffer.size() + sizeof(SEndpointDescriptor));
  IUsbDevice::SEndpointDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SEndpointDescriptor>(uiCurrentOffset);
  pInterface->init();
  pInterface->uiEndpointAddress = uiEndpointAddress;
  pInterface->uiAttributes      = uiAttributes; 
  pInterface->wMaxPacketSize    = wMaxPacketSize;
  pInterface->uInterval         = uInterval;

  m_oEndPoints.append(uiCurrentOffset);
  m_oEndPointConfigs.appendDefault();
  m_oEndPointConfigs.last().oInBuffer.resize(wMaxPacketSize);
  m_oEndPointConfigs.last().oInBuffer.memset(0);
  m_oEndPointConfigs.last().oOnChange = oOnChange;
  getConfig()->uiTotalLength = static_cast<uint16>(m_oBuffer.size());
  return pInterface;
}

IUsbDevice::SConfigDescriptor* IUsbDevice::CConfigDescriptor::getConfig()
{
  return m_oBuffer.cast<IUsbDevice::SConfigDescriptor>(0);
}

IUsbDevice::SInterfaceDescriptor* IUsbDevice::CConfigDescriptor::getInterface(size_t uiIndex)
{
  return m_oBuffer.cast<IUsbDevice::SInterfaceDescriptor>(m_oEndPoints[uiIndex]);
}

IUsbDevice::SEndpointDescriptor* IUsbDevice::CConfigDescriptor::getEndpoint(size_t uiIndex)
{
  return m_oBuffer.cast<IUsbDevice::SEndpointDescriptor>(m_oEndPoints[uiIndex]);
}

IUsbDevice::CConfigDescriptor::CEndpointInfo& IUsbDevice::CConfigDescriptor::getEndpointInfo(size_t uiIndex)
{
  return m_oEndPointConfigs[uiIndex];
}

IUsbDevice::SFunctionalDescriptor* IUsbDevice::CConfigDescriptor::getFunction(size_t uiIndex)
{
  return m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(m_oEndPoints[uiIndex]);
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
