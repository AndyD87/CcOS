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
 * @brief     Implementation of class IrqHandles on STM devices
 **/
#include <CcBase.h>
#include <Devices/ICpu.h>
#include "STM32FX_IrqHandlers.h"

__attribute__( ( always_inline ) ) inline uint32 GetMSP(void)
{
  register uint32 result;
  __asm volatile ("MRS %0, msp\n" : "=r" (result) );
  return(result);
}

__attribute__( ( always_inline ) ) inline uint32 GetPSP(void)
{
  register uint32 result;
  __asm volatile ("MRS %0, psp\n"  : "=r" (result) );
  return(result);
}

//! Storage for unimplemented irq handler id
volatile size_t g_uiFailedHandler = 0x00;
volatile size_t g_uiCrashRunning  = 0x00;

struct __attribute__((packed)) _ContextStateFrame
{
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t return_address;
  uint32_t xpsr;
} *puiStack;

//! @brief method to call from unimplemented irq handler
void Crashed(){
  g_uiCrashRunning  = 0x01;
  while(g_uiCrashRunning);
}

#pragma GCC push_options
#pragma GCC optimize("O0")
//! Forward not implemented Handler and set crashed code
#ifndef NMI_Handler_Handled
  CCEXTERNC void NMI_Handler                  (){g_uiFailedHandler = 0x01;Crashed();}
