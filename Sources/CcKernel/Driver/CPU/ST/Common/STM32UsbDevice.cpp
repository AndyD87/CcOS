
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

void CallbackReceived(const char* pMessage)
{
  CcKernel::message(EMessage::Info, pMessage);
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
  CallbackReceived("HAL_PCD_SOFCallback");
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
  CallbackReceived("HAL_PCD_ResumeCallback");
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
  CallbackReceived("HAL_PCD_ISOINIncompleteCallback");
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
  CallbackReceived("HAL_PCD_ConnectCallback");
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
  CallbackReceived("HAL_PCD_DisconnectCallback");
}


STM32UsbDevice* s_pInstance = nullptr;

/**
 * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
 */
CCEXTERNC void OTG_HS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(s_pInstance->getPcdHandle());
}

/**
 * @brief  This function handles USB-On-The-Go HS global interrupt request.
 */
CCEXTERNC void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(s_pInstance->getPcdHandle());
}

STM32UsbDevice::STM32UsbDevice()
{
  s_pInstance = this;
  CcStatic_memsetZeroObject(m_oGlobalDef);
  CcStatic_memsetZeroObject(m_oConfigDef);
  CcStatic_memsetZeroObject(m_oPcdHandle);
}

STM32UsbDevice::~STM32UsbDevice()
{
}

CcStatus STM32UsbDevice::onState(EState eState)
{
  CcStatus bSuccess = true;
  switch(eState)
  {
    case EState::Start:
      if(HAL_OK != HAL_PCD_Start(getPcdHandle()))
      {
        bSuccess = false;
      }
      break;
    case EState::Stop:
      HAL_PCD_Stop(&m_oPcdHandle);
      break;
    default:
      break;
  }
  return bSuccess;
}

CcStatus STM32UsbDevice::loadDeviceDescriptor(const CDeviceDescriptor& oDescriptor)
{
  CcStatus oStatus = EStatus::CommandInvalidParameter;
  m_oDeviceDescriptor = oDescriptor;
  /*Set LL Driver parameters */
  m_oPcdHandle.Instance                 = USB_OTG_FS;
  m_oPcdHandle.Init.dev_endpoints       = 4;
  m_oPcdHandle.Init.speed               = PCD_SPEED_FULL;
  m_oPcdHandle.Init.dma_enable          = DISABLE;
  m_oPcdHandle.Init.phy_itface          = PCD_PHY_EMBEDDED;
  m_oPcdHandle.Init.Sof_enable          = DISABLE;
  m_oPcdHandle.Init.low_power_enable    = DISABLE;
  m_oPcdHandle.Init.vbus_sensing_enable = DISABLE;
  m_oPcdHandle.Init.use_dedicated_ep1   = DISABLE;
  /* Link The driver to the stack */
  m_oPcdHandle.pData = this;

  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

  if(HAL_OK == HAL_PCD_Init(getPcdHandle()))
  {
    HAL_PCDEx_SetRxFiFo(getPcdHandle(), 0x80);
    HAL_PCDEx_SetTxFiFo(getPcdHandle(), 0, 0x40);
    HAL_PCDEx_SetTxFiFo(getPcdHandle(), 1, 0x80);
    oStatus = true;
  }
  return oStatus;
}

void STM32UsbDevice::setUsbState(EUsbState eNewState)
{
  switch(eNewState) 
  {
    case EUsbState::Default:
      CCFALLTHROUGH;
    case EUsbState::Addressed:
      CCFALLTHROUGH;
    case EUsbState::Configured:
      m_eCurrentSate = eNewState; 
      break;
    case EUsbState::Suspended:
      __HAL_PCD_GATE_PHYCLOCK(getPcdHandle());
      m_eOldState = m_eCurrentSate;
      m_eCurrentSate = eNewState; 
    break;
    case EUsbState::Resume:
      // Reload old state
      if(m_eCurrentSate == EUsbState::Suspended)
        m_eCurrentSate = m_eOldState; 
    break;
  }
}

