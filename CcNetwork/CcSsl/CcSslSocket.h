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
 * @page      CcSsl
 * @subpage   CcSslSocket
 *
 * @page      CcSslSocket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSslSocket
 */
#ifndef _CcSslSocket_H_
#define _CcSslSocket_H_

#include "CcBase.h"
#include "CcSsl.h"
#include "Network/CcSocketAbstract.h"

class CcSslSocketPrivate;

 /**
 * @brief Button for GUI Applications
 */
class CcSslSHARED CcSslSocket : public CcSocketAbstract
{
public:
  /**
   * @brief Constructro
   */
  CcSslSocket(void);
  
  /**
   * @brief Constructor
   */
  CcSslSocket(CcSocketAbstract* pParentSocket);

  /**
   * @brief Destructor
   */
  virtual ~CcSslSocket( void );


  CcStatus open(EOpenFlags) override;
  CcStatus close() override;
  CcStatus cancel() override;
  size_t write(const void *buf, size_t bufSize) override;
  size_t read(void *buf, size_t bufSize) override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus bind(const CcSocketAddressInfo& oAddrInfo) override;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus connect(const CcSocketAddressInfo& oAddressInfo) override;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @param Port: Value of Port-Address
   * @return true if port is successfully initiated.
   */
  CcStatus listen(void) override;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  CcSocketAbstract* accept(void) override;

  CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  void setTimeout(const CcDateTime& uiTimeValue) override;

  virtual CcSocketAddressInfo getPeerInfo(void) override;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  virtual CcStatus setOption(ESocketOption eOption, void* pData = nullptr, size_t uiDataLen = 0) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData = nullptr, size_t uiDataLen = 0) override;

  bool initServer();
  bool initClient();
  bool loadKey(const CcString& sPathToKey);
  bool loadCertificate(const CcString& sPathToKey);
  void deinit();
  
private:
  bool finalizeAccept();
private:
  CcSslSocketPrivate* m_pPrivate = nullptr;
};

#endif /* _CcSslSocket_H_ */
