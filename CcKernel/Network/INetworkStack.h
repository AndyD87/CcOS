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
 * @brief     Class INetworkStack
 */
#pragma once

#include "CcBase.h"
#include "CcObject.h"
#include "Network/ISocket.h"
#include "CcVector.h"
#include "CcList.h"

class CcIp;
class CcIpInterface;
class INetwork;

/**
 * @brief Network stack interface defining required methods to support.
 */
class CcKernelSHARED INetworkStack : public CcObject
{
public:
  INetworkStack() = default;
  virtual ~INetworkStack() = default;

  /**
   * @brief Init method must have an implementation to setup network stack
   * @return True if initializing succeeded and stack is ready for use.
   */
  virtual bool init() = 0;

  /**
   * @brief Deinit network stack and cleanup.
   */
  virtual void deinit() = 0;

  /**
   * @brief Create a socket for network operations.s
   * @param eType: Type of socket to create.
   * @return Created socket from stack.
   */
  virtual ISocket* getSocket(ESocketType eType) = 0;

  /**
   * @brief Get a matching interface for targeting ip
   * @param oIp:  Ip to search for connection.
   * @return Matching interface or nullptr if no interface maching the condions.
   */
  virtual CcIpInterface* getInterfaceForIp(const CcIp& oIp) = 0;
};
