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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class STM32F407Usb
 **/
#include <STM32F407Usb.h>
#include "CcKernel.h"
#include "CcStatic.h"
#include <stm32f4xx_hal.h>
#include <STM32F407Driver.h>

STM32F407Usb::STM32F407Usb()
{
  m_eType = EType::Host;
  CcStatic_memsetZeroObject(m_oData);
}

STM32F407Usb::~STM32F407Usb()
{
}

CcStatus STM32F407Usb::setState(EState eState)
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
          /* Set the LL Driver parameters */
          m_oData.oHcd.Instance = USB_OTG_FS;
          m_oData.oHcd.Init.Host_channels = 11;
          m_oData.oHcd.Init.dma_enable = 0;
          m_oData.oHcd.Init.low_power_enable = 0;
          m_oData.oHcd.Init.phy_itface = HCD_PHY_EMBEDDED;
          m_oData.oHcd.Init.Sof_enable = 0;
          m_oData.oHcd.Init.speed = HCD_SPEED_FULL;
          m_oData.oHcd.Init.vbus_sensing_enable = 0;
          m_oData.oHcd.Init.lpm_enable = 0;

          /* Link the driver to the stack */
          //m_oData.oHcd.pData = phost;
          //phost->pData = &hhcd;
          //
          ///* Initialize the LL Driver */
          oStatus = HAL_OK == HAL_HCD_Init(&m_oData.oHcd);
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
      oStatus = EStatus::CommandUnknownParameter;
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