#endif
#ifndef HardFault_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void HardFault_Handler            ()
{
  g_uiFailedHandler = 0x02;
  puiStack     = reinterpret_cast<struct _ContextStateFrame*>(GetPSP());
  CCUNUSED(puiStack);
  Crashed();
}
#endif
#ifndef MemManage_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void MemManage_Handler            (){g_uiFailedHandler = 0x03;Crashed();}
#endif
#ifndef BusFault_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void BusFault_Handler             (){g_uiFailedHandler = 0x04;Crashed();}
#endif
#ifndef UsageFault_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void UsageFault_Handler           (){g_uiFailedHandler = 0x05;Crashed();}
#endif
#ifndef SVC_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SVC_Handler                  (){g_uiFailedHandler = 0x06;Crashed();}
#endif
#ifndef DebugMon_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DebugMon_Handler             (){g_uiFailedHandler = 0x07;Crashed();}
#endif
#ifndef PendSV_Handler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void PendSV_Handler               (){g_uiFailedHandler = 0x08;Crashed();}
#endif
#ifndef WWDG_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void WWDG_IRQHandler              (){g_uiFailedHandler = 0x09;Crashed();}
#endif
#ifndef PVD_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void PVD_IRQHandler               (){g_uiFailedHandler = 0x10;Crashed();}
#endif
#ifndef TAMP_STAMP_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TAMP_STAMP_IRQHandler        (){g_uiFailedHandler = 0x11;Crashed();}
#endif
#ifndef RTC_WKUP_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void RTC_WKUP_IRQHandler          (){g_uiFailedHandler = 0x12;Crashed();}
#endif
#ifndef FLASH_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void FLASH_IRQHandler             (){g_uiFailedHandler = 0x13;Crashed();}
#endif
#ifndef RCC_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void RCC_IRQHandler               (){g_uiFailedHandler = 0x14;Crashed();}
#endif
#ifndef EXTI0_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI0_IRQHandler             (){g_uiFailedHandler = 0x15;Crashed();}
#endif
#ifndef EXTI1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI1_IRQHandler             (){g_uiFailedHandler = 0x16;Crashed();}
#endif
#ifndef EXTI2_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI2_IRQHandler             (){g_uiFailedHandler = 0x17;Crashed();}
#endif
#ifndef EXTI3_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI3_IRQHandler             (){g_uiFailedHandler = 0x18;Crashed();}
#endif
#ifndef EXTI4_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI4_IRQHandler             (){g_uiFailedHandler = 0x19;Crashed();}
#endif
#ifndef DMA1_Stream0_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream0_IRQHandler      (){g_uiFailedHandler = 0x20;Crashed();}
#endif
#ifndef DMA1_Stream1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream1_IRQHandler      (){g_uiFailedHandler = 0x21;Crashed();}
#endif
#ifndef DMA1_Stream2_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream2_IRQHandler      (){g_uiFailedHandler = 0x22;Crashed();}
#endif
#ifndef DMA1_Stream3_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream3_IRQHandler      (){g_uiFailedHandler = 0x23;Crashed();}
#endif
#ifndef DMA1_Stream4_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream4_IRQHandler      (){g_uiFailedHandler = 0x24;Crashed();}
#endif
#ifndef DMA1_Stream5_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream5_IRQHandler      (){g_uiFailedHandler = 0x25;Crashed();}
#endif
#ifndef DMA1_Stream6_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream6_IRQHandler      (){g_uiFailedHandler = 0x26;Crashed();}
#endif
#ifndef ADC_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void ADC_IRQHandler               (){g_uiFailedHandler = 0x27;Crashed();}
#endif
#ifndef CAN1_TX_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_TX_IRQHandler           (){g_uiFailedHandler = 0x28;Crashed();}
#endif
#ifndef CAN1_RX0_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_RX0_IRQHandler          (){g_uiFailedHandler = 0x29;Crashed();}
#endif
#ifndef CAN1_RX1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_RX1_IRQHandler          (){g_uiFailedHandler = 0x30;Crashed();}
#endif
#ifndef CAN1_SCE_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN1_SCE_IRQHandler          (){g_uiFailedHandler = 0x31;Crashed();}
#endif
#ifndef EXTI9_5_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI9_5_IRQHandler           (){g_uiFailedHandler = 0x32;Crashed();}
#endif
#ifndef TIM1_BRK_TIM9_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_BRK_TIM9_IRQHandler     (){g_uiFailedHandler = 0x33;Crashed();}
#endif
#ifndef TIM1_UP_TIM10_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_UP_TIM10_IRQHandler     (){g_uiFailedHandler = 0x34;Crashed();}
#endif
#ifndef TIM1_TRG_COM_TIM11_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_TRG_COM_TIM11_IRQHandler(){g_uiFailedHandler = 0x35;Crashed();}
#endif
#ifndef TIM1_CC_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM1_CC_IRQHandler           (){g_uiFailedHandler = 0x36;Crashed();}
#endif
#ifndef TIM3_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM3_IRQHandler              (){g_uiFailedHandler = 0x37;Crashed();}
#endif
#ifndef TIM4_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM4_IRQHandler              (){g_uiFailedHandler = 0x38;Crashed();}
#endif
#ifndef I2C1_EV_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C1_EV_IRQHandler           (){g_uiFailedHandler = 0x39;Crashed();}
#endif
#ifndef I2C1_ER_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C1_ER_IRQHandler           (){g_uiFailedHandler = 0x40;Crashed();}
#endif
#ifndef I2C2_EV_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C2_EV_IRQHandler           (){g_uiFailedHandler = 0x41;Crashed();}
#endif
#ifndef I2C2_ER_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C2_ER_IRQHandler           (){g_uiFailedHandler = 0x42;Crashed();}
#endif
#ifndef SPI1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SPI1_IRQHandler              (){g_uiFailedHandler = 0x43;Crashed();}
#endif
#ifndef SPI2_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SPI2_IRQHandler              (){g_uiFailedHandler = 0x44;Crashed();}
#endif
#ifndef USART1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART1_IRQHandler            (){g_uiFailedHandler = 0x45;Crashed();}
#endif
#ifndef USART2_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART2_IRQHandler            (){g_uiFailedHandler = 0x46;Crashed();}
#endif
#ifndef USART3_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART3_IRQHandler            (){g_uiFailedHandler = 0x47;Crashed();}
#endif
#ifndef EXTI15_10_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void EXTI15_10_IRQHandler         (){g_uiFailedHandler = 0x48;Crashed();}
#endif
#ifndef RTC_Alarm_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void RTC_Alarm_IRQHandler         (){g_uiFailedHandler = 0x49;Crashed();}
#endif
#ifndef OTG_FS_WKUP_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_FS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x50;Crashed();}
#endif
#ifndef TIM8_BRK_TIM12_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_BRK_TIM12_IRQHandler    (){g_uiFailedHandler = 0x51;Crashed();}
#endif
#ifndef TIM8_UP_TIM13_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_UP_TIM13_IRQHandler     (){g_uiFailedHandler = 0x52;Crashed();}
#endif
#ifndef TIM8_TRG_COM_TIM14_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_TRG_COM_TIM14_IRQHandler(){g_uiFailedHandler = 0x53;Crashed();}
#endif
#ifndef TIM8_CC_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM8_CC_IRQHandler           (){g_uiFailedHandler = 0x54;Crashed();}
#endif
#ifndef DMA1_Stream7_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA1_Stream7_IRQHandler      (){g_uiFailedHandler = 0x55;Crashed();}
#endif
#ifndef FSMC_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void FSMC_IRQHandler              (){g_uiFailedHandler = 0x56;Crashed();}
#endif
#ifndef SDIO_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SDIO_IRQHandler              (){g_uiFailedHandler = 0x57;Crashed();}
#endif
#ifndef TIM5_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM5_IRQHandler              (){g_uiFailedHandler = 0x58;Crashed();}
#endif
#ifndef SPI3_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void SPI3_IRQHandler              (){g_uiFailedHandler = 0x59;Crashed();}
#endif
#ifndef UART4_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void UART4_IRQHandler             (){g_uiFailedHandler = 0x60;Crashed();}
#endif
#ifndef UART5_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void UART5_IRQHandler             (){g_uiFailedHandler = 0x61;Crashed();}
#endif
#ifndef TIM6_DAC_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM6_DAC_IRQHandler          (){g_uiFailedHandler = 0x62;Crashed();}
#endif
#ifndef TIM7_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void TIM7_IRQHandler              (){g_uiFailedHandler = 0x63;Crashed();}
#endif
#ifndef DMA2_Stream0_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream0_IRQHandler      (){g_uiFailedHandler = 0x64;Crashed();}
#endif
#ifndef DMA2_Stream1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream1_IRQHandler      (){g_uiFailedHandler = 0x65;Crashed();}
#endif
#ifndef DMA2_Stream2_IRQHandler
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream2_IRQHandler      (){g_uiFailedHandler = 0x66;Crashed();}
#endif
#ifndef DMA2_Stream3_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream3_IRQHandler      (){g_uiFailedHandler = 0x67;Crashed();}
#endif
#ifndef DMA2_Stream4_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream4_IRQHandler      (){g_uiFailedHandler = 0x68;Crashed();}
#endif
#ifndef ETH_WKUP_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void ETH_WKUP_IRQHandler          (){g_uiFailedHandler = 0x70;Crashed();}
#endif
#ifndef CAN2_TX_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_TX_IRQHandler           (){g_uiFailedHandler = 0x71;Crashed();}
#endif
#ifndef CAN2_RX0_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_RX0_IRQHandler          (){g_uiFailedHandler = 0x72;Crashed();}
#endif
#ifndef CAN2_RX1_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_RX1_IRQHandler          (){g_uiFailedHandler = 0x73;Crashed();}
#endif
#ifndef CAN2_SCE_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CAN2_SCE_IRQHandler          (){g_uiFailedHandler = 0x74;Crashed();}
#endif
#ifndef OTG_FS_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_FS_IRQHandler            (){g_uiFailedHandler = 0x75;Crashed();}
#endif
#ifndef DMA2_Stream5_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream5_IRQHandler      (){g_uiFailedHandler = 0x76;Crashed();}
#endif
#ifndef DMA2_Stream6_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream6_IRQHandler      (){g_uiFailedHandler = 0x77;Crashed();}
#endif
#ifndef DMA2_Stream7_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DMA2_Stream7_IRQHandler      (){g_uiFailedHandler = 0x78;Crashed();}
#endif
#ifndef USART6_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void USART6_IRQHandler            (){g_uiFailedHandler = 0x79;Crashed();}
#endif
#ifndef I2C3_EV_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C3_EV_IRQHandler           (){g_uiFailedHandler = 0x80;Crashed();}
#endif
#ifndef I2C3_ER_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void I2C3_ER_IRQHandler           (){g_uiFailedHandler = 0x81;Crashed();}
#endif
#ifndef OTG_HS_EP1_OUT_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_EP1_OUT_IRQHandler    (){g_uiFailedHandler = 0x82;Crashed();}
#endif
#ifndef OTG_HS_EP1_IN_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_EP1_IN_IRQHandler     (){g_uiFailedHandler = 0x83;Crashed();}
#endif
#ifndef OTG_HS_WKUP_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_WKUP_IRQHandler       (){g_uiFailedHandler = 0x84;Crashed();}
#endif
#ifndef OTG_HS_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void OTG_HS_IRQHandler            (){g_uiFailedHandler = 0x85;Crashed();}
#endif
#ifndef DCMI_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void DCMI_IRQHandler              (){g_uiFailedHandler = 0x86;Crashed();}
#endif
#ifndef HASH_RNG_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void HASH_RNG_IRQHandler          (){g_uiFailedHandler = 0x87;Crashed();}
#endif
#ifndef FPU_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void FPU_IRQHandler               (){g_uiFailedHandler = 0x88;Crashed();}
#endif
#ifndef CCOS_DEFAULT_IRQHandler_Handled
//! Forward not implemented Handler and set crashed code
CCEXTERNC void CCOS_DEFAULT_IRQHandler      (){g_uiFailedHandler = 0xff;Crashed();}
#endif

#pragma GCC pop_options

//! Forward not implemented Handler and set crashed code
