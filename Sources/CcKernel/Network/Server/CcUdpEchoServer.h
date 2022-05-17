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
 * @brief     Class CcUdpEchoServer
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcApp.h"

/**
 * @brief This server will wait on an UDP Port for input
 *        and will send back the data to sender.
 */
class CcKernelSHARED CcUdpEchoServer : public CcApp
{
public:
  /**
   * @brief Create server on specific port
   * @param uiPort: Target port to listen on
   */
  CcUdpEchoServer(uint16 uiPort):
    CcApp("CcUdpEchoServer"),
    m_uiPort(uiPort)
  {}
  virtual ~CcUdpEchoServer()
  {}

  /**
   * @brief Receiving loop
   */
  virtual void run() override;
private:
  uint16 m_uiPort;
};
