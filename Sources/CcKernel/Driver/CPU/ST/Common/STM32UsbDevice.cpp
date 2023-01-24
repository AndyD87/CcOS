
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

#include "STM32UsbDevice.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "usbh_conf.h"

#define USBD_EP_TYPE_BULK                               0x02U

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
  HAL_Custom_MspInit(pcdHandle);
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
  HAL_Custom_MspDeinit(pcdHandle);
}

/**
  * @brief  Data OUT stage callback.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  static_cast<STM32UsbDevice*>(hpcd->pData)->doOutputData(epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
  * @brief  Data IN stage callback
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  static_cast<STM32UsbDevice*>(hpcd->pData)->doInputData(epnum, hpcd->IN_ep[epnum].xfer_buff);
}
/**
  * @brief  Setup stage callback
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32UsbDevice*>(hpcd->pData)->doSetup();
}

/**
  * @brief  USB Start Of Frame callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  //IUsbDevice::debugMessage("HAL_PCD_SOFCallback");
}

/**
  * @brief  USB Reset callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32UsbDevice*>(hpcd->pData)->doReset();
}

/**
  * @brief  Suspend event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32UsbDevice*>(hpcd->pData)->setUsbState(STM32UsbDevice::EUsbState::Suspended);
}

/**
  * @brief  Resume event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32UsbDevice*>(hpcd->pData)->setUsbState(STM32UsbDevice::EUsbState::Resume);
}

/**
  * @brief  Incomplete ISO OUT callback.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);
  IUsbDevice::debugMessage("HAL_PCD_ResumeCallback");
}

/**
  * @brief  Incomplete ISO IN callback.
  * @param  hpcd PCD handle
  * @param  epnum endpoint number
  * @retval None
  */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  UNUSED(epnum);
  IUsbDevice::debugMessage("HAL_PCD_ISOINIncompleteCallback");
}

/**
  * @brief  Connection event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  IUsbDevice::debugMessage("HAL_PCD_ConnectCallback");
}

/**
  * @brief  Disconnection event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpcd);
  IUsbDevice::debugMessage("HAL_PCD_DisconnectCallback");
}


STM32UsbDevice* s_pInstance = nullptr;

#ifdef USB_EXTENDED_OFF
  /**
   * @brief  This function handles USB-On-The-Go HS global interrupt request.
   */
  CCEXTERNC void USB_LP_CAN1_RX0_IRQHandler(void)
  {
    if(s_pInstance)
    {
      HAL_PCD_IRQHandler(s_pInstance->getPcdHandle());
    }
  }
#else
  /**
   * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
   */
  CCEXTERNC void OTG_HS_IRQHandler(void)
  {
    if(s_pInstance)
      HAL_PCD_IRQHandler(s_pInstance->getPcdHandle());
  }

  /**
   * @brief  This function handles USB-On-The-Go HS global interrupt request.
   */
  CCEXTERNC void OTG_FS_IRQHandler(void)
  {
    if(s_pInstance)
      HAL_PCD_IRQHandler(s_pInstance->getPcdHandle());
  }
#endif

STM32UsbDevice::STM32UsbDevice()
{
  s_pInstance = this;
  CcStatic_memsetZeroObject(m_oPcdHandle);
}

STM32UsbDevice::~STM32UsbDevice()
{
  s_pInstance = nullptr;
}

CcStatus STM32UsbDevice::onState(EState eState)
{
  CcStatus bSuccess = true;
  switch(eState)
  {
    case EState::Start:
      bSuccess = onStart();
      if(bSuccess)
      {
        if(HAL_OK != HAL_PCD_Start(getPcdHandle()))
        {
          bSuccess = false;      
        }
        else
        {
          registerIdle();
        }
      }
      break;
    case EState::Stop:
      deregisterIdle();
      HAL_PCD_Stop(getPcdHandle());
      HAL_PCD_DeInit(getPcdHandle());
      break;
    default:
      break;
  }
  return bSuccess;
}

