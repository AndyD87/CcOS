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
 * @subpage   CcWindowsSocketBase
 *
 * @page      CcWindowsSocketBase
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsSocketBase
 */
#ifndef _CcWindowsSocketBase_H_
#define _CcWindowsSocketBase_H_

#include "CcBase.h"
#include <winsock2.h>
#include "CcWindowsGlobals.h"
#include "Network/ISocket.h"
#include <ws2tcpip.h>

/**
 * @brief Create a Socket on Windows Systems
 */
class CcWindowsSocketBase : public ISocket
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsSocketBase( ESocketType type = ESocketType::TCP );

  CcWindowsSocketBase(SOCKET socket, sockaddr sockAddr, int sockAddrlen);
  /**
   * @brief Destructor
   */
  virtual ~CcWindowsSocketBase();

  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  /**
   * @brief read with timeout
   */
  virtual size_t readTimeout(char *buf, size_t bufSize, time_t timeout) = 0;

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

  SOCKETFD getSocketFD() override {return static_cast<SOCKETFD>(m_ClientSocket);}

  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) override;

private:
  static void startWSA();

protected:
  SOCKET        m_ClientSocket;      //!< ClientSocket generated by InitSocket
  CcSocketAddressInfo m_oConnectionInfo;
  CcSocketAddressInfo m_oPeerInfo;

private:
  static bool   g_sWsaStarted;
  static CRITICAL_SECTION m_CS;
};

#endif /* _CcWindowsSocketBase_H_ */
