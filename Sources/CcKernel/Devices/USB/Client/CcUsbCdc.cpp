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
  onStart();
}

CcUsbCdc::~CcUsbCdc()
{
  onStop();
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
      m_oUsbDevice.getDevice()->write(0x81, static_cast<const uint8*>(pBuffer), CCMIN(UINT16_MAX, static_cast<uint16>(uSize)));
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
  IUsbDevice::CDeviceDescriptor& oDeviceDescriptor = m_oUsbDevice.getDevice()->getDeviceDescriptor();

  IUsbDevice::CConfigDescriptor& oConfig = oDeviceDescriptor.createConfig();
  oConfig.getConfig()->uiConfigurationValue = 1;
  oConfig.getConfig()->uiConfiguration = 0;
  oConfig.getConfig()->uiAttributes = 0xC0;
  oConfig.getConfig()->uiMaxPower   = 0x32; 

  IUsbDevice::SInterfaceDescriptor* oInterface0 = oConfig.createInterface(NewCcEvent(this, CcUsbCdc::onInterfaceRequest));
  oInterface0->uiAlternateSetting = 0;
  oInterface0->uiNumEndpoints = 1;
  oInterface0->uiInterfaceClass = 0x02;
  oInterface0->uiInterfaceSubClass = 0x02;
  oInterface0->uiInterfaceProtocol = 0x01;
  oInterface0->uiInterfaceStringIdx = 0;

  IUsbDevice::SFunctionalDescriptor* pFuntional0 = oConfig.createFunctional(5);
  pFuntional0->Data.Raw.uiRaw[0]= 0x00;
  pFuntional0->Data.Raw.uiRaw[1]= 0x10;
  pFuntional0->Data.Raw.uiRaw[2]= 0x01;
  
  IUsbDevice::SFunctionalDescriptor* pFuntional1 = oConfig.createFunctional(5);
  pFuntional1->Data.Raw.uiRaw[0]= 0x01;
  pFuntional1->Data.Raw.uiRaw[1]= 0x00;
  pFuntional1->Data.Raw.uiRaw[2]= 0x01;
  
  IUsbDevice::SFunctionalDescriptor* pFuntional2 = oConfig.createFunctional(4);
  pFuntional2->Data.Raw.uiRaw[0]= 0x02;
  pFuntional2->Data.Raw.uiRaw[1]= 0x02;

  IUsbDevice::SFunctionalDescriptor* pFuntional3 = oConfig.createFunctional(5);
  pFuntional3->Data.Raw.uiRaw[0]= 0x06;
  pFuntional3->Data.Raw.uiRaw[1]= 0x00;
  pFuntional3->Data.Raw.uiRaw[2]= 0x01;

  oConfig.createEndpoint(true, 0x03, 0x08, 0x10, NewCcEvent(this, CcUsbCdc::onControl));
  IUsbDevice::SInterfaceDescriptor* pInterface1 = oConfig.createInterface(NewCcEvent(this, CcUsbCdc::onInterfaceRequest));
  pInterface1->uiAlternateSetting = 0;
  pInterface1->uiNumEndpoints = 2;
  pInterface1->uiInterfaceClass = 0x0A;
  pInterface1->uiInterfaceSubClass = 0x00;
  pInterface1->uiInterfaceProtocol = 0x00;
  pInterface1->uiInterfaceStringIdx = 0;

  oConfig.createEndpoint(false, 0x02, 64, 0x00, NewCcEvent(this, CcUsbCdc::onDataIn));
  oConfig.createEndpoint(true, 0x02, 64, 0x00, NewCcEvent(this, CcUsbCdc::onDataOut));

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
        break;
      }
      else
      {
        m_oUsbDevice->ctrlSendStatus();
      }
  }
}

void CcUsbCdc::onControl(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{
  CCUNUSED(pRequest);
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