void STM32UsbDevice::idle()
{
  if(m_eCurrentSate == EUsbState::Configured)
  {
    IUsbDevice::CConfigDescriptor& oConfig = m_oDeviceDescriptor.getActiveConfig();
    for(CConfigDescriptor::CEndpointInfo& oInfo : oConfig.getEndpointInfos())
    {
      // Check for pending reads
      if(oInfo.pDescriptor->uiEndpointAddress < 80)
      {
        if(oInfo.oBufferList.getChunkCount())
        {
          oInfo.oOnChange.call(&oInfo);
        }
      }
      // Check for pending writes
      else
      {
        // Check if write is already active
        if(oInfo.eState == EEnpointState::DataIn &&
            oInfo.oBufferList.getChunkCount())
        {
          //HAL_PCD_EP_Transmit(getPcdHandle(), oInfo.pDescriptor->uiEndpointAddress, oInfo.oBufferList[0].cast<uint8>(), oInfo.oBufferList[0].size());
        }
      }
    }
  }
}

CcStatus STM32UsbDevice::onStart()
{
  CcStatus oStatus = EStatus::CommandInvalidParameter;
  
  /*Set LL Driver parameters */
  #ifdef HS
    m_oPcdHandle.Instance                 = USB_OTG_HS;
    m_oPcdHandle.Init.dev_endpoints       = getDeviceDescriptor().getActiveConfig().getEndpointCount();
    m_oPcdHandle.Init.speed               = PCD_SPEED_FULL;
    m_oPcdHandle.Init.dma_enable          = DISABLE;
    m_oPcdHandle.Init.phy_itface          = USB_OTG_EMBEDDED_PHY;
    m_oPcdHandle.Init.Sof_enable          = DISABLE;
    m_oPcdHandle.Init.low_power_enable    = DISABLE;
    m_oPcdHandle.Init.vbus_sensing_enable = ENABLE;
    m_oPcdHandle.Init.use_dedicated_ep1   = DISABLE;
  #else
    m_oPcdHandle.Init.dev_endpoints       = getDeviceDescriptor().getActiveConfig().getEndpointCount();
    m_oPcdHandle.Init.phy_itface          = PCD_PHY_EMBEDDED;
    m_oPcdHandle.Init.Sof_enable          = DISABLE;
    m_oPcdHandle.Init.low_power_enable    = DISABLE;
    m_oPcdHandle.Init.speed               = PCD_SPEED_FULL;
    #ifndef USB_EXTENDED_OFF
      m_oPcdHandle.Instance                 = USB_OTG_FS;
      m_oPcdHandle.Init.dma_enable          = DISABLE;
      m_oPcdHandle.Init.vbus_sensing_enable = DISABLE;
      m_oPcdHandle.Init.use_dedicated_ep1   = DISABLE;
    #else
      m_oPcdHandle.Instance                 = USB;
      m_oPcdHandle.Init.lpm_enable = DISABLE;
      m_oPcdHandle.Init.battery_charging_enable = DISABLE;
    #endif
  #endif
  /* Link The driver to the stack */
  m_oPcdHandle.pData = this;

  __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

  if(HAL_OK == HAL_PCD_Init(getPcdHandle()))
  {
    #ifdef USB_EXTENDED_OFF
      HAL_PCDEx_PMAConfig(getPcdHandle(), 0x00 , PCD_SNG_BUF, m_uiPmaOffset);
      m_uiPmaOffset += m_uiEp0MaxSize;
      HAL_PCDEx_PMAConfig(getPcdHandle(), 0x80 , PCD_SNG_BUF, m_uiPmaOffset);
      m_uiPmaOffset += m_uiEp0MaxSize;
    #elif defined(HS)
      HAL_PCDEx_SetRxFiFo(getPcdHandle(), m_uiEp0MaxSize);
      HAL_PCDEx_SetTxFiFo(getPcdHandle(), 0, 0x80);
    #else
      HAL_PCDEx_SetRxFiFo(getPcdHandle(), m_uiEp0MaxSize);
      HAL_PCDEx_SetTxFiFo(getPcdHandle(), 0, 0x40);
    #endif
    //Configure and open endpoints. Enpoint interfaces are required
    IUsbDevice::CConfigDescriptor& oConfig = m_oDeviceDescriptor.getActiveConfig();
    for(size_t uiIdx=0; uiIdx<oConfig.getEndpointCount(); uiIdx++)  
    { 
      IUsbDevice::SEndpointDescriptor* pEndPoint = oConfig.getEndpoint(uiIdx);
      if(pEndPoint->uiEndpointAddress == 0 ||
          pEndPoint->uiEndpointAddress == 0x80)
      { 
        // Nothing to do, it is our default config
      }
      else if(pEndPoint->uiEndpointAddress < 0x80)
      {
        #ifdef USB_EXTENDED_OFF
          HAL_PCDEx_PMAConfig(getPcdHandle(), pEndPoint->uiEndpointAddress, PCD_SNG_BUF, m_uiPmaOffset);
          m_uiPmaOffset += pEndPoint->wMaxPacketSize;
        #endif
      }
      else
      {
        #ifdef USB_EXTENDED_OFF
          HAL_PCDEx_PMAConfig(getPcdHandle(), pEndPoint->uiEndpointAddress, PCD_SNG_BUF, m_uiPmaOffset);
          m_uiPmaOffset += pEndPoint->wMaxPacketSize;
        #else
          HAL_PCDEx_SetTxFiFo(getPcdHandle(), pEndPoint->uiEndpointAddress & ~0x80, pEndPoint->wMaxPacketSize);
        #endif
      }
    }
    oStatus = true;
  }
  return oStatus;
}

