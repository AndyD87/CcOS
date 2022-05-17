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
 * @brief     Implementation of class STM32F407SystemTimer
 **/
#include <CcBase.h>
#include <Devices/ICpu.h>

//! Storage for unimplemented irq handler id
size_t g_uiFailedHandler = 0x00;

//! @brief method to call from unimplemented irq handler
CCEXTERNC void Crashed( void )
{
  g_uiFailedHandler = 0;
  // goto endless loop
  ICpu::CreateThreadMethod(nullptr);
}

//! Forward not implemented Handler and set crashed code
CCEXTERNC void NMI_Handler                  (){g_uiFailedHandler = 0x01;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void HardFault_Handler            (){g_uiFailedHandler = 0x02;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void MemManage_Handler            (){g_uiFailedHandler = 0x03;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void BusFault_Handler             (){g_uiFailedHandler = 0x04;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void UsageFault_Handler           (){g_uiFailedHandler = 0x05;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SVC_Handler                  (){g_uiFailedHandler = 0x06;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DebugMon_Handler             (){g_uiFailedHandler = 0x07;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void PendSV_Handler               (){g_uiFailedHandler = 0x08;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void WWDG_IRQHandler              (){g_uiFailedHandler = 0x09;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void PVD_IRQHandler               (){g_uiFailedHandler = 0x10;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TAMP_STAMP_IRQHandler        (){g_uiFailedHandler = 0x11;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void RTC_WKUP_IRQHandler          (){g_uiFailedHandler = 0x12;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void FLASH_IRQHandler             (){g_uiFailedHandler = 0x13;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void RCC_IRQHandler               (){g_uiFailedHandler = 0x14;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI0_IRQHandler             (){g_uiFailedHandler = 0x15;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI1_IRQHandler             (){g_uiFailedHandler = 0x16;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI2_IRQHandler             (){g_uiFailedHandler = 0x17;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI3_IRQHandler             (){g_uiFailedHandler = 0x18;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI4_IRQHandler             (){g_uiFailedHandler = 0x19;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream0_IRQHandler      (){g_uiFailedHandler = 0x20;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream1_IRQHandler      (){g_uiFailedHandler = 0x21;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream2_IRQHandler      (){g_uiFailedHandler = 0x22;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream3_IRQHandler      (){g_uiFailedHandler = 0x23;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream4_IRQHandler      (){g_uiFailedHandler = 0x24;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream5_IRQHandler      (){g_uiFailedHandler = 0x25;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream6_IRQHandler      (){g_uiFailedHandler = 0x26;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void ADC_IRQHandler               (){g_uiFailedHandler = 0x27;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_TX_IRQHandler           (){g_uiFailedHandler = 0x28;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_RX0_IRQHandler          (){g_uiFailedHandler = 0x29;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_RX1_IRQHandler          (){g_uiFailedHandler = 0x30;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_SCE_IRQHandler          (){g_uiFailedHandler = 0x31;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI9_5_IRQHandler           (){g_uiFailedHandler = 0x32;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_BRK_TIM9_IRQHandler     (){g_uiFailedHandler = 0x33;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_UP_TIM10_IRQHandler     (){g_uiFailedHandler = 0x34;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_TRG_COM_TIM11_IRQHandler(){g_uiFailedHandler = 0x35;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_CC_IRQHandler           (){g_uiFailedHandler = 0x36;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM3_IRQHandler              (){g_uiFailedHandler = 0x37;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM4_IRQHandler              (){g_uiFailedHandler = 0x38;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C1_EV_IRQHandler           (){g_uiFailedHandler = 0x39;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C1_ER_IRQHandler           (){g_uiFailedHandler = 0x40;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C2_EV_IRQHandler           (){g_uiFailedHandler = 0x41;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C2_ER_IRQHandler           (){g_uiFailedHandler = 0x42;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SPI1_IRQHandler              (){g_uiFailedHandler = 0x43;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SPI2_IRQHandler              (){g_uiFailedHandler = 0x44;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART1_IRQHandler            (){g_uiFailedHandler = 0x45;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART3_IRQHandler            (){g_uiFailedHandler = 0x47;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI15_10_IRQHandler         (){g_uiFailedHandler = 0x48;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void RTC_Alarm_IRQHandler         (){g_uiFailedHandler = 0x49;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_FS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x50;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_BRK_TIM12_IRQHandler    (){g_uiFailedHandler = 0x51;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_UP_TIM13_IRQHandler     (){g_uiFailedHandler = 0x52;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_TRG_COM_TIM14_IRQHandler(){g_uiFailedHandler = 0x53;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_CC_IRQHandler           (){g_uiFailedHandler = 0x54;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream7_IRQHandler      (){g_uiFailedHandler = 0x55;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void FSMC_IRQHandler              (){g_uiFailedHandler = 0x56;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SDIO_IRQHandler              (){g_uiFailedHandler = 0x57;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM5_IRQHandler              (){g_uiFailedHandler = 0x58;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SPI3_IRQHandler              (){g_uiFailedHandler = 0x59;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void UART4_IRQHandler             (){g_uiFailedHandler = 0x60;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void UART5_IRQHandler             (){g_uiFailedHandler = 0x61;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM6_DAC_IRQHandler          (){g_uiFailedHandler = 0x62;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM7_IRQHandler              (){g_uiFailedHandler = 0x63;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream0_IRQHandler      (){g_uiFailedHandler = 0x64;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream1_IRQHandler      (){g_uiFailedHandler = 0x65;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream2_IRQHandler      (){g_uiFailedHandler = 0x66;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream3_IRQHandler      (){g_uiFailedHandler = 0x67;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream4_IRQHandler      (){g_uiFailedHandler = 0x68;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void ETH_WKUP_IRQHandler          (){g_uiFailedHandler = 0x70;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_TX_IRQHandler           (){g_uiFailedHandler = 0x71;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_RX0_IRQHandler          (){g_uiFailedHandler = 0x72;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_RX1_IRQHandler          (){g_uiFailedHandler = 0x73;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_SCE_IRQHandler          (){g_uiFailedHandler = 0x74;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_FS_IRQHandler            (){g_uiFailedHandler = 0x75;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream5_IRQHandler      (){g_uiFailedHandler = 0x76;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream6_IRQHandler      (){g_uiFailedHandler = 0x77;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream7_IRQHandler      (){g_uiFailedHandler = 0x78;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART6_IRQHandler            (){g_uiFailedHandler = 0x79;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C3_EV_IRQHandler           (){g_uiFailedHandler = 0x80;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C3_ER_IRQHandler           (){g_uiFailedHandler = 0x81;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_EP1_OUT_IRQHandler    (){g_uiFailedHandler = 0x82;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_EP1_IN_IRQHandler     (){g_uiFailedHandler = 0x83;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x84;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_IRQHandler            (){g_uiFailedHandler = 0x85;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DCMI_IRQHandler              (){g_uiFailedHandler = 0x86;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void HASH_RNG_IRQHandler          (){g_uiFailedHandler = 0x87;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void FPU_IRQHandler               (){g_uiFailedHandler = 0x88;Crashed();}
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CCOS_DEFAULT_IRQHandler      (){g_uiFailedHandler = 0xff;Crashed();}

//! Forward not implemented Handler and set crashed code
