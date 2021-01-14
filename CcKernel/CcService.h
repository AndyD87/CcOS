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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcService
 */
#ifndef H_CcService_H_
#define H_CcService_H_

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
   * @brief Constructor
   */
  CcService(const CcString& sAppName);

  /**
   * @brief Constructor
   */
  CcService(const CcString& sAppName, const CcUuid& oUuid);

  /**
   * @brief Destructor
   */
  virtual ~CcService() override;

  virtual void eventStart();
  virtual void eventPause();
  virtual void eventStop();

  virtual CcStatus exec() override;

protected:
  virtual void idle();

private:
  virtual void run() override final;
  virtual bool onLoop() override final;
  virtual void onStop() override final;
};

#endif // H_CcService_H_
