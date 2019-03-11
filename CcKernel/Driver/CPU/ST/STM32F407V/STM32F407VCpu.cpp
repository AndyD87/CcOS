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
#include <STM32F407VCpu.h>
#include "CcKernel.h"
#include <STM32F407VSystemTimer.h>
#include "IThread.h"

#define STACK_SIZE 1024

typedef void(*TaskFunction_t)(void* pParam);

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
}

static void prvTaskExitError( void )
{

}

class CcThreadData
{
public:
  CcThreadData(IThread* pThread) :
    m_pThread(pThread)
  {
    puiTopStack = aStack + STACK_SIZE - 1;
    pxPortInitialiseStack();
  }

  /*
   * See header file for description.
   */
  void  pxPortInitialiseStack()
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
    *puiTopStack = ( uint32 ) prvTaskExitError;  /* LR */

    /* Save code space by skipping register initialisation. */
    puiTopStack -= 5;  /* R12, R3, R2 and R1. */
    *puiTopStack = ( uint32 ) m_pThread; /* R0 */

    /* A save method is being used that requires each task to maintain its
    own exec return value. */
    puiTopStack--;
    *puiTopStack =  0xfffffffd;

    puiTopStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
  }


  volatile uint32* puiTopStack = nullptr;
  volatile uint32  aStack[STACK_SIZE];
private:
  IThread* m_pThread;
};

/*-----------------------------------------------------------*/

class STM32F407VCpu::STM32F407VCpuPrivate
{
public:
  bool bThreadChanged = false;
  CcThreadContext* pThreadContext = nullptr;
  static STM32F407VSystemTimer* pSysTimer;
  static volatile CcThreadData oMainThreadContext;
};

STM32F407VSystemTimer* STM32F407VCpu::STM32F407VCpuPrivate::pSysTimer= nullptr;
volatile CcThreadData STM32F407VCpu::STM32F407VCpuPrivate::oMainThreadContext(nullptr);

volatile CcThreadData* pCurrentThreadContext = &STM32F407VCpu::STM32F407VCpuPrivate::oMainThreadContext;
const uint8 ucMaxSyscallInterruptPriority = 0;

CCEXTERNC void SysTick()
{
  if(STM32F407VCpu::STM32F407VCpuPrivate::pSysTimer)
    STM32F407VCpu::STM32F407VCpuPrivate::pSysTimer->timeout();
}

CCEXTERNC void SysTick_Handler( void )
{
#if 1
  /* This is a naked function. */
    __asm volatile("  mrs r0, psp                    \n");
    __asm volatile("  isb                            \n");
    __asm volatile("                                 \n");
    __asm volatile("  ldr  r3, pCurrentThreadContextConst     \n"); /* Get the location of the current TCB. */
    __asm volatile("  ldr  r2, [r3]                  \n");
    __asm volatile("                                 \n");
    __asm volatile("  tst r14, #0x10                 \n"); /* Is the task using the FPU context?  If so, push high vfp registers. */
    __asm volatile("  it eq                          \n");
    __asm volatile("  vstmdbeq r0!, {s16-s31}        \n");
    __asm volatile("                                 \n");
    __asm volatile("  stmdb r0!, {r4-r11, r14}       \n"); /* Save the remaining registers. */
    __asm volatile("  str r0, [r2]                   \n"); /* Save the new top of stack into the first member of the TCB. */
    __asm volatile("                                 \n");
    __asm volatile("  stmdb sp!, {r0, r3}            \n");
    __asm volatile("  mov r0, #0                     \n");
    __asm volatile("  msr basepri, r0                \n");
    __asm volatile("  dsb                            \n");
    __asm volatile("  isb                            \n");

    __asm volatile("  bl SysTick                     \n");  // Go back to main application.

    __asm volatile("  mov r0, #0                     \n");
    __asm volatile("  msr basepri, r0                \n");
    __asm volatile("  ldmia sp!, {r0, r3}            \n");
    __asm volatile("                                 \n"); /* Restore the context. */
    __asm volatile("  ldr r1, [r3]                   \n");
    __asm volatile("  ldr r0, [r1]                   \n"); /* The first item in the TCB is the task top of stack. */
    __asm volatile("  ldmia r0!, {r4-r11, r14}       \n"); /* Pop the registers that are not automatically saved on exception entry. */
    __asm volatile("                                 \n");
    __asm volatile("  tst r14, #0x10                 \n"); /* Is the task using the FPU context?  If so, pop the high vfp registers too. */
    __asm volatile("  it eq                          \n");
    __asm volatile("  vldmiaeq r0!, {s16-s31}        \n");
    __asm volatile("                                 \n");
    __asm volatile("  msr psp, r0                    \n");
    __asm volatile("  bx r14                         \n");
    __asm volatile("                                 \n");
    __asm volatile("  .align 4                       \n");
    __asm volatile("pCurrentThreadContextConst: .word pCurrentThreadContext  \n");
#else
  SwitchContext();
//  __asm volatile("  ldr  r0, pPestoreThreadContextConst     \n"); /* Get the location of the current TCB. */
//  __asm volatile("  str r2, [r0] \n"); /* Save the new top of stack into the first member of the TCB. */
//  __asm volatile("  stmia r2!, {r0, r3}            \n");
//  __asm volatile("  ldr  r10, pPestoreThreadContextConst     \n"); /* Get the location of the current TCB. */
//  __asm volatile("  ldr r2, [r10] \n"); /* Save the new top of stack into the first member of the TCB. */
//  __asm volatile("  ldmdb r2!, {r0, r3}            \n");
#endif
}

STM32F407VCpu::STM32F407VCpu()
{
  m_pPrivate = new STM32F407VCpuPrivate();
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->pThreadContext = new CcThreadContext();
  CCMONITORNEW(m_pPrivate->pThreadContext);
  m_pPrivate->pThreadContext->pContext= (void*)&m_pPrivate->oMainThreadContext;
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
  return m_pPrivate->pThreadContext;
}

CcThreadContext* STM32F407VCpu::createThread(IThread* pTargetThread)
{
  CcThreadContext* pReturn = new CcThreadContext();
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
