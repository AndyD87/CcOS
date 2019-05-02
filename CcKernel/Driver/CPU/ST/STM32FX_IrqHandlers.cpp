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
 * @brief     Implementation of class STM32F407VSystemTimer
 **/
#include <CcBase.h>
#include <Devices/ICpu.h>

size_t g_uiFailedHandler = 0x00;
CCEXTERNC void Crashed( void )
{
  g_uiFailedHandler = 0;
  // goto endless loop
  ICpu::CreateThread(nullptr);
}
CCEXTERNC void NMI_Handler                  (){g_uiFailedHandler = 0x01;Crashed();}
CCEXTERNC void HardFault_Handler            (){g_uiFailedHandler = 0x02;Crashed();}
CCEXTERNC void MemManage_Handler            (){g_uiFailedHandler = 0x03;Crashed();}
CCEXTERNC void BusFault_Handler             (){g_uiFailedHandler = 0x04;Crashed();}
CCEXTERNC void UsageFault_Handler           (){g_uiFailedHandler = 0x05;Crashed();}
CCEXTERNC void SVC_Handler                  (){g_uiFailedHandler = 0x06;Crashed();}
CCEXTERNC void DebugMon_Handler             (){g_uiFailedHandler = 0x07;Crashed();}
CCEXTERNC void PendSV_Handler               (){g_uiFailedHandler = 0x08;Crashed();}
CCEXTERNC void WWDG_IRQHandler              (){g_uiFailedHandler = 0x09;Crashed();}
CCEXTERNC void PVD_IRQHandler               (){g_uiFailedHandler = 0x10;Crashed();}
CCEXTERNC void TAMP_STAMP_IRQHandler        (){g_uiFailedHandler = 0x11;Crashed();}
CCEXTERNC void RTC_WKUP_IRQHandler          (){g_uiFailedHandler = 0x12;Crashed();}
CCEXTERNC void FLASH_IRQHandler             (){g_uiFailedHandler = 0x13;Crashed();}
CCEXTERNC void RCC_IRQHandler               (){g_uiFailedHandler = 0x14;Crashed();}
CCEXTERNC void EXTI0_IRQHandler             (){g_uiFailedHandler = 0x15;Crashed();}
CCEXTERNC void EXTI1_IRQHandler             (){g_uiFailedHandler = 0x16;Crashed();}
CCEXTERNC void EXTI2_IRQHandler             (){g_uiFailedHandler = 0x17;Crashed();}
CCEXTERNC void EXTI3_IRQHandler             (){g_uiFailedHandler = 0x18;Crashed();}
CCEXTERNC void EXTI4_IRQHandler             (){g_uiFailedHandler = 0x19;Crashed();}
CCEXTERNC void DMA1_Stream0_IRQHandler      (){g_uiFailedHandler = 0x20;Crashed();}
CCEXTERNC void DMA1_Stream1_IRQHandler      (){g_uiFailedHandler = 0x21;Crashed();}
CCEXTERNC void DMA1_Stream2_IRQHandler      (){g_uiFailedHandler = 0x22;Crashed();}
CCEXTERNC void DMA1_Stream3_IRQHandler      (){g_uiFailedHandler = 0x23;Crashed();}
CCEXTERNC void DMA1_Stream4_IRQHandler      (){g_uiFailedHandler = 0x24;Crashed();}
CCEXTERNC void DMA1_Stream5_IRQHandler      (){g_uiFailedHandler = 0x25;Crashed();}
CCEXTERNC void DMA1_Stream6_IRQHandler      (){g_uiFailedHandler = 0x26;Crashed();}
CCEXTERNC void ADC_IRQHandler               (){g_uiFailedHandler = 0x27;Crashed();}
CCEXTERNC void CAN1_TX_IRQHandler           (){g_uiFailedHandler = 0x28;Crashed();}
CCEXTERNC void CAN1_RX0_IRQHandler          (){g_uiFailedHandler = 0x29;Crashed();}
CCEXTERNC void CAN1_RX1_IRQHandler          (){g_uiFailedHandler = 0x30;Crashed();}
CCEXTERNC void CAN1_SCE_IRQHandler          (){g_uiFailedHandler = 0x31;Crashed();}
CCEXTERNC void EXTI9_5_IRQHandler           (){g_uiFailedHandler = 0x32;Crashed();}
CCEXTERNC void TIM1_BRK_TIM9_IRQHandler     (){g_uiFailedHandler = 0x33;Crashed();}
CCEXTERNC void TIM1_UP_TIM10_IRQHandler     (){g_uiFailedHandler = 0x34;Crashed();}
CCEXTERNC void TIM1_TRG_COM_TIM11_IRQHandler(){g_uiFailedHandler = 0x35;Crashed();}
CCEXTERNC void TIM1_CC_IRQHandler           (){g_uiFailedHandler = 0x36;Crashed();}
CCEXTERNC void TIM3_IRQHandler              (){g_uiFailedHandler = 0x37;Crashed();}
CCEXTERNC void TIM4_IRQHandler              (){g_uiFailedHandler = 0x38;Crashed();}
CCEXTERNC void I2C1_EV_IRQHandler           (){g_uiFailedHandler = 0x39;Crashed();}
CCEXTERNC void I2C1_ER_IRQHandler           (){g_uiFailedHandler = 0x40;Crashed();}
CCEXTERNC void I2C2_EV_IRQHandler           (){g_uiFailedHandler = 0x41;Crashed();}
CCEXTERNC void I2C2_ER_IRQHandler           (){g_uiFailedHandler = 0x42;Crashed();}
CCEXTERNC void SPI1_IRQHandler              (){g_uiFailedHandler = 0x43;Crashed();}
CCEXTERNC void SPI2_IRQHandler              (){g_uiFailedHandler = 0x44;Crashed();}
CCEXTERNC void USART1_IRQHandler            (){g_uiFailedHandler = 0x45;Crashed();}
CCEXTERNC void USART2_IRQHandler            (){g_uiFailedHandler = 0x46;Crashed();}
CCEXTERNC void USART3_IRQHandler            (){g_uiFailedHandler = 0x47;Crashed();}
CCEXTERNC void EXTI15_10_IRQHandler         (){g_uiFailedHandler = 0x48;Crashed();}
CCEXTERNC void RTC_Alarm_IRQHandler         (){g_uiFailedHandler = 0x49;Crashed();}
CCEXTERNC void OTG_FS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x50;Crashed();}
CCEXTERNC void TIM8_BRK_TIM12_IRQHandler    (){g_uiFailedHandler = 0x51;Crashed();}
CCEXTERNC void TIM8_UP_TIM13_IRQHandler     (){g_uiFailedHandler = 0x52;Crashed();}
CCEXTERNC void TIM8_TRG_COM_TIM14_IRQHandler(){g_uiFailedHandler = 0x53;Crashed();}
CCEXTERNC void TIM8_CC_IRQHandler           (){g_uiFailedHandler = 0x54;Crashed();}
CCEXTERNC void DMA1_Stream7_IRQHandler      (){g_uiFailedHandler = 0x55;Crashed();}
CCEXTERNC void FSMC_IRQHandler              (){g_uiFailedHandler = 0x56;Crashed();}
CCEXTERNC void SDIO_IRQHandler              (){g_uiFailedHandler = 0x57;Crashed();}
CCEXTERNC void TIM5_IRQHandler              (){g_uiFailedHandler = 0x58;Crashed();}
CCEXTERNC void SPI3_IRQHandler              (){g_uiFailedHandler = 0x59;Crashed();}
CCEXTERNC void UART4_IRQHandler             (){g_uiFailedHandler = 0x60;Crashed();}
CCEXTERNC void UART5_IRQHandler             (){g_uiFailedHandler = 0x61;Crashed();}
CCEXTERNC void TIM6_DAC_IRQHandler          (){g_uiFailedHandler = 0x62;Crashed();}
CCEXTERNC void TIM7_IRQHandler              (){g_uiFailedHandler = 0x63;Crashed();}
CCEXTERNC void DMA2_Stream0_IRQHandler      (){g_uiFailedHandler = 0x64;Crashed();}
CCEXTERNC void DMA2_Stream1_IRQHandler      (){g_uiFailedHandler = 0x65;Crashed();}
CCEXTERNC void DMA2_Stream2_IRQHandler      (){g_uiFailedHandler = 0x66;Crashed();}
CCEXTERNC void DMA2_Stream3_IRQHandler      (){g_uiFailedHandler = 0x67;Crashed();}
CCEXTERNC void DMA2_Stream4_IRQHandler      (){g_uiFailedHandler = 0x68;Crashed();}
CCEXTERNC void ETH_WKUP_IRQHandler          (){g_uiFailedHandler = 0x70;Crashed();}
CCEXTERNC void CAN2_TX_IRQHandler           (){g_uiFailedHandler = 0x71;Crashed();}
CCEXTERNC void CAN2_RX0_IRQHandler          (){g_uiFailedHandler = 0x72;Crashed();}
CCEXTERNC void CAN2_RX1_IRQHandler          (){g_uiFailedHandler = 0x73;Crashed();}
CCEXTERNC void CAN2_SCE_IRQHandler          (){g_uiFailedHandler = 0x74;Crashed();}
CCEXTERNC void OTG_FS_IRQHandler            (){g_uiFailedHandler = 0x75;Crashed();}
CCEXTERNC void DMA2_Stream5_IRQHandler      (){g_uiFailedHandler = 0x76;Crashed();}
CCEXTERNC void DMA2_Stream6_IRQHandler      (){g_uiFailedHandler = 0x77;Crashed();}
CCEXTERNC void DMA2_Stream7_IRQHandler      (){g_uiFailedHandler = 0x78;Crashed();}
CCEXTERNC void USART6_IRQHandler            (){g_uiFailedHandler = 0x79;Crashed();}
CCEXTERNC void I2C3_EV_IRQHandler           (){g_uiFailedHandler = 0x80;Crashed();}
CCEXTERNC void I2C3_ER_IRQHandler           (){g_uiFailedHandler = 0x81;Crashed();}
CCEXTERNC void OTG_HS_EP1_OUT_IRQHandler    (){g_uiFailedHandler = 0x82;Crashed();}
CCEXTERNC void OTG_HS_EP1_IN_IRQHandler     (){g_uiFailedHandler = 0x83;Crashed();}
CCEXTERNC void OTG_HS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x84;Crashed();}
CCEXTERNC void OTG_HS_IRQHandler            (){g_uiFailedHandler = 0x85;Crashed();}
CCEXTERNC void DCMI_IRQHandler              (){g_uiFailedHandler = 0x86;Crashed();}
CCEXTERNC void HASH_RNG_IRQHandler          (){g_uiFailedHandler = 0x87;Crashed();}
CCEXTERNC void FPU_IRQHandler               (){g_uiFailedHandler = 0x88;Crashed();}
