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
#include <stm32f4xx_hal.h>

CCEXTERNC void CreateThread(void*);

size_t g_uiFailedHandler = 0x00;
CCEXTERNC void STM32F407VCpu_SysTick( void );
CCEXTERNC void Crashedh( void )
{
  g_uiFailedHandler = 0;
  CreateThread(nullptr);
}
CCEXTERNC void NMI_Handler                  (){g_uiFailedHandler = 0x01;Crashedh();}
CCEXTERNC void HardFault_Handler            (){g_uiFailedHandler = 0x02;Crashedh();}
CCEXTERNC void MemManage_Handler            (){g_uiFailedHandler = 0x03;Crashedh();}
CCEXTERNC void BusFault_Handler             (){g_uiFailedHandler = 0x04;Crashedh();}
CCEXTERNC void UsageFault_Handler           (){g_uiFailedHandler = 0x05;Crashedh();}
CCEXTERNC void SVC_Handler                  (){g_uiFailedHandler = 0x06;Crashedh();}
CCEXTERNC void DebugMon_Handler             (){g_uiFailedHandler = 0x07;Crashedh();}
CCEXTERNC void PendSV_Handler               (){g_uiFailedHandler = 0x08;Crashedh();}
CCEXTERNC void WWDG_IRQHandler              (){g_uiFailedHandler = 0x09;Crashedh();}
CCEXTERNC void PVD_IRQHandler               (){g_uiFailedHandler = 0x10;Crashedh();}
CCEXTERNC void TAMP_STAMP_IRQHandler        (){g_uiFailedHandler = 0x11;Crashedh();}
CCEXTERNC void RTC_WKUP_IRQHandler          (){g_uiFailedHandler = 0x12;Crashedh();}
CCEXTERNC void FLASH_IRQHandler             (){g_uiFailedHandler = 0x13;Crashedh();}
CCEXTERNC void RCC_IRQHandler               (){g_uiFailedHandler = 0x14;Crashedh();}
CCEXTERNC void EXTI0_IRQHandler             (){g_uiFailedHandler = 0x15;Crashedh();}
CCEXTERNC void EXTI1_IRQHandler             (){g_uiFailedHandler = 0x16;Crashedh();}
CCEXTERNC void EXTI2_IRQHandler             (){g_uiFailedHandler = 0x17;Crashedh();}
CCEXTERNC void EXTI3_IRQHandler             (){g_uiFailedHandler = 0x18;Crashedh();}
CCEXTERNC void EXTI4_IRQHandler             (){g_uiFailedHandler = 0x19;Crashedh();}
CCEXTERNC void DMA1_Stream0_IRQHandler      (){g_uiFailedHandler = 0x20;Crashedh();}
CCEXTERNC void DMA1_Stream1_IRQHandler      (){g_uiFailedHandler = 0x21;Crashedh();}
CCEXTERNC void DMA1_Stream2_IRQHandler      (){g_uiFailedHandler = 0x22;Crashedh();}
CCEXTERNC void DMA1_Stream3_IRQHandler      (){g_uiFailedHandler = 0x23;Crashedh();}
CCEXTERNC void DMA1_Stream4_IRQHandler      (){g_uiFailedHandler = 0x24;Crashedh();}
CCEXTERNC void DMA1_Stream5_IRQHandler      (){g_uiFailedHandler = 0x25;Crashedh();}
CCEXTERNC void DMA1_Stream6_IRQHandler      (){g_uiFailedHandler = 0x26;Crashedh();}
CCEXTERNC void ADC_IRQHandler               (){g_uiFailedHandler = 0x27;Crashedh();}
CCEXTERNC void CAN1_TX_IRQHandler           (){g_uiFailedHandler = 0x28;Crashedh();}
CCEXTERNC void CAN1_RX0_IRQHandler          (){g_uiFailedHandler = 0x29;Crashedh();}
CCEXTERNC void CAN1_RX1_IRQHandler          (){g_uiFailedHandler = 0x30;Crashedh();}
CCEXTERNC void CAN1_SCE_IRQHandler          (){g_uiFailedHandler = 0x31;Crashedh();}
CCEXTERNC void EXTI9_5_IRQHandler           (){g_uiFailedHandler = 0x32;Crashedh();}
CCEXTERNC void TIM1_BRK_TIM9_IRQHandler     (){g_uiFailedHandler = 0x33;Crashedh();}
CCEXTERNC void TIM1_UP_TIM10_IRQHandler     (){g_uiFailedHandler = 0x34;Crashedh();}
CCEXTERNC void TIM1_TRG_COM_TIM11_IRQHandler(){g_uiFailedHandler = 0x35;Crashedh();}
CCEXTERNC void TIM1_CC_IRQHandler           (){g_uiFailedHandler = 0x36;Crashedh();}
CCEXTERNC void TIM3_IRQHandler              (){g_uiFailedHandler = 0x37;Crashedh();}
CCEXTERNC void TIM4_IRQHandler              (){g_uiFailedHandler = 0x38;Crashedh();}
CCEXTERNC void I2C1_EV_IRQHandler           (){g_uiFailedHandler = 0x39;Crashedh();}
CCEXTERNC void I2C1_ER_IRQHandler           (){g_uiFailedHandler = 0x40;Crashedh();}
CCEXTERNC void I2C2_EV_IRQHandler           (){g_uiFailedHandler = 0x41;Crashedh();}
CCEXTERNC void I2C2_ER_IRQHandler           (){g_uiFailedHandler = 0x42;Crashedh();}
CCEXTERNC void SPI1_IRQHandler              (){g_uiFailedHandler = 0x43;Crashedh();}
CCEXTERNC void SPI2_IRQHandler              (){g_uiFailedHandler = 0x44;Crashedh();}
CCEXTERNC void USART1_IRQHandler            (){g_uiFailedHandler = 0x45;Crashedh();}
CCEXTERNC void USART2_IRQHandler            (){g_uiFailedHandler = 0x46;Crashedh();}
CCEXTERNC void USART3_IRQHandler            (){g_uiFailedHandler = 0x47;Crashedh();}
CCEXTERNC void EXTI15_10_IRQHandler         (){g_uiFailedHandler = 0x48;Crashedh();}
CCEXTERNC void RTC_Alarm_IRQHandler         (){g_uiFailedHandler = 0x49;Crashedh();}
CCEXTERNC void OTG_FS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x50;Crashedh();}
CCEXTERNC void TIM8_BRK_TIM12_IRQHandler    (){g_uiFailedHandler = 0x51;Crashedh();}
CCEXTERNC void TIM8_UP_TIM13_IRQHandler     (){g_uiFailedHandler = 0x52;Crashedh();}
CCEXTERNC void TIM8_TRG_COM_TIM14_IRQHandler(){g_uiFailedHandler = 0x53;Crashedh();}
CCEXTERNC void TIM8_CC_IRQHandler           (){g_uiFailedHandler = 0x54;Crashedh();}
CCEXTERNC void DMA1_Stream7_IRQHandler      (){g_uiFailedHandler = 0x55;Crashedh();}
CCEXTERNC void FSMC_IRQHandler              (){g_uiFailedHandler = 0x56;Crashedh();}
CCEXTERNC void SDIO_IRQHandler              (){g_uiFailedHandler = 0x57;Crashedh();}
CCEXTERNC void TIM5_IRQHandler              (){g_uiFailedHandler = 0x58;Crashedh();}
CCEXTERNC void SPI3_IRQHandler              (){g_uiFailedHandler = 0x59;Crashedh();}
CCEXTERNC void UART4_IRQHandler             (){g_uiFailedHandler = 0x60;Crashedh();}
CCEXTERNC void UART5_IRQHandler             (){g_uiFailedHandler = 0x61;Crashedh();}
CCEXTERNC void TIM6_DAC_IRQHandler          (){g_uiFailedHandler = 0x62;Crashedh();}
CCEXTERNC void TIM7_IRQHandler              (){g_uiFailedHandler = 0x63;Crashedh();}
CCEXTERNC void DMA2_Stream0_IRQHandler      (){g_uiFailedHandler = 0x64;Crashedh();}
CCEXTERNC void DMA2_Stream1_IRQHandler      (){g_uiFailedHandler = 0x65;Crashedh();}
CCEXTERNC void DMA2_Stream2_IRQHandler      (){g_uiFailedHandler = 0x66;Crashedh();}
CCEXTERNC void DMA2_Stream3_IRQHandler      (){g_uiFailedHandler = 0x67;Crashedh();}
CCEXTERNC void DMA2_Stream4_IRQHandler      (){g_uiFailedHandler = 0x68;Crashedh();}
CCEXTERNC void ETH_IRQHandler               (){g_uiFailedHandler = 0x69;Crashedh();}
CCEXTERNC void ETH_WKUP_IRQHandler          (){g_uiFailedHandler = 0x70;Crashedh();}
CCEXTERNC void CAN2_TX_IRQHandler           (){g_uiFailedHandler = 0x71;Crashedh();}
CCEXTERNC void CAN2_RX0_IRQHandler          (){g_uiFailedHandler = 0x72;Crashedh();}
CCEXTERNC void CAN2_RX1_IRQHandler          (){g_uiFailedHandler = 0x73;Crashedh();}
CCEXTERNC void CAN2_SCE_IRQHandler          (){g_uiFailedHandler = 0x74;Crashedh();}
CCEXTERNC void OTG_FS_IRQHandler            (){g_uiFailedHandler = 0x75;Crashedh();}
CCEXTERNC void DMA2_Stream5_IRQHandler      (){g_uiFailedHandler = 0x76;Crashedh();}
CCEXTERNC void DMA2_Stream6_IRQHandler      (){g_uiFailedHandler = 0x77;Crashedh();}
CCEXTERNC void DMA2_Stream7_IRQHandler      (){g_uiFailedHandler = 0x78;Crashedh();}
CCEXTERNC void USART6_IRQHandler            (){g_uiFailedHandler = 0x79;Crashedh();}
CCEXTERNC void I2C3_EV_IRQHandler           (){g_uiFailedHandler = 0x80;Crashedh();}
CCEXTERNC void I2C3_ER_IRQHandler           (){g_uiFailedHandler = 0x81;Crashedh();}
CCEXTERNC void OTG_HS_EP1_OUT_IRQHandler    (){g_uiFailedHandler = 0x82;Crashedh();}
CCEXTERNC void OTG_HS_EP1_IN_IRQHandler     (){g_uiFailedHandler = 0x83;Crashedh();}
CCEXTERNC void OTG_HS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x84;Crashedh();}
CCEXTERNC void OTG_HS_IRQHandler            (){g_uiFailedHandler = 0x85;Crashedh();}
CCEXTERNC void DCMI_IRQHandler              (){g_uiFailedHandler = 0x86;Crashedh();}
CCEXTERNC void HASH_RNG_IRQHandler          (){g_uiFailedHandler = 0x87;Crashedh();}
CCEXTERNC void FPU_IRQHandler               (){g_uiFailedHandler = 0x88;Crashedh();}