void STM32UsbDevice::setUsbState(EUsbState eNewState)
{
  switch(eNewState) 
  {
    case EUsbState::Connected:
    {
      // EPTypes:
      //   EP_TYPE_CTRL                           0U
      //   EP_TYPE_ISOC                           1U
      //   EP_TYPE_BULK                           2U
      //   EP_TYPE_INTR                           3U
      //   EP_TYPE_MSK                            3U
      // configure endpoints!
      /* Open EP0 OUT */
      HAL_StatusTypeDef status = HAL_PCD_EP_Open(getPcdHandle(), 0x00, m_uiEp0MaxSize, EP_TYPE_CTRL);
      /* Open EP0 IN */
      if(status == HAL_OK || status == HAL_BUSY)
      {
        status = HAL_PCD_EP_Open(getPcdHandle(), 0x80, m_uiEp0MaxSize, EP_TYPE_CTRL);
      }
      m_eCurrentSate = eNewState; 
      break;
    }
    case EUsbState::Addressed:
      CCFALLTHROUGH;
    case EUsbState::Configured:
      m_eCurrentSate = eNewState;       
      break;
    case EUsbState::Suspended:
    {
      __HAL_PCD_GATE_PHYCLOCK(getPcdHandle());
      // Close all endpoints
      HAL_PCD_EP_Close(getPcdHandle(), 0x00);
      HAL_PCD_EP_Close(getPcdHandle(), 0x80);
      IUsbDevice::CConfigDescriptor& oConfig = m_oDeviceDescriptor.getActiveConfig();
      for(size_t uiIdx=0; uiIdx<oConfig.getEndpointCount(); uiIdx++)  
      { 
        IUsbDevice::SEndpointDescriptor* pEndPoint = oConfig.getEndpoint(uiIdx);
        HAL_PCD_EP_Close(
          getPcdHandle(), 
          pEndPoint->uiEndpointAddress
        );
      }
      m_eOldState = m_eCurrentSate;
      m_eCurrentSate = eNewState; 
      break;
    }
    case EUsbState::Resume:
      // Reload old state
      if(m_eCurrentSate == EUsbState::Suspended)
        m_eCurrentSate = m_eOldState; 
    break;
  }
}

void STM32UsbDevice::doReset()
{
  setUsbState(Connected);
}

#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))

void STM32UsbDevice::doSetup()
{
  SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);

  switch (pRequest->bmRequest & 0x1FU)
  {
    case UsbRequestTarget_DEVICE:
      doSetupDevice();
      break;

    case UsbRequestTarget_INTERFACE:
      doSetupInterface();
      break;

    case UsbRequestTarget_ENDPOINT:
      doSetupEndPoint();
      break;

    default:
      IUsbDevice::debugMessage("Unknown Request");
      stallEp(0x80);
      break;
  }
}


