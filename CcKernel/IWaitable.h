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
 * @brief     Class IWaitable
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief Waitable object for kernel for faster check if thread could
 *        be continued.
 */
class CcKernelSHARED IWaitable
{
public:
  IWaitable() = default;
  /**
   * @brief Destructor
   */
  virtual ~IWaitable( ) = default;
  
  /**
   * @brief Called from kernel to check condition.
   * @return True if waiting is done
   */
  virtual bool condition() = 0;

  /**
   * @brief Signal from external command to check condition again.
   */
  virtual void signal()    = 0;

  /**
   * @brief Execute wait, register thread for waiting in Kernel space
   *        until condition is solved and thread can continue.
   */
  virtual void wait();
};
