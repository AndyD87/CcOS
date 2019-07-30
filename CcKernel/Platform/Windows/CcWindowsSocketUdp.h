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
 * @page      Windows
 * @subpage   CcWindowsSocketUdp
 *
 * @page      CcWindowsSocketUdp
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsSocketUdp
 */
#ifndef H_CcWindowsSocketUdp_H_
#define H_CcWindowsSocketUdp_H_

#include "CcBase.h"
#include <winsock2.h>
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

  CcWindowsSocketUdp(SOCKET socket, sockaddr sockAddr, int sockAddrlen);
  /**
   * @brief Destructor
   */
  virtual ~CcWindowsSocketUdp();

  CcStatus open(EOpenFlags eOpenFlags = EOpenFlags::NoFlag) override;
  CcStatus close() override;
  CcStatus cancel() override;

  CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus bind() override;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus connect() override;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @return true if port is successfully initiated.
   */
  CcStatus listen() override;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  ISocket* accept() override;

  /**
   * @brief Receive incoming data from socket
   * @param buf: Buffer to store received data.
   * @param bufSize: maximum size of buffer;
   * @return Size of data read from socket.
   */
  size_t read(void *buf, size_t bufSize) override;

  /**
   * @brief Send data to established socket-connection
   * @param buf: Buffer of data to be send
   * @param bufSize: size of buffer to send
   * @return return true if transmission succeeded.
   */
  size_t write(const void *buf, size_t bufSize) override;

  /**
   * @brief read with timeout
   */
  size_t readTimeout(void *buf, size_t bufSize, const CcDateTime& oTimeout);
};

#endif /* H_CcWindowsSocketUdp_H_ */
