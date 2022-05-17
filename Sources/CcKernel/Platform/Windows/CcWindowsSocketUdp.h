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
 * @brief     Class CcWindowsSocketUdp
 */
#pragma once

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "IWindowsSocket.h"
#include <ws2tcpip.h>

/**
 * @brief Create a Socket on Windows Systems
 */
class CcWindowsSocketUdp : public IWindowsSocket
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsSocketUdp();

  /**
   * @brief Create socket with existing socket from system
   * @param socket:       Socket handle
   * @param sockAddr:     Connection info
   * @param sockAddrlen:  Size of @p sockAddr
   */
  CcWindowsSocketUdp(SOCKET socket, sockaddr sockAddr, int sockAddrlen);

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsSocketUdp();

  virtual CcStatus open(EOpenFlags eOpenFlags = EOpenFlags::NoFlag) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) override;
  virtual CcStatus bind() override;
  virtual CcStatus connect() override;
  virtual CcStatus listen() override;
  virtual ISocket* accept() override;
  virtual size_t read(void *buf, size_t bufSize) override;
  virtual size_t write(const void *buf, size_t bufSize) override;
};
