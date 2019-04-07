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
 * @page      Linux
 * @subpage   ILinuxSocket
 *
 * @page      ILinuxSocket
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ILinuxSocket
 */
#ifndef _ILinuxSocket_H_
#define _ILinuxSocket_H_

#include "CcBase.h"
#include "Network/ISocket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
* @brief Create a Socket on Linux Systems
*/
class ILinuxSocket : public ISocket
{
public:
  /**
  * @brief Constructor
  */
  ILinuxSocket( ESocketType type = ESocketType::TCP );

  ILinuxSocket(int socket, sockaddr sockAddr, int sockAddrlen);
  /**
   * @brief Destructor
   */
  virtual ~ILinuxSocket();

  /**
   * @brief read with timeout
   */
  virtual size_t readTimeout(void *buf, size_t bufSize, const CcDateTime& oTimeout) = 0;

  /**
   * @brief Get ip-address of Hostname
   * @param hostname: Hostname as String to search for
   * @param addr [out]: found ip-address or 0.0.0.0 if failed
   * @return
   */
  CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  void setTimeout(const CcDateTime& uiTimeValue) override;

  virtual CcSocketAddressInfo getPeerInfo() override;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) override;

  virtual SOCKETFD getSocketFD() override { return m_ClientSocket; }

protected:
  int           m_ClientSocket = -1;      //!< ClientSocket generated by InitSocket
  CcSocketAddressInfo m_oConnectionInfo;
  CcSocketAddressInfo m_oPeerInfo;
};

#endif /* _ILinuxSocket_H_ */
