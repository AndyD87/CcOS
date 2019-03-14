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

  __asm volatile("  bl SysTick                     \n");  // Publish tick to kernel, it could change thread context too.

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