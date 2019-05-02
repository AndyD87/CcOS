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

ICpu::~ICpu() {
}


void ICpu::CreateThread(void* pParam)
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
  while(1)
    CcKernel::delayMs(0);
}

