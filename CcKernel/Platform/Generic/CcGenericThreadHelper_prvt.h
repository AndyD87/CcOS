/*
 *
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
 * @page      CcGenericThreadHelper_prvt
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcGenericThreadHelper_prvt
 */
#pragma once

#include "CcBase.h"

//! Get offset of specific member
#define OFFSET_OF(type, member) ((size_t)&(((type *)0)->member))

//! Mark method as aused to force implementation
#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif

CCEXTERNC_BEGIN
  extern void* pxCurrentTCB;              //!< Task variables from FreeRTOS for debug
  extern void* pxReadyTasksLists;         //!< Task variables from FreeRTOS for debug
  extern size_t xDelayedTaskList1;        //!< Task variables from FreeRTOS for debug
  extern size_t xDelayedTaskList2;        //!< Task variables from FreeRTOS for debug
  extern size_t pxDelayedTaskList;        //!< Task variables from FreeRTOS for debug
  extern size_t xPendingReadyList;        //!< Task variables from FreeRTOS for debug
  extern size_t xTasksWaitingTermination; //!< Task variables from FreeRTOS for debug
  extern size_t xSuspendedTaskList;       //!< Task variables from FreeRTOS for debug
  extern size_t uxCurrentNumberOfTasks;   //!< Task variables from FreeRTOS for debug
  extern size_t uxTopUsedPriority;        //!< Task variables from FreeRTOS for debug
CCEXTERNC_END
