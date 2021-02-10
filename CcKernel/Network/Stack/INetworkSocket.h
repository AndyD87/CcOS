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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class INetworkSocket
 */
#pragma once

#include "CcBase.h"
#include "Network/ISocket.h"
#include "CcDateTime.h"

class CcNetworkStack;

/**
* @brief Generic network socket for CcNetwork stack
*/
class INetworkSocket : public ISocket
{
public:
  /**
  * @brief Constructor
  */
  INetworkSocket(CcNetworkStack* pStack, ESocketType type = ESocketType::TCP );

  /**
   * @brief Destructor
   */
  virtual ~INetworkSocket();

  /**
   * @brief Get ip-address of Hostname
   * @param hostname: Hostname as String to search for
   * @return Address info to target host
   */
  CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  virtual CcStatus setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode = ERwMode::ReadWrite) override;

  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddressInfo) override
  { m_oConnectionInfo = oAddressInfo; return true; }

  virtual CcSocketAddressInfo& getAddressInfo() override
  { return m_oConnectionInfo; }

  virtual CcSocketAddressInfo& getPeerInfo() override;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) override;

  //! @return Get local connection infos
  const CcSocketAddressInfo& getConnectionInfo()
  { return m_oConnectionInfo; }

protected:
  CcNetworkStack*     m_pStack = nullptr;   //!< Target stack for interop with
  CcSocketAddressInfo m_oConnectionInfo;    //!< Local connection info
  CcSocketAddressInfo m_oPeerInfo;          //!< Remote connection info
  CcDateTime          m_oReadTimeout = 0;   //!< Timeout option for read requests
  CcDateTime          m_oWriteTimeout = 0;  //!< Timeout option for write requests
};