void STM32UsbDevice::doReset()
{
  // EPTypes:
  //   EP_TYPE_CTRL                           0U
  //   EP_TYPE_ISOC                           1U
  //   EP_TYPE_BULK                           2U
  //   EP_TYPE_INTR                           3U
  //   EP_TYPE_MSK                            3U
  // configure endpoints!
  /* Open EP0 OUT */
  HAL_StatusTypeDef status = HAL_PCD_EP_Open(getPcdHandle(), 0x00, uiEp0MaxSize, EP_TYPE_CTRL);
  /* Open EP0 IN */
  if(status == HAL_OK)
  {
    status = HAL_PCD_EP_Open(getPcdHandle(), 0x80, uiEp0MaxSize, EP_TYPE_CTRL);
  }
}

#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))
#define  USB_REQ_RECIPIENT_DEVICE                       0x00U
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01U
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02U
#define  USB_REQ_RECIPIENT_MASK                         0x03U

void STM32UsbDevice::doSetup()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);

  switch (pRequest->bmRequest & 0x1FU)
  {
    case USB_REQ_RECIPIENT_DEVICE:
      doSetupDevice();
      break;

    case USB_REQ_RECIPIENT_INTERFACE:
      doSetupInterface();
      break;

    case USB_REQ_RECIPIENT_ENDPOINT:
      doSetupEndPoint();
      break;

    default:
      CallbackReceived("Unknown Request");
      stallEp(0x80);
      break;
  }
}


#define  USB_REQ_TYPE_STANDARD                          0x00U
#define  USB_REQ_TYPE_CLASS                             0x20U
#define  USB_REQ_TYPE_VENDOR                            0x40U
#define  USB_REQ_TYPE_MASK                              0x60U

#define  USB_REQ_GET_STATUS                             0x00U
#define  USB_REQ_CLEAR_FEATURE                          0x01U
#define  USB_REQ_SET_FEATURE                            0x03U
#define  USB_REQ_SET_ADDRESS                            0x05U
#define  USB_REQ_GET_DESCRIPTOR                         0x06U
#define  USB_REQ_SET_DESCRIPTOR                         0x07U
#define  USB_REQ_GET_CONFIGURATION                      0x08U
#define  USB_REQ_SET_CONFIGURATION                      0x09U
#define  USB_REQ_GET_INTERFACE                          0x0AU
#define  USB_REQ_SET_INTERFACE                          0x0BU
#define  USB_REQ_SYNCH_FRAME                            0x0CU

void STM32UsbDevice::doSetupDevice()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  switch (pRequest->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_STANDARD:
      switch (pRequest->bRequest)
      {
        case USB_REQ_GET_DESCRIPTOR:
          doSetupDeviceDescriptor();
          break;
        case USB_REQ_SET_ADDRESS:
          if ((pRequest->wIndex == 0U) && (pRequest->wLength == 0U) && (pRequest->wValue < 128U))
          {
            if(m_eCurrentSate == EUsbState::Configured)
            {
              // Write Error
              CallbackReceived("Error already configured");
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
                setUsbState(EUsbState::Default);
              }
            }
          }
          else
          {
            // Write Error
          }
          break;
        case USB_REQ_SET_CONFIGURATION:
          doSetConfiguration();
          break;
        case USB_REQ_GET_CONFIGURATION:
        case USB_REQ_GET_STATUS:
        case USB_REQ_SET_FEATURE:
        case USB_REQ_CLEAR_FEATURE:
        default:
          CallbackReceived("USB_REQ_...");
          break;
      }
      break;
    case USB_REQ_TYPE_CLASS:
    case USB_REQ_TYPE_VENDOR:
    default:
      CallbackReceived("USB_REQ_TYPE_CLASS/USB_REQ_TYPE_VENDOR");
      break;

  }
}

#define  USB_DESC_TYPE_DEVICE                           0x01U
#define  USB_DESC_TYPE_CONFIGURATION                    0x02U
#define  USB_DESC_TYPE_STRING                           0x03U
#define  USB_DESC_TYPE_ENDPOINT                         0x05U
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                 0x06U
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION        0x07U
#define  USB_DESC_TYPE_BOS                              0x0FU


