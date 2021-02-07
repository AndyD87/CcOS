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
 * @brief     Implementation of class FreeRTOSCpu
 **/

#include <Driver/CPU/Common/CcThreadData.h>
#include "CcKernel.h"
#include "CcStatic.h"
#include "IThread.h"
CCEXTERNC_BEGIN
CCEXTERNC_END

#define STACK_SIZE              1024  //!< Default stack settings
#define STACK_OVERFLOW_SPACE      64  //!< Default stack settings
#define STACK_OVERFLOW_PATTERN  0xcc  //!< Default stack settings

CcThreadData::CcThreadData(CcThreadContext* pThreadContext)
{
  size_t uiStackSize = (STACK_SIZE > pThreadContext->pThreadObject->getStackSize()) ? STACK_SIZE : pThreadContext->pThreadObject->getStackSize();
  uiStackSize += STACK_OVERFLOW_SPACE;
  uiStackSize >>= 2;
  CCNEWARRAY(puiStack, uint32, uiStackSize);
  CcStatic::memset(puiStack, STACK_OVERFLOW_PATTERN, STACK_OVERFLOW_SPACE);
  uiStackSize--;
  puiTopStack = puiStack + uiStackSize;
  initStack(pThreadContext);
}

CcThreadData::~CcThreadData()
{
  if(isOverflowDetectedEx())
  {
    CcKernel::message(EMessage::Error);
  }
  CCDELETEARR(puiStack);
}

bool CcThreadData::isOverflowDetected() volatile
{
  bool bOverflow = false;
  if(puiStack + (STACK_OVERFLOW_SPACE >> 2) > puiTopStack)
  {
    bOverflow = true;
  }
  return bOverflow;
}

bool CcThreadData::isOverflowDetectedEx() volatile
{
  bool bOverflow = false;
  unsigned char* pucBuffer = CCVOIDPTRCAST(unsigned char*, puiStack);
  for(size_t uiPos = 0; uiPos < STACK_OVERFLOW_SPACE; uiPos++)
  {
    if(STACK_OVERFLOW_PATTERN != pucBuffer[uiPos])
      bOverflow = true;
  }
  return bOverflow;
}

void CcThreadData::initStack(CcThreadContext* pThread)
{
  /* Simulate the stack frame as it would be created by a context switch
  interrupt. */
  /* Offset added to account for the way the MCU uses the stack on entry/exit
  of interrupts, and to ensure alignment. */
  puiTopStack--;
  *puiTopStack = 0x01000000; /* xPSR */
  puiTopStack--;
  *puiTopStack = (reinterpret_cast<uintptr>(ICpu::CreateThreadMethod)) & 0xfffffffe;  /* PC */
  puiTopStack--;
  *puiTopStack = (reinterpret_cast<uintptr>(ICpu::CreateThreadMethod));  /* LR */
  puiTopStack -= 5;
  *puiTopStack = reinterpret_cast<uintptr>( pThread); /* R0 */
  puiTopStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
}
