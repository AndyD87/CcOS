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
#include "CcSharedPointer.h"
#include "IThread.h"

#ifdef WIN32
template class CcKernelSHARED CcSharedPointer<CcApp>;
#endif

/**
 * @brief Service handler for Aplications
 */
class CcKernelSHARED CcService : private CcEventActionLoop
{
public:
  /**
   * @brief Constructor for Service
   *        Name of service will be taken from Application.
   * @param pApplication: Application to be managed by service
   */
  CcService(CcSharedPointer<CcApp> pApplication);

  /**
   * @brief Create service by name
   * @param sServiceName: Service name to create for
   * @param pApplication: Application to be managed by service
   */
  CcService(const CcString& sServiceName, CcSharedPointer<CcApp> pApplication);

  /**
   * @brief Destructor
   */
  virtual ~CcService();

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

  CcStatus run();

  CcStatus exec();

  //! @return Get current name of Service
  const CcString& getName() { return m_sServiceName; }

protected:
  /**
   * @brief Idle command executed on event loop
   *        It can be overloaded from service to do work that
   *        does not need an instant action.
   */
  virtual void idle();

private:
  bool onLoop();
  void onStop();

private:
  CcSharedPointer<CcApp>  m_oApplication;
  CcString                m_sServiceName;
};
