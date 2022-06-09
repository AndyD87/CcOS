
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

STM32F407UsbDevice* s_pInstance = nullptr;

/**
 * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
 */
CCEXTERNC void OTG_HS_IRQHandler(void)
{
  if(s_pInstance)
	  s_pInstance->ISR();
}

/**
 * @brief  This function handles USB-On-The-Go HS global interrupt request.
 */
CCEXTERNC void OTG_FS_IRQHandler(void)
{
  if(s_pInstance)
    s_pInstance->ISR();
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
  CcStatus bSuccess = false;
  switch(eState)
  {
    case EState::Start:
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

      if(HAL_OK == HAL_PCD_Init(&m_oPcdHandle))
      {
        if(HAL_OK == USB_EnableGlobalInt(&m_oGlobalDef))
        {
          if(HAL_OK == HAL_PCD_Start(&m_oPcdHandle))
          {
            bSuccess = true;
          }
          else
          {
            USB_DisableGlobalInt(&m_oGlobalDef);
            HAL_PCD_MspDeInit(&m_oPcdHandle);
          }
        }
      }
      break;
    case EState::Stop:
      USB_DisableGlobalInt(&m_oGlobalDef);
      HAL_PCD_MspDeInit(&m_oPcdHandle);
      break;
    default:
      bSuccess = EStatus::NotSupported;
  }
  return bSuccess;
}

CcStatus STM32F407UsbDevice::loadDeviceDescriptor(const CDeviceDescriptor& oDescriptor)
{
  CcStatus oStatus = EStatus::CommandInvalidParameter;
  
  return oStatus;
}