void STM32UsbDevice::doSetupDevice()
{
  SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
  switch (pRequest->bmRequest & UsbRequestType_MASK)
  {
    case UsbRequestType_STANDARD:
      switch (pRequest->bRequest)
      {
        case UsbRequest_GET_DESCRIPTOR:
          doSetupDeviceDescriptor();
          break;
        case UsbRequest_SET_ADDRESS:
          if ((pRequest->wIndex == 0U) && (pRequest->wLength == 0U) && (pRequest->wValue < 128U))
          {
            if(m_eCurrentSate == EUsbState::Configured)
            {
              // Write Error
              IUsbDevice::debugMessage("Error already configured");
            }
            else
            {
              m_uiUsbAddress = (uint8_t)(pRequest->wValue) & 0x7FU;
              HAL_PCD_SetAddress(getPcdHandle(), m_uiUsbAddress);
             
              ctrlSendStatus();

              if (m_uiUsbAddress != 0U)
              {
                setUsbState(EUsbState::Addressed);
              }
              else
              {
                setUsbState(EUsbState::Connected);
              }
            }
          }
          else
          {
            IUsbDevice::debugMessage("Invalid request");
          }
          break;
        case UsbRequest_SET_CONFIGURATION:
          doSetConfiguration();
          setUsbState(EUsbState::Configured);
          ctrlSendStatus();
          break;
        case UsbRequest_GET_CONFIGURATION:
        case UsbRequest_GET_STATUS:
        case UsbRequest_SET_FEATURE:
        case UsbRequest_CLEAR_FEATURE:
        default:
          IUsbDevice::debugMessage("UsbRequest_...");
          break;
      }
      break;
    case UsbRequestType_CLASS:
    case UsbRequestType_VENDOR:
    default:
      IUsbDevice::debugMessage("UsbRequestType_CLASS/UsbRequestType_VENDOR");
      break;

  }
}

void STM32UsbDevice::doSetupDeviceDescriptor()
{
  SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
  uint8* pBuffer = nullptr;
  uint16 uiSize = 0;

  //m_eEp0State = EEnpointState::Setup;

  switch(pRequest->wValue >> 8)
  {
    case UsbDescriptorType_DEVICE:
      pBuffer = reinterpret_cast<uint8*>(&m_oDeviceDescriptor);
      uiSize = m_oDeviceDescriptor.uiLength;
      break;
    case UsbDescriptorType_STRING:
      doSetupDeviceDescriptorString(pBuffer, uiSize);
      break;
    case UsbDescriptorType_CONFIGURATION:
      doSetupDeviceConfigDescriptor(pBuffer, uiSize);
      break;
    case UsbDescriptorType_DEVICE_QUALIFIER:
      ctrlSendError();
      break;
    case UsbDescriptorType_OTHER_SPEED_CONFIGURATION:
    default:
      //IUsbDevice::debugMessage("Test");
      break;
  }
  if(pBuffer != nullptr && uiSize > 0)
  {
    write(0, pBuffer, CCMIN(uiSize, pRequest->wLength));
  }
  else
  {
    ctrlSendStatus();
  }
}

void STM32UsbDevice::doSetupDeviceDescriptorString(uint8*& pBuffer, uint16& uiSize)
{
  SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
  uint8 uiIndex = (pRequest->wValue & 0xff);
  if(uiIndex < m_oDeviceDescriptor.getStrings().size())
  {
    pBuffer = reinterpret_cast<uint8*>(m_oDeviceDescriptor.getStrings()[uiIndex]);
    uiSize = m_oDeviceDescriptor.getStrings()[uiIndex]->uiLength;
  }
}

void STM32UsbDevice::doSetupDeviceConfigDescriptor(uint8*& pBuffer, uint16& uiSize)
{
  pBuffer = reinterpret_cast<uint8*>(m_oDeviceDescriptor.getActiveConfig().getConfig());
  uiSize = m_oDeviceDescriptor.getActiveConfig().getConfig()->uiTotalLength;
}

