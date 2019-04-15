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
 * @page      Network
 * @subpage   CcNetworkSocketTcp
 *
 * @page      CcNetworkSocketTcp
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcNetworkSocketTcp
 */
#ifndef _CcNetworkSocketTcp_H_
#define _CcNetworkSocketTcp_H_

#include "CcBase.h"
#include "INetworkSocket.h"

class CcNetworkPacket;
class CcTcpProtocol;

/**
* @brief Create a Socket on Linux Systems
*/
class CcNetworkSocketTcp : public INetworkSocket
{
public:
  /**
  * @brief Constructor
  */
  CcNetworkSocketTcp(CcNetworkStack* pStack);

  /**
   * @brief Destructor
   */
  virtual ~CcNetworkSocketTcp();

  virtual CcStatus open(EOpenFlags eFlags = EOpenFlags::NoFlag) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @return true if connection was successfully established
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

  /**
   * @brief read with timeout
   */
  virtual size_t readTimeout(void *buf, size_t bufSize, const CcDateTime& oTimeout) override;

  bool insertPacket(CcNetworkPacket* pPacket);

private:
  CcNetworkPacket* genNetworkPaket();
  CcNetworkSocketTcp(CcNetworkStack* pStack, CcTcpProtocol* pProtocol, CcNetworkSocketTcp* pParent);

private: // Types
  class CPrivate;
private: // Member
  CPrivate*  m_pPrivate;
};

#endif /* _CcNetworkSocketTcp_H_ */
