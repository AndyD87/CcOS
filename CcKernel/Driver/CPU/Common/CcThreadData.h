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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcThreadData
 */
#pragma once

#include "CcBase.h"
#include "Devices/ICpu.h"

/**
 * @brief Thread data with ThreadObject and stack
 */
class CcThreadData
{
public:
  /**
   * @brief Create Thread Data with CPU thread context
   * @param pThreadContext: Threadcontext with Thread informations
   */
  CcThreadData(CcThreadContext* pThreadContext);
  ~CcThreadData();

  /**
   * @brief Check if patterns at the end of Stack are changed.
   * @return True if data behind thread stack was changed.
   */
  bool isOverflowDetected() volatile;

  /**
   * @brief Check if patterns at the end of Stack are changed.
   * @return True if data behind thread stack was changed.
   */
  bool isOverflowDetectedEx() volatile;

  /**
   * @brief Init stack with size from Thread
   * @param pThread: Thread info with stack size
   */
  void initStack(CcThreadContext* pThread);

public:
  uint32*  puiTopStack = nullptr; //!< First pointer on stack
  uint32*  puiStack    = nullptr; //!< Current stack position
};
