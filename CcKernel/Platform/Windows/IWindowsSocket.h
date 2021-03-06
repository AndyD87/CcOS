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
 * @subpage   IWindowsSocket
 *
 * @page      IWindowsSocket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IWindowsSocket
 */
#ifndef H_IWindowsSocket_H_
#define H_IWindowsSocket_H_

#include "CcBase.h"
#include <winsock2.h>
#include "CcWindowsGlobals.h"
#include "Network/ISocket.h"
#include <ws2tcpip.h>

/**
 * @brief Create a Socket on Windows Systems
 */
class IWindowsSocket : public ISocket
{
public:
  /**
   * @brief Constructor
   */
  IWindowsSocket( ESocketType type = ESocketType::TCP );

  IWindowsSocket(SOCKET socket, sockaddr sockAddr, int sockAddrlen);
  /**
   * @brief Destructor
   */
  virtual ~IWindowsSocket() override;

  virtual CcStatus close() override;

  /**
   * @brief Get ip-address of Hostname
   * @param hostname: Hostname as String to search for
   * @param addr [out]: found ip-address or 0.0.0.0 if failed
   * @return
   */
  CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  virtual CcStatus setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode = ERwMode::ReadWrite) override;
  
  virtual CcSocketAddressInfo& getAddressInfo() override
  { return m_oConnectionInfo; }

  virtual CcSocketAddressInfo& getPeerInfo() override;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  SOCKETFD getSocketFD() override {return static_cast<SOCKETFD>(m_hClientSocket);}

  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) override;

private:
  static void startWSA();

protected:
  SOCKET        m_hClientSocket;      //!< ClientSocket generated by InitSocket
  CcSocketAddressInfo m_oConnectionInfo;
  CcSocketAddressInfo m_oPeerInfo;

private:
  static bool   g_sWsaStarted;
  static CRITICAL_SECTION m_CS;
};

#endif // H_IWindowsSocket_H_
