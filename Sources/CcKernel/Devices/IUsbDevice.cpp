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

IUsbDevice::CDeviceDescriptor::CDeviceDescriptor()
{
}

IUsbDevice::CDeviceDescriptor::~CDeviceDescriptor()
{

}

IUsbDevice::CConfigDescriptor& IUsbDevice::CDeviceDescriptor::createConfig()
{
  oConfigs.append(CConfigDescriptor());
  return oConfigs.last();
}

IUsbDevice::CStringDescriptor& IUsbDevice::CDeviceDescriptor::createString(const CcString& sValue)
{
  uint8 uiLength = (sizeof(SStringDescriptor) - 3) + sValue.length();
  CCNEWARRAYTYPE(pBuffer, uint8, uiLength);
  CStringDescriptor* pCasted = reinterpret_cast<CStringDescriptor*>(pBuffer);
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
  getConfig()->uiTotalLength = m_oBuffer.size();
  return pInterface;
}

IUsbDevice::SFunctionalDescriptor* IUsbDevice::CConfigDescriptor::createFunctional(uint8 uiSize)
{
  uint32 uiCurrentOffset = m_oBuffer.size();
  m_oBuffer.resize(m_oBuffer.size() + uiSize);
  IUsbDevice::SFunctionalDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(uiCurrentOffset);
  pInterface->init(uiSize);
  m_oFunctions.append(uiCurrentOffset);
  getConfig()->uiTotalLength = m_oBuffer.size();
  return pInterface;
}

IUsbDevice::SEndpointDescriptor* IUsbDevice::CConfigDescriptor::createEndpoint()
{
  uint32 uiCurrentOffset = m_oBuffer.size();
  m_oBuffer.resize(m_oBuffer.size() + sizeof(SEndpointDescriptor));
  IUsbDevice::SEndpointDescriptor* pInterface = m_oBuffer.cast<IUsbDevice::SEndpointDescriptor>(uiCurrentOffset);
  pInterface->init();
  m_oEndPoints.append(uiCurrentOffset);
  getConfig()->uiTotalLength = m_oBuffer.size();
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

IUsbDevice::SFunctionalDescriptor* IUsbDevice::CConfigDescriptor::getFunction(size_t uiIndex)
{
  return m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(m_oEndPoints[uiIndex]);
}

IUsbDevice::CStringDescriptor::CStringDescriptor()
{

}

IUsbDevice::CStringDescriptor::~CStringDescriptor()
{

}

IUsbDevice::IUsbDevice()
{
}

IUsbDevice::~IUsbDevice()
{
}
