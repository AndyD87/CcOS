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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcGenericThreadHelper
 */
#pragma once

#include "CcBase.h"

/**
 * @brief Target of thred helper class is to generate an interface for
 *        debuggers to read Thred informations for debugging.
 */
class CcGenericThreadHelper
{
public:
  CcGenericThreadHelper() = default;
  ~CcGenericThreadHelper() = default;

  /**
   * @brief Insert thread with context to helper
   * @param pContext:       Context of thread to get thread info
   * @param pStackPointer:  Current pointer on thread stack
   * @param pName:          Name of thread
   */
  void insert(void* pContext, void* pStackPointer, const char* pName);

  /**
   * @brief Set informations of current thread by it's context
   * @param pContext:       Context of thread to get thread info
   */
  void current(void* pContext);

  /**
   * @brief Remove informations of thread by it's context
   * @param pContext:       Context of thread to get thread info
   */
  void remove(void* pContext);
};
