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
 * @page      WindowsSocket
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsSocket
 */
#ifndef WindowsSocket_H_
#define WindowsSocket_H_

#include "CcBase.h"
#include <winsock2.h>
#include "WindowsGlobals.h"
#include "Network/CcSocket.h"
#include <ws2tcpip.h>

/**
* @brief Create a Socket on Windows Systems
*/
class WindowsSocket : public CcSocket
{
public:
  /**
  * @brief Constructor
  */
  WindowsSocket( ESocketType type = ESocketType::TCP );

  WindowsSocket(SOCKET socket, sockaddr sockAddr, int sockAddrlen);
  /**
   * @brief Destructor
   */
  virtual ~WindowsSocket( void );

  bool open(EOpenFlags) override
      { return true; }
  bool close() override;
  bool cancel() override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  bool bind( uint16 Port) override;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  bool connect(const CcSocketAddressInfo& oAddressInfo) override;

  /**
  * @brief connect to Host with known Name in Network and Port
  * @param hostName: Name of Host to connect to
  * @param Port:     Port where host ist waiting for connection
  * @return true if connection was successfully established
  */
  bool connect(const CcString& hostName, const CcString& hostPort) override;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @return true if port is successfully initiated.
   */
  bool listen(void) override;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  CcSocket* accept(void) override;

  /**
   * @brief Send data to established socket-connection
   * @param buf: Buffer of data to be send
   * @param bufSize: size of buffer to send
   * @return return true if transmission succeeded.
   */
  size_t write(const char *buf, size_t bufSize) override;

  /**
   * @brief Receive incoming data from socket
   * @param buf: Buffer to store received data.
   * @param bufSize: maximum size of buffer;
   * @return Size of data read from socket.
   */
  size_t read(char *buf, size_t bufSize) override;

  /**
   * @brief read with timeout
   */
  size_t readTimeout(char *buf, size_t bufSize, time_t timeout);

  /**
   * @brief Get ip-address of Hostname
   * @param hostname: Hostname as String to search for
   * @param addr [out]: found ip-address or 0.0.0.0 if failed
   * @return
   */
  CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  void setTimeout(const CcDateTime& uiTimeValue) override;

  SOCKETFD getSocketFD() override {return static_cast<SOCKETFD>(m_ClientSocket);}

private:

  static void startWSA(void);
private:
  SOCKET        m_ClientSocket;      //!< ClientSocket generated by InitSocket
  sockaddr      m_sockAddr;
  int           m_sockAddrlen;
  static bool   g_sWsaStarted;
  static CRITICAL_SECTION m_CS;
};

#endif /* WindowsSocket_H_ */
