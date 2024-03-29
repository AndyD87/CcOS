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
 * @brief     Class CcWindowsSocketTcp
 */
#pragma once

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "IWindowsSocket.h"
#include <ws2tcpip.h>

/**
 * @brief Create a Socket on Windows Systems
 */
class CcWindowsSocketTcp : public IWindowsSocket
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsSocketTcp();

  /**
   * @brief Create socket with existing socket from system
   * @param socket:       Socket handle
   * @param sockAddr:     Connection info
   * @param sockAddrlen:  Size of @p sockAddr
   */
  CcWindowsSocketTcp(SOCKET socket, sockaddr sockAddr, int sockAddrlen);

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsSocketTcp();

  virtual CcStatus open(EOpenFlags eOpenFlags = EOpenFlags::NoFlag) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) override;
  virtual CcStatus bind() override;
  virtual CcStatus connect() override;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @return true if port is successfully initiated.
   */
  virtual CcStatus listen() override;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  virtual ISocket* accept() override;

  /**
   * @brief Receive incoming data from socket
   * @param buf: Buffer to store received data.
   * @param bufSize: maximum size of buffer;
   * @return Size of data read from socket.
   */
  virtual size_t read(void *buf, size_t bufSize) override;

  /**
   * @brief Send data to established socket-connection
   * @param buf: Buffer of data to be send
   * @param bufSize: size of buffer to send
   * @return return true if transmission succeeded.
   */
  virtual size_t write(const void *buf, size_t bufSize) override;
private:
  HANDLE m_hAbortEvent = NULL;
};
