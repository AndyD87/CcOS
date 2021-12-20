/*
 * This file is part of CcRemoteDeviceServerService.
 *
 * CcRemoteDeviceServerService is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServerService is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServerService.  If not, see <http://www.gnu.org/licenses/>.
 **/
 /**
  * @page      CcRemoteDeviceServerService
  * @copyright Andreas Dirmeier (C) 2017
  * @author    Andreas Dirmeier
  * @par       Web: http://coolcow.de
  * @par       Language   C++ ANSI V3
  * @brief     Class CcRemoteDeviceServerService
  **/
#pragma once

#include "CcBase.h"
#include "CcService.h"
#include "CcArguments.h"

  /**
   * @brief CcRemoteDeviceServerService impelmentation
   *        Main class wich is loaded to start Application.
   */
class CcRemoteDeviceServerService : public CcService
{
public:
  /**
   * @brief Constructor with Arguments
   * @param oArguments: Arguments from cli to pass.
   */
  CcRemoteDeviceServerService(const CcArguments& oArguments);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServerService(void);
};
