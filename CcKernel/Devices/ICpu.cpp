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
 * @brief     Implementation of class ICpu
 */

#include "Devices/ICpu.h"
#include "IThread.h"
#include "CcKernel.h"
#include "CcThreadContext.h"

ICpu::~ICpu() {
}


void ICpu::CreateThreadMethod(CcThreadContext* pThreadContext)
{
  if(pThreadContext < (void*)0x10000)
    CCCHECKNULL(nullptr);
  CCCHECKNULL(pThreadContext);
  CCCHECKNULL(pThreadContext->pThreadObject);
  if (pThreadContext->pThreadObject->getThreadState() == EThreadState::Starting)
  {
    if(pThreadContext < (void*)0x10000)
      CCCHECKNULL(nullptr);
    pThreadContext->pThreadObject->enterState(EThreadState::Running);
    if(pThreadContext < (void*)0x10000)
      CCCHECKNULL(nullptr);
    pThreadContext->pThreadObject->run();
    if(pThreadContext < (void*)0x10000)
      CCCHECKNULL(nullptr);
    pThreadContext->pThreadObject->enterState(EThreadState::Stopped);
    if(pThreadContext < (void*)0x10000)
      CCCHECKNULL(nullptr);
    pThreadContext->pThreadObject->onStopped();
    if(pThreadContext < (void*)0x10000)
      CCCHECKNULL(nullptr);
  }
  else
  {
    // Do net create threads wich are not in starting state
    pThreadContext->pThreadObject->enterState(EThreadState::Stopped);
    if(pThreadContext < (void*)0x10000)
      CCCHECKNULL(nullptr);
  }
  // @todo force thread switch
  pThreadContext->bClosed = true;
  while(1)
    CcKernel::delayMs(0);
}

