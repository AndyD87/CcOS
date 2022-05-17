
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

#include "STM32F407Usb.h"
#include "CcKernel.h"
#include "CcDevice.h"

/**
  ******************************************************************************
  * @file    STM32F407Usb.cpp
  * @author  MCD Application Team
  * @brief   USB Host configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_hid.h"
#include "Devices/IGpioPort.h"
#include "CcConsole.h"

HCD_HandleTypeDef STM32F407Usb::s_hHcd;
IGpioPin* STM32F407Usb::s_pUsbPowerPin = nullptr;

/**
 * @brief Debug print for USB implementations
 * @param pBuff: String to print
 */
void print_here(const char* pBuff = "")
{
  CcConsole::writeLine(pBuff);
}

/**
 * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
 */
CCEXTERNC void OTG_HS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&STM32F407Usb::s_hHcd);
}

/**
 * @brief  This function handles USB-On-The-Go HS global interrupt request.
 */
CCEXTERNC void OTG_FS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&STM32F407Usb::s_hHcd);
}

/*******************************************************************************
                       HCD BSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
CCEXTERNC void HAL_HCD_MspInit(HCD_HandleTypeDef* hhcd)
{
  print_here();
#if defined(STM32F407VET) || defined(STM32F407VGT)
  CcHandle<IGpioPort> pPortA = CcKernel::getDevice(EDeviceType::GpioPort, 0).getDevice<IGpioPort>();
  CcHandle<IGpioPort> pPortC = CcKernel::getDevice(EDeviceType::GpioPort, 2).getDevice<IGpioPort>();
  if( pPortA.isValid() &&
      pPortC.isValid())
  {
    uint32 uiPin = (1<<11)|(1<<12);
    #ifdef STM32F407VGT
      uiPin |= (1<<10);
    #endif

    // Enable USB PINS        ID      DM      DP
    pPortA->setPinsDirection(uiPin, IGpioPin::EDirection::Alternate, GPIO_AF10_OTG_HS);

    #ifdef STM32F407VGT
      IGpioPin* pUsbVbus = pPortA->getPin(9);
      if(pUsbVbus) pUsbVbus->setDirection(IGpioPin::EDirection::Input);

      // Enable USB POWER       PIN
      STM32F407Usb::s_pUsbPowerPin = pPortC->getPin(0);
      if(STM32F407Usb::s_pUsbPowerPin) STM32F407Usb::s_pUsbPowerPin->setDirection(IGpioPin::EDirection::Output);
      if(STM32F407Usb::s_pUsbPowerPin) STM32F407Usb::s_pUsbPowerPin->setValue(true);
      if(STM32F407Usb::s_pUsbPowerPin) STM32F407Usb::s_pUsbPowerPin->setValue(false);
    #endif
  }

  CCUNUSED(hhcd);
  /* Enable USB HS Clocks */
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

  /* Set USBFS Interrupt priority */
  HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);

  /* Enable USBFS Interrupt */
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
#else
  #error "USB not configured for current device"
#endif
}

/**
  * @brief  DeInitializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
CCEXTERNC void HAL_HCD_MspDeInit(HCD_HandleTypeDef* hhcd)
{
  print_here();
  CCUNUSED(hhcd);
  /* Disable USB FS Clocks */
  __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
}

/*******************************************************************************
                       LL Driver Callbacks (HCD -> USB Host Library)
*******************************************************************************/

/**
  * @brief  SOF callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
CCEXTERNC void HAL_HCD_SOF_Callback(HCD_HandleTypeDef* hhcd)
{
  print_here();
  USBH_LL_IncTimer (static_cast<USBH_HandleTypeDef*>(hhcd->pData));
}

/**
 * @brief Callback an client connection
 * @param hhcd: Handle of client data
 */
CCEXTERNC void HAL_HCD_Connect_Callback(HCD_HandleTypeDef* hhcd)
{
  print_here();
  USBH_LL_Connect(static_cast<USBH_HandleTypeDef*>(hhcd->pData));
}

/**
 * @brief Callback an client disconnection
 * @param hhcd: Handle of client data
 */
CCEXTERNC void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef* hhcd)
{
  print_here();
  USBH_LL_Disconnect(static_cast<USBH_HandleTypeDef*>(hhcd->pData));
}

/**
 * @brief Callback an client port was enabled
 * @param hhcd: Handle of client data
 */
CCEXTERNC void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef* hhcd)
{
  print_here();
  USBH_LL_PortEnabled(static_cast<USBH_HandleTypeDef*>(hhcd->pData));
}

/**
 * @brief Callback an client port was disabled
 * @param hhcd: Handle of client data
 */
CCEXTERNC void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef* hhcd)
{
  print_here();
  USBH_LL_PortDisabled(static_cast<USBH_HandleTypeDef*>(hhcd->pData));
}

/**
  * @brief  Notify URB state change callback.
  * @param  hhcd: HCD handle
  * @param  chnum: Channel number
  * @param  urb_state: URB State
  * @retval None
  */
CCEXTERNC void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef* hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
  print_here();
  CCUNUSED(hhcd);
  CCUNUSED(chnum);
  CCUNUSED(urb_state);
  /* To be used with OS to sync URB state with the global state machine */
}

