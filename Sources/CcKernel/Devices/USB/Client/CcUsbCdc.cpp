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
 * @brief     Force implementations of interface only classes
 */

#include "Devices/USB/Client/CcUsbCdc.h"
#include "Devices/IUsbDevice.h"
#include "CcKernel.h"

CcUsbCdc::CcUsbCdc(const CcDeviceUsbDevice& oInterface) :
  m_oUsbDevice(oInterface)
{
  m_oClassData.resize(UsbCdcDataSize);
  m_oClassData.memset(0);
}

CcUsbCdc::~CcUsbCdc()
{
  close();
}

size_t CcUsbCdc::read(void* pBuffer, size_t uSize)
{
  size_t uiRead = SIZE_MAX;
  if( m_oUsbDevice.isValid())
  {
    if(m_oInputBuffer.size() > 0)
    {
      // store in buffer
      uiRead = m_oInputBuffer.read(pBuffer, uSize);
      if(uiRead <= m_oInputBuffer.size())
        m_oInputBuffer.remove(0, uiRead);
    }
    else
    {
      uiRead = 0;
    }
  }
  return uiRead;
}

size_t CcUsbCdc::write(const void* pBuffer, size_t uSize)
{
  size_t uiWritten = SIZE_MAX;
  if( m_oUsbDevice.isValid())
  {
    uiWritten = 0;
    while(uSize > 0)
    {
      m_oUsbDevice.getDevice()->write(m_uiOutEp, static_cast<const uint8*>(pBuffer), CCMIN(UINT16_MAX, static_cast<uint16>(uSize)));
      uiWritten += CCMIN(UINT16_MAX, uSize);
      uSize -= CCMIN(UINT16_MAX, uSize);
    }
  }
  return uiWritten;
}

CcStatus CcUsbCdc::open(EOpenFlags)
{
  return onStart();
}

CcStatus CcUsbCdc::close()
{
  return onStop();
}

CcStatus CcUsbCdc::cancel()
{
  return EStatus::NotSupported;
}

