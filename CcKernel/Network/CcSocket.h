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
 * @page      Network
 * @subpage   CcSocket
 *
 * @page      CcSocket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSocket
 */
#ifndef H_CcSocket_H_
#define H_CcSocket_H_

#include "Network/ISocket.h"
#include "CcBase.h"
#include "CcBase.h"
#include "IIo.h"
#include "CcTypes.h"
#include "CcSharedPointer.h"
#include "CcMutex.h"

class CcSocketAddressInfo;
class CcString;
class CcDateTime;

#ifdef _MSC_VER
template class CcKernelSHARED CcSharedPointer<ISocket>;
#endif

/**
 * @brief Button for GUI Applications
 */
class CcKernelSHARED CcSocket : public ISocket
{
public:
  /**
   * @brief Constructor
   */
  CcSocket();

  /**
   * @brief Constructor
   *        Create Socket of specified type.
   * @param eType
   */
  CcSocket(ESocketType eType);

  /**
   * @brief Constructor
   */
  CcSocket(ISocket* pSocketImport);

  /**
   * @brief CopyConstructor
   */
  CcSocket(const CcSocket& oToCopy);

  /**
   * @brief MoveConstructor
   */
  CcSocket(CcSocket&& oToMove);

  /**
   * @brief Destructor
   */
  virtual ~CcSocket() override;

  CcSocket& operator=(ISocket* pToSet);
  CcSocket& operator=(CcSocket&& oToMove);
  CcSocket& operator=(const CcSocket& oToCopy);
  inline bool operator==(const CcSocket& oToCompare) const
    { return oToCompare.m_pSystemSocket == m_pSystemSocket; }
  inline bool operator!=(const CcSocket& oToCompare) const
    { return !operator==(oToCompare); }

  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param pBuffer: Buffer to load data to.
   * @param uSize: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  virtual size_t read(void* pBuffer, size_t uSize) override;

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param pBuffer: Buffer to load data from.
   * @param uSize: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  virtual size_t write(const void* pBuffer, size_t uSize) override;

  /**
   * @brief Open Device in a specific mode.
   *        For more informations lock at: @ref EOpenFlags
   * @return true if Device was opened successfully.
   */
  virtual CcStatus open(EOpenFlags eOpenFlags = EOpenFlags::NoFlag) override;

  /**
   * @brief Close the connection to device.
   * @return true if Connection was successfully closed.
   */
  virtual CcStatus close() override;

  /**
   * @brief Cancel Current Operation.
   *        It can optionally be Implemented from inheriting Device.
   *        Look at device definintion it it supports canceling.
   * @return true if Opperation was aborted successfully.
   */
  virtual CcStatus cancel() override;


  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddressInfo) override;

  virtual CcSocketAddressInfo& getAddressInfo() override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus bind(uint16 Port);

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual CcStatus bind() override;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus bind(const CcSocketAddressInfo& oAddressInfo);

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual CcStatus connect() override;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus connect(const CcSocketAddressInfo& oAddressInfo);

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

  virtual CcStatus setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode = ERwMode::ReadWrite) override;

  virtual CcSocketAddressInfo getHostByName(const CcString& hostname) override;

  virtual CcSocketAddressInfo& getPeerInfo() override;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override;

  virtual CcStatus setOption(ESocketOption eOption, void* pData = nullptr, size_t uiDataLen = 0) override;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData = nullptr, size_t uiDataLen = 0) override;


  /**
   * @brief connect to Host with known Name in Network and Port
   *        can be overloaded if System can connect by name.
   * @param hostName: Name of Host to connect to
   * @param hostPort: Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  CcStatus connect(const CcString& hostName, const CcString& hostPort);

  virtual SOCKETFD getSocketFD() override { return 0; }

  ISocket* getRawSocket()
  { return m_pSystemSocket.ptr(); }
  bool isValid()
  { return m_pSystemSocket != nullptr; }
  void lock()
  { m_oLock.lock(); }
  void unlock()
  { m_oLock.unlock(); }

private:
  CcSharedPointer<ISocket> m_pSystemSocket = nullptr;
  CcMutex                  m_oLock;
  CcMutex                  m_oCloseLock;

};

#endif // H_CcSocket_H_
