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

  /**
   * @brief open is not implemented,
   *        please use connect, bind and listen.
   * @return false
   */
  CcStatus open(EOpenFlags) override;

  /**
   * @brief Close current socket
   * @return true
   */
  CcStatus close() override;

  /**
   * @brief Cancel current socket operations.
   *        This will close current socket.
   * @return true
   */
  CcStatus cancel() override;

  /**
   * @brief Write data to current socket.
   * @param pBuffer: Pointer to Buffer with containing data.
   * @param uBufferSize: Size of pBuffer to transfer.
   * @return Number of Bytes written, or SIZE_MAX if failed
   */
  size_t write(const void *pBuffer, size_t uBufferSize) override;

  /**
   * @brief Read data from current socket.
   * @param pBuffer: Pointer to Buffer for storing data.
   * @param uBufferSize: Size of pBuffer to transfer.
   * @return Number of Bytes read, or SIZE_MAX if failed
   */
  size_t read(void *pBuffer, size_t uBufferSize) override;

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

  /**
   * @brief Get a host address information by name
   * @param sHostname: Hostname to search information for
   * @return Found information data
   */
  CcSocketAddressInfo getHostByName(const CcString& sHostname) override;

  /**
   * @brief Set socket timeout for read and write.
   * @param uiTimeValue: Timeout value
   */
  void setTimeout(const CcDateTime& uiTimeValue) override;

  /**
   * @brief Get infor of incoming peer.
   * @return Host information of incoming peer
   */
  virtual CcSocketAddressInfo getPeerInfo(void) override;

  /**
   * @brief Overwrite peer information, does not have any effect on connection.
   * @param oPeerInfo: PeerInfo to set.
   */
  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  /**
   * @brief Set a common known Option to this socket.
   * @param eOption:  Option Type as enum
   * @param pData:    Pointer to data if option requires data, otherwise nullptr.
   * @param uiDataLen:Sizte of pData
   * @return Status of option request
   */
  virtual CcStatus setOption(ESocketOption eOption, void* pData = nullptr, size_t uiDataLen = 0) override;

  /**
   * @brief Send a option like setsockopt would do.
   * @param iLevel: The level at which the option is defined (for example, SOL_SOCKET).
   * @param iOptName: Socket option number, see defines with SO_
   * @param pData: Check corresponding iOptName for required Option data.
   * @param uiDataLen: size of pData
   * @return Status of option request
   */
  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData = nullptr, size_t uiDataLen = 0) override;

  /**
   * @brief For Ssl there is a difference in initializing a socket as server
   *        or client.
   *
   *        This Method initializes Socket as Server.
   * @return true if all succeeded
   */
  bool initServer();

  /**
   * @brief For Ssl there is a difference in initializing a socket as server
   *        or client.
   *
   *        This Method initializes Socket as Client.
   * @return true if all succeeded
   */
  bool initClient();

  /**
   * @brief Load key for socket communication from file.
   * @param sPathToKey: Path to file wich contains the key.
   * @return true if Key was successfully read.
   */
  bool loadKey(const CcString& sPathToKey);

  /**
   * @brief Load certificate for socket communication from file.
   * @param sPathToKey: Path to file wich contains the key.
   * @return true if certificate was successfully read.
   */
  bool loadCertificate(const CcString& sPathToKey);

  /**
   * @brief Deinitialize Socket, it is never mined if it was initialized by initServer or initClient
   */
  void deinit();
  
private:
  bool finalizeAccept();
private:
  CcSslSocketPrivate* m_pPrivate = nullptr; //<! Private data
};

#endif /* _CcSslSocket_H_ */