void STM32UsbDevice::doSetupDeviceDescriptor()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  uint8* pBuffer = nullptr;
  uint16 uiSize = 0;

  //eEp0State = EEnpointState::Setup;

  switch(pRequest->wValue >> 8)
  {
    case USB_DESC_TYPE_DEVICE:
      pBuffer = reinterpret_cast<uint8*>(&m_oDeviceDescriptor);
      uiSize = m_oDeviceDescriptor.uiLength;
      break;
    case USB_DESC_TYPE_STRING:
      doSetupDeviceDescriptorString(pBuffer, uiSize);
      break;
    case USB_DESC_TYPE_CONFIGURATION:
      doSetupDeviceConfigDescriptor(pBuffer, uiSize);
      break;
    case USB_DESC_TYPE_DEVICE_QUALIFIER:
    case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
    default:
      CallbackReceived("Test");
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
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  uint8 uiIndex = pRequest->wValue & 0xff;
  if(uiIndex < m_oDeviceDescriptor.getStrings().size())
  {
    pBuffer = reinterpret_cast<uint8*>(&m_oDeviceDescriptor.getStrings()[uiIndex]);
    uiSize = m_oDeviceDescriptor.getStrings()[uiIndex]->uiLength;
  }
}

void STM32UsbDevice::doSetupDeviceConfigDescriptor(uint8*& pBuffer, uint16& uiSize)
{
  pBuffer = reinterpret_cast<uint8*>(m_oDeviceDescriptor.getConfigs()[0].getConfig());
  uiSize = m_oDeviceDescriptor.getConfigs()[0].getConfig()->uiTotalLength;
}

void STM32UsbDevice::doSetupInterface()
{
  //CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  CallbackReceived("doSetupInterface");
}

void STM32UsbDevice::doSetupEndPoint()
{
  //CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  CallbackReceived("doSetupEndPoint");
}

void STM32UsbDevice::doSetConfiguration()
{
  //CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  if(m_eCurrentSate == EUsbState::Addressed)
  {
    // @todo Configure and open endpoints. Enpoint interfaces are required
  }
}

void STM32UsbDevice::doInputData(uint8 uiEndpoint, uint8* pBuffer)
{
  if(uiEndpoint == 0)
  {
    if(eEp0State == EEnpointState::DataIn)
    {
      if(writeRequired())
      {
        m_uiWriteSize -= uiEp0MaxSize;
        writeContinue(uiEndpoint, pBuffer, m_uiWriteSize);
        
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
    else
    {
    }
  }
  else
  {
    CallbackReceived("Endpoint IN called");
  }
}

void STM32UsbDevice::doOutputData(uint8 uiEndpoint, const uint8* pBuffer)
{
  if(uiEndpoint == 0)
  {                                                                                                                                    
    // Check if more data is required
    if(eEp0State == EEnpointState::DataOut)
    {
      if(0)
        ctrlReceiveStatus();
      
      ctrlSendStatus();
    }
    // Check if more data is required
    else if(eEp0State == EEnpointState::StateOut)
    {
      eEp0State = EEnpointState::Idle;
      stallEp(0U);
    }
  }
  else
  {
    CallbackReceived("Endpoint OUT called");
  }
}


void STM32UsbDevice::write(uint8 uiEndpoint, const uint8* pBuffer, uint16 uiSize)
{
  m_uiWriteSize = uiSize;
  eEp0State = EEnpointState::DataIn;
  if(HAL_OK == HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, const_cast<uint8*>(pBuffer), uiSize))
  {
  }
}

bool STM32UsbDevice::writeRequired()
{
  return m_uiWriteSize > uiEp0MaxSize;
}

void STM32UsbDevice::writeContinue(uint8 uiEndpoint, uint8* pBuffer, uint16 uiSize)
{
  if(HAL_OK == HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, pBuffer, uiSize))
  {
  }
}

void STM32UsbDevice::stallEp(uint8 uiEndpoint)
{
  HAL_PCD_EP_SetStall(getPcdHandle(), uiEndpoint);
}

void STM32UsbDevice::ctrlSendStatus()
{
  eEp0State = EEnpointState::StateIn;
  HAL_PCD_EP_Transmit(getPcdHandle(), 0, nullptr, 0);
}

void STM32UsbDevice::ctrlReceiveStatus()
{
  eEp0State = EEnpointState::StateOut;
  HAL_PCD_EP_Receive(getPcdHandle(), 0, nullptr, 0);
}