/*******************************************************************************
                       LL Driver Interface (USB Host Library --> HCD)
*******************************************************************************/
/**
  * @brief  USBH_LL_Init
  *         Initialize the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef* phost)
{
  print_here();
  /* Set the LL Driver parameters */
  CcStatic_memsetZeroObject(STM32F407Usb::s_hHcd);
  STM32F407Usb::s_hHcd.Instance = USB_OTG_FS;
  STM32F407Usb::s_hHcd.Init.Host_channels = 11;
  STM32F407Usb::s_hHcd.Init.dma_enable = 0;
  STM32F407Usb::s_hHcd.Init.low_power_enable = 0;
  STM32F407Usb::s_hHcd.Init.phy_itface = HCD_PHY_EMBEDDED;
  STM32F407Usb::s_hHcd.Init.Sof_enable = 0;
  STM32F407Usb::s_hHcd.Init.speed = HCD_SPEED_HIGH;
  STM32F407Usb::s_hHcd.Init.vbus_sensing_enable = 0;
  STM32F407Usb::s_hHcd.Init.lpm_enable = 0;

  /* Link the driver to the stack */
  STM32F407Usb::s_hHcd.pData = phost;
  phost->pData = &STM32F407Usb::s_hHcd;

  /* Initialize the LL Driver */
  USBH_StatusTypeDef uStatus = (USBH_StatusTypeDef)HAL_HCD_Init(&STM32F407Usb::s_hHcd);

  if(uStatus == USBH_OK)
    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&STM32F407Usb::s_hHcd));

  return uStatus;
}

/**
  * @brief  De-Initializes the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
  print_here();
  HAL_HCD_DeInit(static_cast<HCD_HandleTypeDef*>(phost->pData));
  return USBH_OK;
}

/**
  * @brief  Starts the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
  print_here();
  HAL_HCD_Start(static_cast<HCD_HandleTypeDef*>(phost->pData));
  return USBH_OK;
}

/**
  * @brief  Stops the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
  print_here();
  HAL_HCD_Stop(static_cast<HCD_HandleTypeDef*>(phost->pData));
  return USBH_OK;
}

/**
  * @brief  Returns the USB Host Speed from the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Speeds
  */
CCEXTERNC USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
  print_here();
  USBH_SpeedTypeDef speed = USBH_SPEED_HIGH;

  switch (HAL_HCD_GetCurrentSpeed(static_cast<HCD_HandleTypeDef*>(phost->pData)))
  {
  case 0:
    speed = USBH_SPEED_HIGH;
    break;

  case 1:
    speed = USBH_SPEED_FULL;
    break;

  case 2:
    speed = USBH_SPEED_LOW;
    break;

  default:
    speed = USBH_SPEED_HIGH;
    break;
  }
  return speed;
}

/**
  * @brief  Resets the Host Port of the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_ResetPort (USBH_HandleTypeDef *phost)
{
  print_here();
  HAL_HCD_ResetPort(static_cast<HCD_HandleTypeDef*>(phost->pData));
  return USBH_OK;
}

/**
  * @brief  Returns the last transferred packet size.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval Packet Size
  */
CCEXTERNC uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  print_here();
  return HAL_HCD_HC_GetXferCount(static_cast<HCD_HandleTypeDef*>(phost->pData), pipe);
}

/**
  * @brief  Opens a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  epnum: Endpoint Number
  * @param  dev_address: Device USB address
  * @param  speed: Device Speed
  * @param  ep_type: Endpoint Type
  * @param  mps: Endpoint Max Packet Size
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost,
                                    uint8_t pipe,
                                    uint8_t epnum,
                                    uint8_t dev_address,
                                    uint8_t speed,
                                    uint8_t ep_type,
                                    uint16_t mps)
{
  print_here();
  HAL_HCD_HC_Init(static_cast<HCD_HandleTypeDef*>(phost->pData),
                  pipe,
                  epnum,
                  dev_address,
                  speed,
                  ep_type,
                  mps);
  return USBH_OK;
}

/**
  * @brief  Closes a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  print_here();
  HAL_HCD_HC_Halt(static_cast<HCD_HandleTypeDef*>(phost->pData), pipe);
  return USBH_OK;
}

/**
  * @brief  Submits a new URB to the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @param  direction: Channel number
  *          This parameter can be one of these values:
  *           0: Output
  *           1: Input
  * @param  ep_type: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg EP_TYPE_CTRL: Control type
  *            @arg EP_TYPE_ISOC: Isochronous type
  *            @arg EP_TYPE_BULK: Bulk type
  *            @arg EP_TYPE_INTR: Interrupt type
  * @param  token: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg 0: PID_SETUP
  *            @arg 1: PID_DATA
  * @param  pbuff: pointer to URB data
  * @param  length: length of URB data
  * @param  do_ping: activate do ping protocol (for high speed only)
  *          This parameter can be one of these values:
  *           0: do ping inactive
  *           1: do ping active
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost,
                                     uint8_t pipe,
                                     uint8_t direction,
                                     uint8_t ep_type,
                                     uint8_t token,
                                     uint8_t* pbuff,
                                     uint16_t length,
                                     uint8_t do_ping)
{
  print_here();
  HAL_HCD_HC_SubmitRequest(static_cast<HCD_HandleTypeDef*>(phost->pData),
                           pipe,
                           direction,
                           ep_type,
                           token,
                           pbuff,
                           length,
                           do_ping);
  return USBH_OK;
}

/**
  * @brief  Gets a URB state from the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @retval URB state
  *          This parameter can be one of these values:
  *            @arg URB_IDLE
  *            @arg URB_DONE
  *            @arg URB_NOTREADY
  *            @arg URB_NYET
  *            @arg URB_ERROR
  *            @arg URB_STALL
  */
