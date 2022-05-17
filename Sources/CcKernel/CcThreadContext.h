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
 * @brief     Class CcThreadContext
 */
#pragma once

#include "CcBase.h"

class IThread;
class CcThreadData;
/**
 * @brief Thread context to create and manage threads in cpu
 */
class CcThreadContext
{
public:
  CcThreadContext() = default;

  /**
   * @brief Initialize context with CPU specific data and ThreadObject to
   *        load.
   * @param pThreadObject:  Target thread object to load on thread
   * @param pData:          Data generated from CPU
   */
  CcThreadContext(IThread* pThreadObject, CcThreadData* pData):
    pThreadObject(pThreadObject),
    pData(pData)
  {}

  /**
   * @brief Change data if to update thread info if required.
   * @param pNewData: New data set.
   */
  void setData(CcThreadData* pNewData)
  { pData = pNewData; }

  IThread*      pThreadObject  = nullptr;   //!< Thread object as workset
  CcThreadData* pData          = nullptr;   //!< CPU Thread info like stack
  bool          bClosed        = false;     //!< Thread closed flag for thread manager
};
