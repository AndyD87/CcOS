
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

#include "STM32F407UsbDevice.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "usbh_conf.h"
#include <stm32f4xx_hal_hcd.h>
#include <stm32f4xx_hal_pcd.h>
#include <stm32f4xx_hal_pcd_ex.h>

void CallbackReceived(const char* pMessage)
{
  CcKernel::message(EMessage::Info, pMessage);
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {};
  if(pcdHandle->Instance==USB_OTG_FS)
  {
  /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

  /* USER CODE END USB_OTG_FS_MspInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_FS GPIO Configuration
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

  /* USER CODE END USB_OTG_FS_MspInit 1 */
  }
  else if(pcdHandle->Instance==USB_OTG_HS)
  {
  /* USER CODE BEGIN USB_OTG_HS_MspInit 0 */

  /* USER CODE END USB_OTG_HS_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USB_OTG_HS GPIO Configuration
    PB13     ------> USB_OTG_HS_VBUS
    PB14     ------> USB_OTG_HS_DM
    PB15     ------> USB_OTG_HS_DP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
  /* USER CODE BEGIN USB_OTG_HS_MspInit 1 */

  /* USER CODE END USB_OTG_HS_MspInit 1 */
  }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
  if(pcdHandle->Instance==USB_OTG_FS)
  {
  /* USER CODE BEGIN USB_OTG_FS_MspDeInit 0 */

  /* USER CODE END USB_OTG_FS_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

    /**USB_OTG_FS GPIO Configuration
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);

  /* USER CODE BEGIN USB_OTG_FS_MspDeInit 1 */

  /* USER CODE END USB_OTG_FS_MspDeInit 1 */
  }
  else if(pcdHandle->Instance==USB_OTG_HS)
  {
  /* USER CODE BEGIN USB_OTG_HS_MspDeInit 0 */

  /* USER CODE END USB_OTG_HS_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();

    /**USB_OTG_HS GPIO Configuration
    PB13     ------> USB_OTG_HS_VBUS
    PB14     ------> USB_OTG_HS_DM
    PB15     ------> USB_OTG_HS_DP
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(OTG_HS_IRQn);

  /* USER CODE BEGIN USB_OTG_HS_MspDeInit 1 */

  /* USER CODE END USB_OTG_HS_MspDeInit 1 */
  }
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
  static_cast<STM32F407UsbDevice*>(hpcd->pData)->doOutputData(epnum, hpcd->OUT_ep[epnum].xfer_buff);
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
  static_cast<STM32F407UsbDevice*>(hpcd->pData)->doInputData(epnum);
}
/**
  * @brief  Setup stage callback
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32F407UsbDevice*>(hpcd->pData)->doSetup();
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
  static_cast<STM32F407UsbDevice*>(hpcd->pData)->doReset();
}

/**
  * @brief  Suspend event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32F407UsbDevice*>(hpcd->pData)->setUsbState(STM32F407UsbDevice::EUsbState::Suspended);
}

/**
  * @brief  Resume event callback.
  * @param  hpcd PCD handle
  * @retval None
  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
  static_cast<STM32F407UsbDevice*>(hpcd->pData)->setUsbState(STM32F407UsbDevice::EUsbState::Resume);
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


STM32F407UsbDevice* s_pInstance = nullptr;

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

void STM32F407UsbDevice::ISR()
{
  CcStatic_memsetZeroObject(m_oGlobalDef);
}

STM32F407UsbDevice::STM32F407UsbDevice()
{
  s_pInstance = this;
  CcStatic_memsetZeroObject(m_oGlobalDef);
  CcStatic_memsetZeroObject(m_oConfigDef);
  CcStatic_memsetZeroObject(m_oPcdHandle);
}

STM32F407UsbDevice::~STM32F407UsbDevice()
{
}

CcStatus STM32F407UsbDevice::onState(EState eState)
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

CcStatus STM32F407UsbDevice::loadDeviceDescriptor(const CDeviceDescriptor& oDescriptor)
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

void STM32F407UsbDevice::setUsbState(EUsbState eNewState)
{
  switch(eNewState) 
  {
    case EUsbState::Default:
    break;
    case EUsbState::Addressed:
    break;
    case EUsbState::Configured:
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

void STM32F407UsbDevice::doReset()
{
  // EPTypes:
  //   EP_TYPE_CTRL                           0U
  //   EP_TYPE_ISOC                           1U
  //   EP_TYPE_BULK                           2U
  //   EP_TYPE_INTR                           3U
  //   EP_TYPE_MSK                            3U
  // configure endpoints!
  /* Open EP0 OUT */
  HAL_StatusTypeDef status = HAL_PCD_EP_Open(getPcdHandle(), 0x00, 0x64, EP_TYPE_CTRL);
  /* Open EP0 IN */
  if(status == HAL_OK)
  {
    status = HAL_PCD_EP_Open(getPcdHandle(), 0x80, 0x64, EP_TYPE_CTRL);
  }
}

#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))
#define  USB_REQ_RECIPIENT_DEVICE                       0x00U
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01U
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02U
#define  USB_REQ_RECIPIENT_MASK                         0x03U

void STM32F407UsbDevice::doSetup()
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

