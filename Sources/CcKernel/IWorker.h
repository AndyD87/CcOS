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
 * @brief     Class IWorker
 */
#pragma once

#include "CcBase.h"
#include "IThread.h"

/**
 * @brief Abstract-Class for Executing an Object wich has to delete itself
 *        when Job is done.
 */
class CcKernelSHARED IWorker : public IThread
{
public:
  /**
   * @brief Constructor
   */
  IWorker(const CcString& sName = ""):
    IThread(sName)
  {}

  /**
   * @brief Destructor
   */
  virtual ~IWorker() = default;

  /**
   * @brief Needs to be overloaded with the Function
   *        wich has to be executed on start().
   *        After returning in this function, the object will delete itself.
   */
  virtual void run() override = 0;

  /**
   * @brief This method has to be called last, it will destroy the worker.
   * @return Status and result of Thread.
   */
  virtual CcStatus onStopped() override;
};