void STM32UsbDevice::doSetupInterface()
{
  SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
  /* Find the class ID relative to the current request */
  switch (pRequest->bmRequest & 0x1FU)
  {
    case UsbRequestTarget_INTERFACE:
    {
      CConfigDescriptor* pConfig;
      m_oDeviceDescriptor.findInterface(pRequest->wIndex, &pConfig);
      if(pConfig)
      {
        pConfig->getInterfaceInfo(pRequest->wIndex).oOnRequest.call(pRequest);
      }
      break;
    }
    case UsbRequestTarget_ENDPOINT:
      IUsbDevice::debugMessage("STM32UsbDevice::doOutputData Endpoint request");
      break;
    default:
      IUsbDevice::debugMessage("STM32UsbDevice::doOutputData Unknown request");
      break;
  }
}

void STM32UsbDevice::doSetupEndPoint()
{
  SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
  switch (pRequest->bRequest)
  {
    case UsbRequest_SET_FEATURE:
      if (pRequest->wValue == UsbFeature_EP_HALT)
      {
        HAL_PCD_EP_SetStall(getPcdHandle(), pRequest->wIndex & 0xff);
      }
      else
      {
        IUsbDevice::debugMessage("STM32UsbDevice::doSetupEndPoint UsbRequest_SET_FEATURE unknown");
      }
      break;
    case UsbRequest_CLEAR_FEATURE:
      if (pRequest->wValue == UsbFeature_EP_HALT)
      {
        HAL_PCD_EP_ClrStall(getPcdHandle(), pRequest->wIndex & 0xff);
      }
      else
      {
        IUsbDevice::debugMessage("STM32UsbDevice::doSetupEndPoint UsbRequest_CLEAR_FEATURE unknown");
      }
      break;
    case UsbRequest_GET_STATUS:
      IUsbDevice::debugMessage("STM32UsbDevice::doSetupEndPoint UsbRequest_GET_STATUS request");
      break;
  }
}

void STM32UsbDevice::doSetConfiguration()
{
  //SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
  if(m_eCurrentSate == EUsbState::Addressed)
  {
    // @todo Configure and open endpoints. Enpoint interfaces are required
    IUsbDevice::CConfigDescriptor& oConfig = m_oDeviceDescriptor.getActiveConfig();
    for(size_t uiIdx=0; uiIdx<oConfig.getEndpointCount(); uiIdx++)  
    { 
      IUsbDevice::SEndpointDescriptor* pEndPoint = oConfig.getEndpoint(uiIdx);
      IUsbDevice::CConfigDescriptor::CEndpointInfo& oEndPointInfo = oConfig.getEndpointInfo(uiIdx);
      oEndPointInfo.reset();
      
      HAL_StatusTypeDef uiHallResult = HAL_PCD_EP_Open(
                                          getPcdHandle(), 
                                          pEndPoint->uiEndpointAddress,
                                          pEndPoint->wMaxPacketSize,
                                          pEndPoint->uiAttributes
                                        );
      if(uiHallResult != HAL_OK)
      {
        IUsbDevice::debugMessage(("Failed to setup endpoint: " + CcString::fromNumber((uint8)uiHallResult)).getCharString());
      }
      else if(pEndPoint->uiEndpointAddress == 0 ||
          pEndPoint->uiEndpointAddress == 0x80)
      { 
        // Nothing to do, it is our default config
      }
      else if(pEndPoint->uiEndpointAddress < 0x80)
      {
        // Setup receive buffer
        oEndPointInfo.oInBuffer.resize(pEndPoint->wMaxPacketSize);
        uiHallResult = HAL_PCD_EP_Receive(
          getPcdHandle(), 
          pEndPoint->uiEndpointAddress,
          oEndPointInfo.oInBuffer.cast<uint8>(), 
          pEndPoint->wMaxPacketSize
        );
        // Prepare receive
        if(HAL_OK != uiHallResult)
        {
          IUsbDevice::debugMessage("Failed to setup receive endpoint");
        }
      }
      else
      {
        #ifndef USB_EXTENDED_OFF
          HAL_PCDEx_SetTxFiFo(getPcdHandle(), pEndPoint->uiEndpointAddress & ~0x80, pEndPoint->wMaxPacketSize);
        #endif
      }
    }
  }
}

