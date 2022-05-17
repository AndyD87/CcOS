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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ISocket
 */
#pragma once

#include "Network/CcSocketAddressInfo.h"
#include "CcBase.h"
#include "IIo.h"
#include "CcTypes.h"
#include "CcString.h"

class CcDateTime;

/**
 * @brief Known socket types to create
 */
enum class ESocketType
{
  Unknown = 0,
  TCP,
  UDP,
};

/**
 * @brief Known socket option to adjust socket operations.
 */
enum class ESocketOption
{
  Reuse,        //!< Send ReuseAddress and ReusePort
  ReuseAddress, //!< Do not block address after crash
  ReusePort,    //!< Do not block port after crash
  Broadcast,    //!< This Option enables Broadcast by default, for disable, send int32 with 0 as Data
  DontRoute,    //!< This Option enables Broadcast by default, for disable, send int32 with 0 as Data
  BindToDevice  //!< This Option bind traffic to a specific adapter by name, parameter is a CcString
};

/**
 * @brief Socket interface for network operations.
 */
class CcKernelSHARED ISocket : public IIo
{
public:
  /**
   * @brief Create socket and set specific type
   * @param pType: Type of socket.
   */
  ISocket(ESocketType pType): m_eSocketType(pType)
  {}

  /**
   * @brief Destructor
   */
  virtual ~ISocket() = default;

  /**
   * @brief Set address info of local device to bind or listen to.
   * @param oAddrInfo: Adress info.
   * @return Status of operation.
   */
  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) = 0;

  /**
   * @brief Get addressinfo which was previosly set by setAddressInfo or determined
   *        by connecting to another service.
   * @return Local address info.
   */
  virtual CcSocketAddressInfo& getAddressInfo() = 0;

  /**
   * @brief Bind socket to local address for listening.
   * @return Status of operation.
   */
  virtual CcStatus bind() = 0;

  /**
   * @brief Connect to another socket, stored in peer socket.
   * @return Status of operation.
   */
  virtual CcStatus connect() = 0;

  /**
   * @brief Socket becomes a host and listen on Port
   * @return Status of operation.
   */
  virtual CcStatus listen() = 0;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  virtual ISocket* accept() = 0;

  /**
   * @brief Set timeout for blocking operations.
   * @param uiTimeValue:  Timeout in date time format
   * @param eMode:        Mode of blocking operations to set timeout.
   * @return Status of operation.
   */
  virtual CcStatus setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode = ERwMode::ReadWrite) = 0;

  /**
   * @brief Get currently set peer info. It might be set by setPeerInfo, or it was determined
   *        by incoming connections.
   * @return Socket address info of peer
   */
  virtual CcSocketAddressInfo& getPeerInfo() = 0;

  /**
   * @brief Set peer info for connection to another services.
   * @param oPeerInfo: Target address of connections.
   */
  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) = 0;

  /**
   * @brief Get host by name. Research for network devices.
   * @param sHostname: Target hostname to search for.
   * @return Found network, or empty if not.
   */
  virtual CcSocketAddressInfo getHostByName(const CcString& sHostname) = 0;

  /**
   * @brief Set known socket options like multicast, broadcast.
   * @param eOption:    Option as enum
   * @param pData:      Data for sending and receiving if required by eOption
   * @param uiDataLen:  Size of pData.
   * @return Status of operation.
   */
  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) = 0;

  /**
   * @brief Set raw socket options like multicast, broadcast. They can be differnt from system to system.
   * @param iLevel:     Target level to hit correct protocol.
   * @param iOptName:   Option as id.
   * @param pData:      Data for sending and receiving if required by eOption
   * @param uiDataLen:  Size of pData.
   * @return Status of operation.
   */
  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) = 0;

  /**
   * @brief Sometimes it is usefull to get socket handle of underlying socket for
   *        System operations.
   * @return System specific socket id.
   */
  virtual SOCKETFD getSocketFD() { return 0; }

  //! @return Type of this socket
  inline ESocketType getType() const
  { return m_eSocketType; }

  /**
   * @brief Check if data is available for read
   * @return SUCCESS if data is available
   */
  virtual CcStatus dataAvailable()
  { return EStatus::CommandNotImplemented; }

protected:
  //! @param eSocketType: Change basic sockettype.
  void setSocketType(ESocketType eSocketType)
  { m_eSocketType = eSocketType; }

private:
  ESocketType m_eSocketType; //!< Type of this socket.
};
