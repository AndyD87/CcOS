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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSslSocket
 */
#ifndef CcSslSocket_H_
#define CcSslSocket_H_

#include "CcBase.h"
#include "CcSsl.h"
#include "Network/CcSocket.h"
#include "CcSslData.h"

class CcSslSocketPrivate;

 /**
 * @brief Button for GUI Applications
 */
class CcSslSHARED CcSslSocket : public CcSocket
{
public:
  /**
   * @brief Constructro
   */
  CcSslSocket(void);
  
  /**
   * @brief Constructor
   */
  CcSslSocket(CcSocket* pParentSocket);

  /**
   * @brief Destructor
   */
  virtual ~CcSslSocket( void );


  bool open(EOpenFlags) override;
  bool close() override;
  bool cancel() override;
  size_t write(const char *buf, size_t bufSize) override;
  size_t read(char *buf, size_t bufSize) override;

  /**
  * @brief connect to Host with known IP-Address and Port
  * @param ipAdress: IpAddress of Host
  * @param Port:     Port where host ist waiting for connection
  * @return true if connection was successfully established
  */
  bool bind(uint16 Port) override;

  /**
  * @brief connect to Host with known Name in Network and Port
  * @param hostName: Name of Host to connect to
  * @param Port:     Port where host ist waiting for connection
  * @return true if connection was successfully established
  */
  bool connect(const CcSocketAddressInfo& oAddressInfo) override;

  bool connect(const CcString& hostName, const CcString& hostPort) override;
  /**
  * @brief Socket becomes a Host and listen on Port
  * @param Port: Value of Port-Address
  * @return true if port is successfully initiated.
  */
  bool listen(void) override;

  /**
  * @brief Waiting for an incoming connection.
  * @return Valid socket if connection established, otherwise 0.
  */
  CcSocket* accept(void) override;

  CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  void setTimeout(const CcDateTime& uiTimeValue) override;
  bool initServer();
  bool initClient();
  bool loadKey(const CcString& sPathToKey);
  bool loadCertificate(const CcString& sPathToKey);

private:
  bool finalizeAccept();
private:
  CcSslSocketPrivate* m_pPrivate = nullptr;
};

#endif /* CcSslSocket_H_ */
