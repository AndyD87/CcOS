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

CcUsbCdc::CcUsbCdc(const CcDeviceUsb& oInterface) :
  m_oInterface(oInterface)
{
  m_oClassData.resize(UsbCdcDataSize);
  m_oClassData.memset(0);
}

CcUsbCdc::~CcUsbCdc()
{

}

CcStatus CcUsbCdc::onState(EState eState)
{   
  CcStatus oState;
  switch (eState)
  {
    case EState::Start:
      oState = onStart();
      break;
    case EState::Stop:
      oState = onStop();
      break;
    default:
      break;
  }
  return oState;
}

size_t CcUsbCdc::read(void* pBuffer, size_t uSize)
{
  size_t uiRead = SIZE_MAX;
  if( m_pUsbDevice &&
      m_pUsbDevice->isStarted())
  {
    uiRead = m_oInputBuffer.read(pBuffer, uSize);
  }
  return uiRead;
}

size_t CcUsbCdc::write(const void* pBuffer, size_t uSize)
{
  size_t uiWritten = SIZE_MAX;
  if( m_pUsbDevice &&
      m_pUsbDevice->isStarted())
  {
    uiWritten = 0;
    while(uSize > 0)
    {
      m_pUsbDevice->write(0x81, static_cast<const uint8*>(pBuffer), CCMIN(UINT16_MAX, uSize));
      uiWritten += CCMIN(UINT16_MAX, uSize);
      uSize -= CCMIN(UINT16_MAX, uSize);
    }
  }
  return uiWritten;
}

CcStatus CcUsbCdc::open(EOpenFlags)
{
  return start();
}

CcStatus CcUsbCdc::close()
{
  return stop();
}

CcStatus CcUsbCdc::cancel()
{
  return EStatus::NotSupported;
}

CcStatus CcUsbCdc::onStart()
{   
  CcStatus oState;
  IUsbDevice::CDeviceDescriptor oDeviceDescriptor;
  oDeviceDescriptor.uiBcd                   = 0x0200;
  oDeviceDescriptor.uiDeviceClass           = 2;
  oDeviceDescriptor.uiDeviceSubClass        = 2;
  oDeviceDescriptor.uiDeviceProtocol        = 0;
  oDeviceDescriptor.uiMaxPacketSize         = 64;
  oDeviceDescriptor.uiVendorId  	          = 0x1234;
  oDeviceDescriptor.uiProductId             = 0x1234;
  oDeviceDescriptor.uiBcdDevice             = 0x0200;
  oDeviceDescriptor.uiManufacturerStringIdx = 1;
  oDeviceDescriptor.uiProductStringIdx      = 2;
  oDeviceDescriptor.uiSerialNumberStringIdx = 3;
  oDeviceDescriptor.uiNumConfigurations     = 1;

  IUsbDevice::CConfigDescriptor& oConfig = oDeviceDescriptor.createConfig();
  oConfig.getConfig()->uiNumInterfaces = 2;
  oConfig.getConfig()->uiConfigurationValue = 1;
  oConfig.getConfig()->uiConfiguration = 0;
  oConfig.getConfig()->uiAttributes = 0xC0;
  oConfig.getConfig()->uiMaxPower   = 0x32; 

  IUsbDevice::SInterfaceDescriptor* oInterface0 = oConfig.createInterface();
  oInterface0->uiInterfaceNumber = 0;
  oInterface0->uiAlternateSetting = 0;
  oInterface0->uiNumEndpoints = 1;
  oInterface0->uiInterfaceClass = 0x02;
  oInterface0->uiInterfaceSubClass = 0x02;
  oInterface0->uiInterfaceProtocol = 0x01;
  oInterface0->uiInterfaceIdx = 0;

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

  oConfig.createEndpoint(0x82, 0x03, 0x08, 0x10, NewCcEvent(this, CcUsbCdc::onControl));
  IUsbDevice::SInterfaceDescriptor* pInterface1 = oConfig.createInterface();
  pInterface1->uiInterfaceNumber = 1;
  pInterface1->uiAlternateSetting = 0;
  pInterface1->uiNumEndpoints = 2;
  pInterface1->uiInterfaceClass = 0x0A;
  pInterface1->uiInterfaceSubClass = 0x00;
  pInterface1->uiInterfaceProtocol = 0x00;
  pInterface1->uiInterfaceIdx = 0;

  oConfig.createEndpoint(0x01, 0x02, 64, 0x00, NewCcEvent(this, CcUsbCdc::onDataIn));
  oConfig.createEndpoint(0x81, 0x02, 64, 0x00, NewCcEvent(this, CcUsbCdc::onDataOut));

  oDeviceDescriptor.createString("GER");
  oDeviceDescriptor.createString("CcOS Manu");
  oDeviceDescriptor.createString("CcOS Prod");
  oDeviceDescriptor.createString("CcOS Seri");
  oDeviceDescriptor.createString("CcOS Numm");

  m_pUsbDevice = m_oInterface.getDevice()->createDevice(oDeviceDescriptor);
  if(m_pUsbDevice)
  {
    m_pUsbDevice->registerInterfaceRequestEvent(NewCcEvent(this, CcUsbCdc::onInterfaceRequest));
    m_pUsbDevice->registerInterfaceReceiveEvent(NewCcEvent(this, CcUsbCdc::onInterfaceReceive));
    m_pUsbDevice->start();
  }
  else
  {
    oState = EStatus::InvalidHandle;
  }   
  return oState;
}

CcStatus CcUsbCdc::onStop()
{   
  CcStatus oState;
  if(m_pUsbDevice)
  {
    m_pUsbDevice->stop();
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
          uint32 uiLen = CCMIN(UsbCdcDataSize, m_oClassData.size());
          m_pUsbDevice->write(0, m_oClassData.cast<uint8>(), uiLen);
        }
        else
        {
          m_uiCmd = pRequest->bRequest;

          uint32 uiLen = CCMIN(UsbCdcDataSize, m_oClassData.size());
          m_pUsbDevice->read(0, m_oClassData.cast<uint8>(), uiLen);
        }
        break;
      }
      else
      {
        m_pUsbDevice->ctrlSendStatus();
      }
  }
}

void CcUsbCdc::onInterfaceReceive(IUsbDevice::SRequest* pRequest)
{
  CCUNUSED(pRequest);
  if(m_uiCmd != UINT8_MAX)
  {
    switch(m_uiCmd)
    {
      default:
      break;
    }
  }
  m_uiCmd = UINT8_MAX;
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
  if(m_oInputBuffer == "stop")
  {
    stop();
  }
}

void CcUsbCdc::onDataOut(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest)
{
  CCUNUSED(pRequest);
  IUsbDevice::debugMessage("CcUsbCdc::onDataOut");
}

