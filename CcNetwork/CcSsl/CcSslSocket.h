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
#ifndef H_CcSslSocket_H_
#define H_CcSslSocket_H_

#include "CcBase.h"
#include "CcSsl.h"
#include "Network/ISocket.h"

/**
 * @brief Button for GUI Applications
 */
class CcSslSHARED CcSslSocket : public ISocket
{
public:
  /**
   * @brief Constructro
   */
  CcSslSocket();

  /**
   * @brief Constructor
   */
  CcSslSocket(ISocket* pParentSocket);

  CcSslSocket(CcSslSocket&& rSocket)
    {operator=(CCMOVE(rSocket));}

  /**
   * @brief Destructor
   */
  virtual ~CcSslSocket();

  void operator=(CcSslSocket&& pToMove);

  /**
   * @brief open is not implemented,
   *        please use connect, bind and listen.
   * @return false
   */
  virtual CcStatus open(EOpenFlags) override;

  /**
   * @brief Close current socket
   * @return true
   */
  virtual CcStatus close() override;

  /**
   * @brief Cancel current socket operations.
   *        This will close current socket.
   * @return true
   */
  virtual CcStatus cancel() override;

  /**
   * @brief Write data to current socket.
   * @param pBuffer: Pointer to Buffer with containing data.
   * @param uBufferSize: Size of pBuffer to transfer.
   * @return Number of Bytes written, or SIZE_MAX if failed
   */
  virtual size_t write(const void *pBuffer, size_t uBufferSize) override;

  /**
   * @brief Read data from current socket.
   * @param pBuffer: Pointer to Buffer for storing data.
   * @param uBufferSize: Size of pBuffer to transfer.
   * @return Number of Bytes read, or SIZE_MAX if failed
   */
  virtual size_t read(void *pBuffer, size_t uBufferSize) override;

  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) override;
  
  virtual CcSocketAddressInfo& getAddressInfo() override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual CcStatus bind() override;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual CcStatus connect() override;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @param Port: Value of Port-Address
   * @return true if port is successfully initiated.
   */
  virtual CcStatus listen() override;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  virtual ISocket* accept() override;

  /**
   * @brief Get a host address information by name
   * @param sHostname: Hostname to search information for
   * @return Found information data
   */
  virtual CcSocketAddressInfo getHostByName(const CcString& sHostname) override;

  /**
   * @brief Set socket timeout for read and write.
   * @param uiTimeValue: Timeout value
   */
  virtual CcStatus setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode = ERwMode::ReadWrite) override;

  /**
   * @brief Get infor of incoming peer.
   * @return Host information of incoming peer
   */
  virtual CcSocketAddressInfo& getPeerInfo() override;

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
  CcSslSocket(const CcSslSocket&) = delete;
  void operator=(const CcSslSocket&) = delete;

  bool finalizeAccept();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr; //<! Private data
};

#endif // H_CcSslSocket_H_
