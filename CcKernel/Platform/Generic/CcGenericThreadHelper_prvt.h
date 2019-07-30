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
 * @page      Linux
 * @subpage   CcGenericThreadHelper_prvt
 *
 * @pag       CcGenericThreadHelper_prvt
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcGenericThreadHelper_prvt
 */
#ifndef H_CcGenericThreadHelper_prvt_H_
#define H_CcGenericThreadHelper_prvt_H_

#include "CcBase.h"

#define OFFSET_OF(type, member) ((size_t)&(((type *)0)->member))

#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif

CCEXTERNC_BEGIN
  extern void* pxCurrentTCB;
  extern void* pxReadyTasksLists;
  extern size_t xDelayedTaskList1;
  extern size_t xDelayedTaskList2;
  extern size_t pxDelayedTaskList;
  extern size_t xPendingReadyList;
  extern size_t xTasksWaitingTermination;
  extern size_t xSuspendedTaskList;
  extern size_t uxCurrentNumberOfTasks;
  extern size_t uxTopUsedPriority;
CCEXTERNC_END

#endif /* H_CcGenericThreadHelper_prvt_H_ */
