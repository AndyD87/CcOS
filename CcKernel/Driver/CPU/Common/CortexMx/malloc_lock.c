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
 * @brief     Implementation of class malloc_lock
 **/

#include "CcBase.h"

int g_iLocked = 0;
int g_iLockPrimask;

void __malloc_lock( struct _reent *_r )
{
  if(g_iLocked == 0)
  {
    CCUNUSED(_r);
    __asm("stmfd      sp!, {r0-r1}");
    __asm("ldr         r1, =g_iLockPrimask");
    __asm("mrs         r0, primask");
    __asm("str         r0, [r1]");
    __asm("cpsid      i");
    __asm("ldmfd      sp!, {r0-r1}");
  }
  g_iLocked++;
}

void __malloc_unlock( struct _reent *_r )
{
  g_iLocked--;
  if(g_iLocked == 0)
  {
    CCUNUSED(_r);
    __asm("stmfd  sp!, {r0-r1}");
    __asm("ldr     r1, =g_iLockPrimask");
    __asm("ldr     r0, [r1]");
    __asm("msr     primask, r0");
    __asm("ldmfd  sp!, {r0-r1}");
  }
}
