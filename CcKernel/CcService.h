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
 * @brief     Class CcService
 */
#pragma once

#include "CcBase.h"
#include "CcKernel.h"
#include "CcApp.h"
#include "CcEventActionLoop.h"

/**
 * @brief Menue-Tree start point with settings for display
 */
class CcKernelSHARED CcService :  private CcApp,
                                  private CcEventActionLoop
{
public:
  /**
   * @brief Constructor
   */
  CcService();

  /**
   * @brief Create service by name
   * @param sAppName: Name of service
   */
  CcService(const CcString& sAppName);

  /**
   * @brief Create service by name and uuid
   * @param sAppName: Name of service
   * @param oUuid:    Unique identifier for service.
   */
  CcService(const CcString& sAppName, const CcUuid& oUuid);

  /**
   * @brief Destructor
   */
  virtual ~CcService() override;

  /**
   * @brief Start event for service startup
   */
  virtual void eventStart();

  /**
   * @brief Pause event to hold on service actions
   */
  virtual void eventPause();

  /**
   * @brief Stop event to close service and exit.
   */
  virtual void eventStop();

  virtual CcStatus exec() override;

protected:
  /**
   * @brief Idle command executed on event loop
   *        It can be overloaded from service to do work that
   *        does not need an instant action.
   */
  virtual void idle();

private:
  virtual void run() override final;
  virtual bool onLoop() override final;
  virtual void onStop() override final;
};
