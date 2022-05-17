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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class LwipSocketUdp
 */
#pragma once

#include "CcBase.h"
#include "ILwipSocket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
* @brief Create a socket on ginux systems
*/
class LwipSocketUdp : public ILwipSocket
{
public:
  /**
  * @brief Constructor
  */
  LwipSocketUdp();

  /**
   * @brief Create socket with existing socket from system
   * @param socket:       Socket handle
   * @param sockAddr:     Connection info
   * @param sockAddrlen:  Size of @p sockAddr
   */
  LwipSocketUdp(int socket, sockaddr sockAddr, int sockAddrlen);

  /**
   * @brief Destructor
   */
  virtual ~LwipSocketUdp();

  virtual CcStatus open(EOpenFlags eFlags = EOpenFlags::NoFlag) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddressInfo) override;
  virtual CcStatus bind() override;
  virtual CcStatus connect() override;
  virtual CcStatus listen() override;
  virtual ISocket* accept() override;
  virtual size_t write(const void *buf, size_t bufSize) override;
  virtual size_t read(void *buf, size_t bufSize) override;
};