void STM32UsbDevice::doInputData(uint8 uiEndpoint, uint8* pBuffer)
{
  if(uiEndpoint == 0)
  {
    if(m_eEp0State == EEnpointState::DataIn)
    {
      if(m_uiWriteSize > m_uiEp0MaxSize)
      {
        m_uiWriteSize -= m_uiEp0MaxSize;
        write(uiEndpoint, pBuffer, m_uiWriteSize);
        
        // Prepare for suprised transer end
        HAL_PCD_EP_Receive(getPcdHandle(), 0, nullptr, 0);
      }
      else
      {
        stallEp(0x80U);

        // Prepare receive
        ctrlReceiveStatus();
      }
    }
    else if(m_eEp0State == EEnpointState::StateIn ||
            m_eEp0State == EEnpointState::Idle)
    {
      stallEp(0x80U);
    }
    else
    {
      IUsbDevice::debugMessage("Invalid Ctrl IN called");
    }
  }
  else
  {
    uiEndpoint |= 0x80;
    CConfigDescriptor* pConfig = nullptr;
    uint8 uiIdx = m_oDeviceDescriptor.findEndpoint(uiEndpoint, &pConfig);
    if(pConfig)
    {
      CConfigDescriptor::CEndpointInfo& oEndpointInfo = pConfig->getEndpointInfo(uiIdx);
      if(oEndpointInfo.eState == EEnpointState::DataIn)
      {
        if(oEndpointInfo.oBufferList.getChunkCount() > 0)
        {
          if(oEndpointInfo.oBufferList[0].size() > oEndpointInfo.pDescriptor->wMaxPacketSize)
          {
            HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, oEndpointInfo.oBufferList[0].cast<uint8>(), oEndpointInfo.pDescriptor->wMaxPacketSize);
            oEndpointInfo.oBufferList[0].remove(0, oEndpointInfo.pDescriptor->wMaxPacketSize);
          }
          else
          {
            HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, oEndpointInfo.oBufferList[0].cast<uint8>(), oEndpointInfo.oBufferList[0].size());
            oEndpointInfo.oBufferList.removeChunk(0);
          }
        }
        else
        {
          oEndpointInfo.eState = EEnpointState::Idle;
        }
      }
    }
    else
    {
      IUsbDevice::debugMessage("Invalid Endpoint IN called");
    }
  }
}

void STM32UsbDevice::doOutputData(uint8 uiEndpoint, uint8* pBuffer)
{
  if(uiEndpoint == 0)
  {                                                                                                                                    
    // Check if more data is required
    if(m_eEp0State == EEnpointState::DataOut)
    {
      if(m_uiReadSize > m_uiEp0MaxSize)
      {
        m_uiReadSize -= m_uiEp0MaxSize;
        read(uiEndpoint, pBuffer, m_uiReadSize);
      }
      else
      {
        SRequest* pRequest = reinterpret_cast<SRequest*>(m_oPcdHandle.Setup);
        
        /* Find the class ID relative to the current request */
        switch (pRequest->bmRequest & 0x1FU)
        {
          case UsbRequestTarget_INTERFACE:
          {
            CConfigDescriptor* pConfig;
            m_oDeviceDescriptor.findInterface(pRequest->wIndex, &pConfig);
            if(pConfig)
            {
              CConfigDescriptor::CInterfaceInfo& oInterface = pConfig->getInterfaceInfo(pRequest->wIndex);
              // Signal transfer done
              oInterface.oOnReadDone.call(pRequest);
            }
            break;
          }
          case UsbRequestTarget_ENDPOINT:
            IUsbDevice::debugMessage("STM32UsbDevice::doOutputData Endpoint request");
            break;
          default:
            break;
        }

        ctrlSendStatus();
      }
    }
    // Check if more data is required
    else if(m_eEp0State == EEnpointState::StateOut)
    {
      m_eEp0State = EEnpointState::Idle;
      stallEp(0U);
    }
    else
    {
      IUsbDevice::debugMessage("Invalid Ctrl OUT called");
    }
  }
  else
  {
    CConfigDescriptor* pConfig = nullptr;
    uint8 uiIdx = m_oDeviceDescriptor.findEndpoint(uiEndpoint, &pConfig);
    if(pConfig)
    {
      // Write data to buffer list
      CConfigDescriptor::CEndpointInfo& oEndpointInfo = pConfig->getEndpointInfo(uiIdx);
      if(oEndpointInfo.oBufferList.getChunkCount() < oEndpointInfo.uiMaxBufferList)
      {
        oEndpointInfo.oBufferList.append(oEndpointInfo.oInBuffer.getArray(), getPcdHandle()->OUT_ep[uiEndpoint].xfer_count);
      }

      // Reset Input buffer
      read(uiEndpoint, oEndpointInfo.oInBuffer.cast<uint8>(), oEndpointInfo.oInBuffer.size());
    }
    else
    {
      IUsbDevice::debugMessage("Invalid Endpoint OUT called");
    }
  }
}

