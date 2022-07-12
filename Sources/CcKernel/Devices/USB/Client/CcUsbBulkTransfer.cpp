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

#include "Devices/USB/Client/CcUsbBulkTransfer.h"
#include "Devices/IUsbDevice.h"
#include "CcKernel.h"

CcUsbBulkTransfer::CcUsbBulkTransfer(const CcDeviceUsbDevice& oInterface) :
  m_oUsbDevice(oInterface)
{
}

CcUsbBulkTransfer::~CcUsbBulkTransfer()
{
  close();
}

size_t CcUsbBulkTransfer::read(void* pBuffer, size_t uSize)
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

size_t CcUsbBulkTransfer::write(const void* pBuffer, size_t uSize)
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

CcStatus CcUsbBulkTransfer::open(EOpenFlags)
{
  return onStart();
}

CcStatus CcUsbBulkTransfer::close()
{
  return onStop();
}

CcStatus CcUsbBulkTransfer::cancel()
{
  return EStatus::NotSupported;
}

CcStatus CcUsbBulkTransfer::onStart()
{   
  CcStatus oState;
  IUsbDevice::CConfigDescriptor& oConfig = m_oUsbDevice.getDevice()->getDeviceDescriptor().getActiveConfig();

  IUsbDevice::SInterfaceAssociationDescriptor* oInterfaceAssoc = oConfig.createInterfaceAssociation(1);
  oInterfaceAssoc->uiDeviceClass = 0xff;
  oInterfaceAssoc->uiDeviceSubClass = 0;
  oInterfaceAssoc->uiDeviceProtocol = 0;

  IUsbDevice::SInterfaceDescriptor* pInterface1 = oConfig.createInterface(NewCcEvent(this, CcUsbBulkTransfer::onInterfaceRequest), NewCcEvent(this, CcUsbBulkTransfer::onInterfaceReadDone));
  pInterface1->uiAlternateSetting = 0;
  pInterface1->uiNumEndpoints = 2;
  pInterface1->uiInterfaceClass = 0xff; // Custom
  pInterface1->uiInterfaceSubClass = 0x00;
  pInterface1->uiInterfaceProtocol = 0x00;
  pInterface1->uiInterfaceStringIdx = 0;

  oConfig.createEndpoint(true, 0x02, 64, 0x00, NewCcEvent(this, CcUsbBulkTransfer::onDataIn));
  IUsbDevice::SEndpointDescriptor* pEp = oConfig.createEndpoint(false, 0x02, 64, 0x00, NewCcEvent(this, CcUsbBulkTransfer::onDataOut), false);
  if(pEp)
  {
    m_uiOutEp = pEp->uiEndpointAddress;
  }

  return oState;
}

CcStatus CcUsbBulkTransfer::onStop()
{   
  CcStatus oState;
  if(m_oUsbDevice.isValid())
  {
    m_oUsbDevice->stop();
    m_oInputBuffer.clear();
  }
  else
  {
    oState = EStatus::InvalidHandle;
  }   
  return oState;
}

void CcUsbBulkTransfer::onInterfaceRequest(IUsbDevice::SRequest* pRequest)
{
  switch (pRequest->bmRequest & UsbRequestType_MASK)
  {
    case UsbRequestType_CLASS:
      if (pRequest->wLength != 0U)
      {
      }
      else
      {
        m_oUsbDevice->ctrlSendStatus();
      }
      break;
    case UsbRequestType_VENDOR:
    case UsbRequestType_STANDARD:
    default:
      IUsbDevice::debugMessage("CcUsbBulkTransfer::onInterfaceRequest");
  }
}

void CcUsbBulkTransfer::onInterfaceReadDone(IUsbDevice::SRequest* pRequest)
{
  switch (pRequest->bmRequest & UsbRequestType_MASK)
  {
    case UsbRequestType_CLASS:
    case UsbRequestType_VENDOR:
    case UsbRequestType_STANDARD:
      IUsbDevice::debugMessage("CcUsbBulkTransfer::onInterfaceReadDone known");
      break;
    default:
      IUsbDevice::debugMessage("CcUsbBulkTransfer::onInterfaceReadDone");
  }
}

void CcUsbBulkTransfer::onDataIn(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{                                                                 
  while(pRequest->oBufferList.getChunkCount())
  {
    m_oInputBuffer.append(pRequest->oBufferList[0]);
    pRequest->oBufferList.removeChunk(0);
  }
}

void CcUsbBulkTransfer::onDataOut(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{
  CCUNUSED(pRequest);
}

