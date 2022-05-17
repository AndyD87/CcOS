/**
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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class CcLinuxSocketUdp
 */
#pragma once

#include "CcBase.h"
#include "ILinuxSocket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
* @brief Create a Socket on Linux Systems
*/
class CcLinuxSocketUdp : public ILinuxSocket
{
public:
  /**
  * @brief Constructor
  */
  CcLinuxSocketUdp();

  /**
   * @brief Initialize Socket with existing data
   * @param socket:       Already created socket
   * @param sockAddr:     Local peer info
   * @param sockAddrlen:  Length of @p sockAddr
   */
  CcLinuxSocketUdp(int socket, sockaddr sockAddr, int sockAddrlen);

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxSocketUdp();

  virtual CcStatus open(EOpenFlags eFlags = EOpenFlags::NoFlag) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddressInfo) override;

  /**
   * @brief Bind to local interface set by address info
   * @return True if bind was succeeded
   */
  virtual CcStatus bind() override;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @return true if connection was successfully established
   */
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
   * @brief Send data to established socket-connection
   * @param buf: Buffer of data to be send
   * @param bufSize: size of buffer to send
   * @return return true if transmission succeeded.
   */
  virtual size_t write(const void *buf, size_t bufSize) override;

  /**
   * @brief Receive incoming data from socket
   * @param buf: Buffer to store received data.
   * @param bufSize: maximum size of buffer;
   * @return Size of data read from socket.
   */
  virtual size_t read(void *buf, size_t bufSize) override;

};