size_t STM32UsbDevice::write(uint8 uiEndpoint, const uint8* pBuffer, uint16 uiSize)
{
  size_t uiWritten = SIZE_MAX;
  CConfigDescriptor* pConfig = nullptr;
  if(uiEndpoint != 0)
  {
    if(m_eCurrentSate == EUsbState::Configured)
    {
      uint8 uiIdx = m_oDeviceDescriptor.findEndpoint(uiEndpoint, &pConfig);
      if(pConfig)
      {
        CConfigDescriptor::CEndpointInfo& oEndpointInfo = pConfig->getEndpointInfo(uiIdx);

        // Check if write is already active
        if(oEndpointInfo.eState == EEnpointState::Idle)
        {
          oEndpointInfo.eState = EEnpointState::DataIn;
          HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, const_cast<uint8*>(pBuffer), uiSize);
          uiWritten = uiSize;
        }
        else
        {
          if(oEndpointInfo.oBufferList.size() < oEndpointInfo.uiMaxBufferList)
          {
            oEndpointInfo.oBufferList.append(pBuffer, uiSize);
            uiWritten = uiSize;
          }
          else
          {
            IUsbDevice::debugMessage("Buffer exceeded");
          }
        }
      }
    }
  }
  else
  {
    uiWritten = uiSize;
    m_uiWriteSize = uiSize;
    m_eEp0State = EEnpointState::DataIn;
    if(HAL_OK == HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, const_cast<uint8*>(pBuffer), uiSize))
    {
    }
  }
  return uiWritten;
}

size_t STM32UsbDevice::read(uint8 uiEndpoint, uint8* pBuffer, uint16 uiSize)
{
  size_t uiRead = SIZE_MAX;
  if(uiEndpoint != 0)
  {
    uiRead = uiSize;
    if(HAL_OK == HAL_PCD_EP_Receive(getPcdHandle(), uiEndpoint, pBuffer, uiSize))
    {
    }
  }
  else
  {
    uiRead = uiSize;
    m_uiReadSize = uiSize;
    m_eEp0State = EEnpointState::DataOut;
    if(HAL_OK == HAL_PCD_EP_Receive(getPcdHandle(), uiEndpoint, pBuffer, uiSize))
    {
    }
  }
  return uiRead;
}

void STM32UsbDevice::stallEp(uint8 uiEndpoint)
{
  HAL_PCD_EP_SetStall(getPcdHandle(), uiEndpoint);
}

void STM32UsbDevice::ctrlSendError()
{
  stallEp(0);
  stallEp(0x80);
}

void STM32UsbDevice::ctrlSendStatus()
{
   m_eEp0State = EEnpointState::StateIn;
  HAL_PCD_EP_Transmit(getPcdHandle(), 0, nullptr, 0);
}

void STM32UsbDevice::ctrlReceiveStatus()
{
  m_eEp0State = EEnpointState::StateOut;
  HAL_PCD_EP_Receive(getPcdHandle(), 0, nullptr, 0);
}
