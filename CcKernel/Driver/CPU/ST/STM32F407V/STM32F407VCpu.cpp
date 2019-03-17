/**
 * @author     Andreas Dirmeier
 * @copyright  Andreas Dirmeier (c) 2015
 * @par       Language: C++11
 */
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class STM32F407VCpu
 **/
#include <stm32f4xx_hal.h>
#include <STM32F407VCpu.h>
#include "CcKernel.h"
#include <STM32F407VSystemTimer.h>
#include "IThread.h"

#define STACK_SIZE 1024

typedef void(*TaskFunction_t)(void* pParam);
size_t g_uiFailedHandler = 0x00;

CCEXTERNC void CreateThread(void* pParam)
{
  IThread *pThreadObject = static_cast<IThread *>(pParam);
  if (pThreadObject->getThreadState() == EThreadState::Starting)
  {
    pThreadObject->enterState(EThreadState::Running);
    pThreadObject->run();
    pThreadObject->enterState(EThreadState::Stopped);
    pThreadObject->onStopped();
  }
  else
  {
    // Do net create threads wich are not in starting state
    pThreadObject->enterState(EThreadState::Stopped);
  }
  // @todo force thread switch
  while(1);
}
CCEXTERNC void STM32F407VCpu_SysTick( void );
CCEXTERNC void Crashedh( void )
{
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

class CcThreadData
{
public:
  CcThreadData(IThread* pThread)
  {
    puiTopStack = aStack + STACK_SIZE - 1;
    pxPortInitialiseStack(pThread);
  }

  /*
   * See header file for description.
   */
  void  pxPortInitialiseStack(IThread* pThread)
  {
    /* Simulate the stack frame as it would be created by a context switch
    interrupt. */

    /* Offset added to account for the way the MCU uses the stack on entry/exit
    of interrupts, and to ensure alignment. */
    puiTopStack--;

    *puiTopStack = 0x01000000; /* xPSR */
    puiTopStack--;
    *puiTopStack = ( ( uint32 ) CreateThread ) & 0xfffffffe;  /* PC */
    puiTopStack--;
    *puiTopStack = ( uint32 ) Crashedh;  /* LR */

    /* Save code space by skipping register initialisation. */
    puiTopStack -= 5;  /* R12, R3, R2 and R1. */
    *puiTopStack = ( uint32 ) pThread; /* R0 */

    /* A save method is being used that requires each task to maintain its
    own exec return value. */
    puiTopStack--;
    *puiTopStack =  0xfffffffd;

    puiTopStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
  }

  volatile uint32* puiTopStack = nullptr;
  volatile uint32  aStack[STACK_SIZE];
};

/*-----------------------------------------------------------*/

class STM32F407VCpu::STM32F407VCpuPrivate
{
public:
  bool bThreadChanged = false;
  CcThreadContext* pMainThreadContext = nullptr;
  static STM32F407VSystemTimer* pSysTimer;
  static volatile CcThreadData oMainThreadContext;
};

class STM32F407VCpuThread : public IThread
{
public:
  STM32F407VCpuThread()
    {enterState(EThreadState::Running);}
  virtual void run() override
    {}
};

STM32F407VSystemTimer* STM32F407VCpu::STM32F407VCpuPrivate::pSysTimer= nullptr;
STM32F407VCpuThread oMainThread;
volatile CcThreadData STM32F407VCpu::STM32F407VCpuPrivate::oMainThreadContext(&oMainThread);

volatile CcThreadData* pCurrentThreadContext = &STM32F407VCpu::STM32F407VCpuPrivate::oMainThreadContext;
const uint8 ucMaxSyscallInterruptPriority = 0;

CCEXTERNC void STM32F407VCpu_SysTick()
{
  if(STM32F407VCpu::STM32F407VCpuPrivate::pSysTimer)
    STM32F407VCpu::STM32F407VCpuPrivate::pSysTimer->timeout();
  HAL_IncTick();
}

CCEXTERNC void SysTick_Handler( void )
{
  __asm volatile("  mrs r0, psp                    \n"); // Load Process Stack Pointer, here we are storing our stack
  __asm volatile("  isb                            \n");
  __asm volatile("                                 \n");
  __asm volatile("  ldr  r3, pCurrentThreadContextConst\n"); // Load current thread context
  __asm volatile("  ldr  r2, [r3]                  \n"); // Write address of first context to r2
  __asm volatile("                                 \n");
  __asm volatile("  tst r14, #0x10                 \n"); //******************
  __asm volatile("  it eq                          \n"); // Backup FPU
  __asm volatile("  vstmdbeq r0!, {s16-s31}        \n"); //******************
  __asm volatile("                                 \n");
  __asm volatile("  stmdb r0!, {r4-r11, r14}       \n"); // Backup Registers to stack of current thread
  __asm volatile("  str r0, [r2]                   \n"); // Backup new stack pointer in thread context
  __asm volatile("                                 \n");
  __asm volatile("  stmdb sp!, {r0, r3}            \n"); // Backup current register state on Main Stack Pointer
  __asm volatile("  mov r0, #0                     \n"); // Disable exceptions
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  dsb                            \n");
  __asm volatile("  isb                            \n");

  __asm volatile("  bl STM32F407VCpu_SysTick       \n");  // Publish tick to kernel, it could change thread context too.

  __asm volatile("  mov r0, #0                     \n");
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  ldmia sp!, {r0, r3}            \n"); // Restore registers from MSP
  __asm volatile("                                 \n");
  __asm volatile("  ldr r1, [r3]                   \n"); // Get back thread context
  __asm volatile("  ldr r0, [r1]                   \n"); // Get back stack pointer form thread context
  __asm volatile("  ldmia r0!, {r4-r11, r14}       \n"); // Get back registers from stack of thread
  __asm volatile("                                 \n");
  __asm volatile("  tst r14, #0x10                 \n"); //******************
  __asm volatile("  it eq                          \n"); // Restore FPU
  __asm volatile("  vldmiaeq r0!, {s16-s31}        \n"); //******************
  __asm volatile("                                 \n");
  __asm volatile("  msr psp, r0                    \n"); // Load stack pointer of thread context
  __asm volatile("  bx r14                         \n"); // continue execution.
  __asm volatile("                                 \n");
  __asm volatile("  .align 4                       \n");
  __asm volatile("pCurrentThreadContextConst: .word pCurrentThreadContext  \n");
}

STM32F407VCpu::STM32F407VCpu()
{
  m_pPrivate = new STM32F407VCpuPrivate();
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->pMainThreadContext = new CcThreadContext();
  CCMONITORNEW(m_pPrivate->pThreadContext);
  m_pPrivate->pMainThreadContext->pThreadObject = &oMainThread;
  m_pPrivate->pMainThreadContext->pContext= (void*)&m_pPrivate->oMainThreadContext;
}

STM32F407VCpu::~STM32F407VCpu()
{
  CCDELETE(m_pPrivate);
}

size_t STM32F407VCpu::coreNumber()
{
  return 1;
}

CcThreadContext* STM32F407VCpu::mainThread()
{
  return m_pPrivate->pMainThreadContext;
}

CcThreadContext* STM32F407VCpu::createThread(IThread* pTargetThread)
{
  CcThreadContext* pReturn = new CcThreadContext();
  pReturn->pThreadObject = pTargetThread;
  CCMONITORNEW(pReturn);
  pReturn->pContext = new CcThreadData(pTargetThread);
  CCMONITORNEW(pReturn->pContext);
  return pReturn;
}

void  STM32F407VCpu::loadThread(CcThreadContext* pTargetThread)
{
  pCurrentThreadContext = static_cast<CcThreadData*>(pTargetThread->pContext);
}

void  STM32F407VCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pThreadData = static_cast<CcThreadData*>(pTargetThread->pContext);
  CCDELETE(pThreadData);
  CCDELETE(pTargetThread);
}

void STM32F407VCpu::setTargetTimer(STM32F407VSystemTimer* pTarget)
{
  STM32F407VCpuPrivate::pSysTimer = pTarget;
}