void STM32F407UsbDevice::doSetupDevice()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  switch (pRequest->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS:
    case USB_REQ_TYPE_VENDOR:
    default:
      CallbackReceived("USB_REQ_TYPE_CLASS/USB_REQ_TYPE_VENDOR");
      break;

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
              eEp0State = EEnpointState::In;
              HAL_PCD_EP_Transmit(getPcdHandle(), 0, nullptr, 0);

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
        case USB_REQ_GET_CONFIGURATION:
        case USB_REQ_GET_STATUS:
        case USB_REQ_SET_FEATURE:
        case USB_REQ_CLEAR_FEATURE:
        default:
          CallbackReceived("USB_REQ_...");
          break;
      }
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


void STM32F407UsbDevice::doSetupDeviceDescriptor()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  uint8* pBuffer = nullptr;
  uint16 uiSize = 0;

  eEp0State = EEnpointState::Setup;

  switch(pRequest->wValue >> 8)
  {
    case USB_DESC_TYPE_DEVICE:
      pBuffer = reinterpret_cast<uint8*>(&m_oDeviceDescriptor);
      uiSize = m_oDeviceDescriptor.uiLength;
      break;
    case USB_DESC_TYPE_STRING:
      doSetupDeviceDescriptorString();
      break;
    case USB_DESC_TYPE_CONFIGURATION:
      pBuffer = reinterpret_cast<uint8*>(m_oDeviceDescriptor.getConfigs()[0].getConfig());
      uiSize = m_oDeviceDescriptor.getConfigs()[0].getConfig()->uiTotalLength;
      break;
    case USB_DESC_TYPE_DEVICE_QUALIFIER:
    case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
      CallbackReceived("Test");
      break;
  }
  if(pBuffer != nullptr && uiSize > 0)
  {
    write(0, pBuffer, uiSize);
  }
}

void STM32F407UsbDevice::doSetupDeviceDescriptorString()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
  uint8 uiIndex = pRequest->wValue & 0xff;
  if(uiIndex < m_oDeviceDescriptor.getStrings().size())
  {
    const uint8* pBuffer = reinterpret_cast<const uint8*>(&m_oDeviceDescriptor.getStrings()[uiIndex]);
    uint16 uiSize = m_oDeviceDescriptor.getStrings()[uiIndex]->uiLength;
    write(0, pBuffer, uiSize);
  }
  CCUNUSED(uiIndex);
}

void STM32F407UsbDevice::doSetupInterface()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
}

void STM32F407UsbDevice::doSetupEndPoint()
{
  CRequest* pRequest = reinterpret_cast<CRequest*>(m_oPcdHandle.Setup);
}

void STM32F407UsbDevice::doInputData(uint8 uiEndpoint)
{
  if(uiEndpoint == 0)
  {
    if(eEp0State == EEnpointState::In)
    {
      if(!writeContinue())
      {
        stallEp(0x80U);
        // Prepare receive
        eEp0State = EEnpointState::Out;
        HAL_PCD_EP_Receive(getPcdHandle(), uiEndpoint, nullptr, 0);
      }
    }
    else
    {
      // Perpare receive
      HAL_PCD_EP_Receive(getPcdHandle(), 0, nullptr, 0);
    }
  }
  else
  {
    if(writeContinue())
    {

    }
  }
}

void STM32F407UsbDevice::doOutputData(uint8 uiEndpoint, const uint8* pBuffer)
{
  if(uiEndpoint == 0)
  {
    // Check if more data is required
    if(eEp0State == EEnpointState::Out)
    {
      if(0)
        HAL_PCD_EP_Receive(getPcdHandle(), 0, nullptr, 0);
      
      eEp0State = EEnpointState::In;
      HAL_PCD_EP_Transmit(getPcdHandle(), uiEndpoint, nullptr, 0);
    }
    else
    {
      eEp0State = EEnpointState::Idle;
      // Perpare receive
      stallEp(0U);
    }
  }
}


void STM32F407UsbDevice::write(uint8 uiEndpoint, const uint8* pBuffer, uint16 uiSize)
{
  m_uiWriteEndpoint = uiEndpoint;
  m_pWriteBuffer = const_cast<uint8*>(pBuffer);
  m_uiWriteSize = uiSize;
  m_uiWritten = 0;
  writeContinue();
}

bool STM32F407UsbDevice::writeContinue()
{
  bool bDataWritten = false;
  if(m_uiWriteSize > m_uiWritten)
  {
    uint16 uiTransferSize = CCMIN(m_uiWriteSize - m_uiWritten, 64);
    if(HAL_OK == HAL_PCD_EP_Transmit(getPcdHandle(), m_uiWriteEndpoint, m_pWriteBuffer + m_uiWritten, uiTransferSize))
    {
      m_uiWritten += uiTransferSize;
      bDataWritten = true;
    }
  }
  if(m_uiWriteSize >= m_uiWritten)
  {
    m_uiWriteEndpoint = 0;
    m_pWriteBuffer = nullptr;
    m_uiWriteSize = 0;
    m_uiWritten = 0;
    eEp0State = EEnpointState::In;
  }
  return bDataWritten;
}

void STM32F407UsbDevice::stallEp(uint8 uiEndpoint)
{
  HAL_PCD_EP_SetStall(getPcdHandle(), uiEndpoint);
}