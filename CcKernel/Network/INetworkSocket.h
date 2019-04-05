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
 * @subpage   INetworkSocket
 *
 * @page      INetworkSocket
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class INetworkSocket
 */
#ifndef _INetworkSocket_H_
#define _INetworkSocket_H_

#include "CcBase.h"
#include "Network/ISocket.h"

/**
* @brief Create a Socket on Linux Systems
*/
class INetworkSocket : public ISocket
{
public:
  /**
  * @brief Constructor
  */
  INetworkSocket( ESocketType type = ESocketType::TCP );

  /**
   * @brief Destructor
   */
  virtual ~INetworkSocket();

  /**
   * @brief read with timeout
   */
  virtual size_t readTimeout(char *buf, size_t bufSize, time_t timeout = 10) = 0;

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

  const CcSocketAddressInfo& getConnectionInfo()
    { return m_oConnectionInfo; }

protected:
  CcSocketAddressInfo m_oConnectionInfo;
  CcSocketAddressInfo m_oPeerInfo;
};

#endif /* _INetworkSocket_H_ */