CcStatus CcUsbCdc::onStart()
{   
  CcStatus oState;
  IUsbDevice::CConfigDescriptor& oConfig = m_oUsbDevice.getDevice()->getDeviceDescriptor().getActiveConfig();

  IUsbDevice::SInterfaceAssociationDescriptor* oInterfaceAssoc = oConfig.createInterfaceAssociation(2);
  oInterfaceAssoc->uiDeviceClass = 2;
  oInterfaceAssoc->uiDeviceSubClass = 2;
  oInterfaceAssoc->uiDeviceProtocol = 1;

  IUsbDevice::SInterfaceDescriptor* oInterface0 = oConfig.createInterface(NewCcEvent(this, CcUsbCdc::onInterfaceRequest));
  oInterface0->uiAlternateSetting = 0;
  oInterface0->uiNumEndpoints = 1;
  oInterface0->uiInterfaceClass = 0x02;     // USB-Class Communication
  oInterface0->uiInterfaceSubClass = 0x02;  // Abstract Control Model
  oInterface0->uiInterfaceProtocol = 0x01;  // AT Command V.250 protocol
  oInterface0->uiInterfaceStringIdx = 0;

  IUsbDevice::SFunctionalDescriptor* pFuntional0 = oConfig.createFunctional(5);
  pFuntional0->Data.Raw.uiRaw[0]= 0x00;   // Header
  pFuntional0->Data.Raw.uiRaw[1]= 0x10;   // BCD CDC
  pFuntional0->Data.Raw.uiRaw[2]= 0x01;   // BCD CDC continue
  
  IUsbDevice::SFunctionalDescriptor* pFuntional1 = oConfig.createFunctional(5);
  pFuntional1->Data.Raw.uiRaw[0]= 0x01;                             // bDescriptorsubtype, CALL MANAGEMENT
  pFuntional1->Data.Raw.uiRaw[1]= 0x00;                             // bmCapabilities, DIY
  pFuntional1->Data.Raw.uiRaw[2]= oInterface0->uiInterfaceNumber+1; // DataInterface
  
  IUsbDevice::SFunctionalDescriptor* pFuntional2 = oConfig.createFunctional(4);
  pFuntional2->Data.Raw.uiRaw[0]= 0x02; // bDescriptorsubtype, ABSTRACT CONTROL MANAGEMENT
  pFuntional2->Data.Raw.uiRaw[1]= 0x00; // bmCapabilities: Supports subset of ACM commands

  IUsbDevice::SFunctionalDescriptor* pFuntional3 = oConfig.createFunctional(5);
  pFuntional3->Data.Raw.uiRaw[0]= 0x06;                             // bDescriptorsubtype, UNION
  pFuntional3->Data.Raw.uiRaw[1]= oInterface0->uiInterfaceNumber;   // bControlInterface - Interface
  pFuntional1->Data.Raw.uiRaw[2]= oInterface0->uiInterfaceNumber+1; // bSubordinateInterface0 - bControlInterface + 1

  oConfig.createEndpoint(true, 0x03, 0x08, 0x10, NewCcEvent(this, CcUsbCdc::onControl));

  IUsbDevice::SInterfaceDescriptor* pInterface1 = oConfig.createInterface(NewCcEvent(this, CcUsbCdc::onInterfaceRequest));
  pInterface1->uiAlternateSetting = 0;
  pInterface1->uiNumEndpoints = 2;
  pInterface1->uiInterfaceClass = 0x0A;
  pInterface1->uiInterfaceSubClass = 0x00;
  pInterface1->uiInterfaceProtocol = 0x00;
  pInterface1->uiInterfaceStringIdx = 0;

  oConfig.createEndpoint(true, 0x02, 64, 0x00, NewCcEvent(this, CcUsbCdc::onDataIn));
  IUsbDevice::SEndpointDescriptor* pEp = oConfig.createEndpoint(false, 0x02, 64, 0x00, NewCcEvent(this, CcUsbCdc::onDataOut));
  if(pEp)
  {
    m_uiOutEp = pEp->uiEndpointAddress;
  }

  return oState;
}

CcStatus CcUsbCdc::onStop()
{   
  CcStatus oState;
  if(m_oUsbDevice.isValid())
  {
    m_oUsbDevice->stop();
    m_oClassData.clear();
    m_oInputBuffer.clear();
  }
  else
  {
    oState = EStatus::InvalidHandle;
  }   
  return oState;
}

void CcUsbCdc::onInterfaceRequest(IUsbDevice::SRequest* pRequest)
{
  switch (pRequest->bmRequest & UsbRequestType_MASK)
  {
    case UsbRequestType_CLASS:
    case UsbRequestType_VENDOR:
    case UsbRequestType_STANDARD:
      if (pRequest->wLength != 0U)
      {
        if ((pRequest->bmRequest & 0x80U) != 0U)
        {
          uint16 uiLen = static_cast<uint16>(CCMIN(UsbCdcDataSize, m_oClassData.size()));
          m_oUsbDevice->write(0, m_oClassData.cast<uint8>(), uiLen);
        }
        else
        {
          m_uiCmd = pRequest->bRequest;

          uint16 uiLen = static_cast<uint16>(CCMIN(UsbCdcDataSize, m_oClassData.size()));
          m_oUsbDevice->read(0, m_oClassData.cast<uint8>(), uiLen);
        }
      }
      else
      {
        m_oUsbDevice->ctrlSendStatus();
      }
      break;
    default:
      IUsbDevice::debugMessage("CcUsbCdc::onInterfaceRequest");
  }
}

void CcUsbCdc::onControl(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{
  CCUNUSED(pRequest);
  IUsbDevice::debugMessage("CcUsbCdc::onControl");
}

void CcUsbCdc::onDataIn(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{
  while(pRequest->oBufferList.getChunkCount())
  {
    m_oInputBuffer.append(pRequest->oBufferList[0]);
    pRequest->oBufferList.removeChunk(0);
  }
}

void CcUsbCdc::onDataOut(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{
  CCUNUSED(pRequest);
}

