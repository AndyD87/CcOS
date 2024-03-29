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
 * @brief     Class ILwipSocket
 */
#pragma once

#include "CcBase.h"
#include "Network/ISocket.h"
CCEXTERNC_BEGIN
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
CCEXTERNC_END

#ifdef bind
  #undef bind
#endif // bind

#ifdef connect
  #undef connect
#endif // connect

#ifdef listen
  #undef listen
#endif // listen

#ifdef accept
  #undef accept
#endif // accept

#ifdef close
  #undef close
#endif // close

#ifdef shutdown
  #undef shutdown
#endif // shutdown

#ifdef read
  #undef read
#endif // read

#ifdef write
  #undef write
#endif // write

/**
 * @brief Create a Socket on Linux Systems
 */
class ILwipSocket : public ISocket
{
public:
  /**
   * @brief Constructor
   * @param type: Target socket type.
   */
  ILwipSocket( ESocketType type);

  /**
   * @brief Initialize socket with already available system data
   * @param socket:       Socket handle
   * @param sockAddr:     Socket address info
   * @param sockAddrlen:  Socket address info length
   */
  ILwipSocket(int socket, sockaddr sockAddr, uint32 sockAddrlen);

  virtual ~ILwipSocket();

  virtual CcSocketAddressInfo getHostByName(const CcString& sHostname) override;

  virtual CcStatus setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode = ERwMode::ReadWrite) override;

  virtual CcSocketAddressInfo& getAddressInfo() override
  { return m_oConnectionInfo; }

  virtual CcSocketAddressInfo& getPeerInfo() override;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) override;

  virtual SOCKETFD getSocketFD() override { return m_hClientSocket; }

protected:
  int                 m_hClientSocket = -1; //!< Socket handle generated by inheriting class
  CcSocketAddressInfo m_oConnectionInfo;    //!< Connection info on local device
  CcSocketAddressInfo m_oPeerInfo;          //!< Connection info of remote service
};
