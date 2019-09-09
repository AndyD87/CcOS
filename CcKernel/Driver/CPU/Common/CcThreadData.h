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
 * @page      CPU_Common
 * @subpage   CcThreadData
 *
 * @page      CcThreadData
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcThreadData
 */
#ifndef H_CcThreadData_H_
#define H_CcThreadData_H_

#include "CcBase.h"
#include "Devices/ICpu.h"

class CcThreadData
{
public:
  CcThreadData(CcThreadContext* pThreadContext);
  ~CcThreadData();

  bool isOverflowDetected() volatile;
  bool isOverflowDetectedEx() volatile;
  void initStack(CcThreadContext* pThread);

public:
  uint32*  puiTopStack = nullptr;
  uint32*  puiStack    = nullptr;
};

#endif // H_CcThreadData_H_