CCEXTERNC USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  print_here();
  return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState (static_cast<HCD_HandleTypeDef*>(phost->pData), pipe);
}

/**
  * @brief  Drives VBUS.
  * @param  phost: Host handle
  * @param  state: VBUS state
  *          This parameter can be one of these values:
  *           0: VBUS Active
  *           1: VBUS Inactive
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
  print_here();
  CCUNUSED(phost);
  if(state == 0)
  {
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
  }
  HAL_Delay(200);
  return USBH_OK;
}

/**
  * @brief  Sets toggle for a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  toggle: toggle (0/1)
  * @retval USBH Status
  */
CCEXTERNC USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
  print_here();
  CCUNUSED(phost);
  if(STM32F407Usb::s_hHcd.hc[pipe].ep_is_in)
  {
    STM32F407Usb::s_hHcd.hc[pipe].toggle_in = toggle;
  }
  else
  {
    STM32F407Usb::s_hHcd.hc[pipe].toggle_out = toggle;
  }
  return USBH_OK;
}

/**
  * @brief  Returns the current toggle of a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval toggle (0/1)
  */
CCEXTERNC uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  print_here();
  CCUNUSED(phost);
  uint8_t toggle = 0;

  if(STM32F407Usb::s_hHcd.hc[pipe].ep_is_in)
  {
    toggle = STM32F407Usb::s_hHcd.hc[pipe].toggle_in;
  }
  else
  {
    toggle = STM32F407Usb::s_hHcd.hc[pipe].toggle_out;
  }
  return toggle;
}

/**
  * @brief  Delay routine for the USB Host Library
  * @param  Delay: Delay in ms
  * @retval None
  */
CCEXTERNC void USBH_Delay(uint32_t Delay)
{
  CcKernel::delayMs(Delay);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
CCEXTERNC  void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  print_here();
  CCUNUSED(phost);
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    //Appli_state = APPLICATION_DISCONNECT;
    break;

  case HOST_USER_CLASS_ACTIVE:
    //Appli_state = APPLICATION_READY;
    break;

  case HOST_USER_CONNECTION:
    //Appli_state = APPLICATION_START;
    break;

  default:
    break;
  }
}

#include "STM32F407Usb.h"
#include "CcStatic.h"
//#include <usbh_hid.h>

STM32F407Usb::STM32F407Usb()
{
  m_eType = EType::Host;
  CcStatic_memsetZeroObject(m_oData);
  registerIdle();
}

STM32F407Usb::~STM32F407Usb()
{
  deregisterIdle();
}

CcStatus STM32F407Usb::onState(EState eState)
{
  CcStatus oStatus;
  switch (eState)
  {
    case EState::Start:
      switch (m_eType)
      {
        case EType::Device:
          oStatus = HAL_OK == HAL_PCD_Init(&m_oData.oPcd);
          break;
        case EType::Host:
          oStatus = false;
          /* Init Host Library */
          if(USBH_OK == USBH_Init(&hUSBHost, USBH_UserProcess, 0))
          {
            /* Add Supported Class */
            if(USBH_OK == USBH_RegisterClass(&hUSBHost, USBH_HID_CLASS))
            {
              /* Start Host Process */
              if(USBH_OK == USBH_Start(&hUSBHost))
              {
                oStatus = true;
              }
            }
          }
          break;
        default:
          oStatus = false;
          break;
      }
      break;
    case EState::Stop:
      switch (m_eType)
      {
        case EType::Device:
          oStatus = HAL_OK == HAL_PCD_DeInit(&m_oData.oPcd);
          break;
        case EType::Host:
          oStatus = HAL_OK == HAL_HCD_DeInit(&m_oData.oHcd);
          break;
        default:
          oStatus = false;
          break;
      }
      break;
    default:
      break;
  }
  return oStatus;
}

bool STM32F407Usb::setType(EType eType)
{
  bool bSuccess = true;
  stop();
  switch(eType)
  {
    case EType::Device:
      m_eType = EType::Device;
      break;
    case EType::Host:
      m_eType = EType::Host;
      break;
    default:
      bSuccess = false;
  }
  return bSuccess;
}

STM32F407Usb::EType STM32F407Usb::getType()
{
  return EType::Device;
}

void STM32F407Usb::idle()
{
  USBH_Process(static_cast<USBH_HandleTypeDef*>(STM32F407Usb::s_hHcd.pData));
}
