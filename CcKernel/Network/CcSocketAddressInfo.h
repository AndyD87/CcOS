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
 * @subpage   CcSocketAddressInfo
 *
 * @page      CcSocketAddressInfo
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSocketAddressInfoAddressInfo
 */
#ifndef _CcSocketAddressInfo_H_
#define _CcSocketAddressInfo_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcTypes.h"
#include "CcIp.h"
#include "CcString.h"
#include "CcStatic.h"

enum class ESocketType; //! forward declaration of Socket Type

#pragma pack(push, 1)
/**
 * @brief definition of in_addr for CcOS Framework to work platform
 *        independant with same data formates.
 */
union CcKernelSHARED CcTypes_in_addr
{
  /**
   * @brief Ip Address as 4 bytes
   */
  struct
  {
    uchar s_b1;
    uchar s_b2;
    uchar s_b3;
    uchar s_b4;
  } S_un_b;
  /**
   * @brief Ip Address as 2 uin16 values
   */
  struct
  {
    uint16 s_w1;
    uint16 s_w2;
  } S_un_w;
  /**
   * @brief Ip Address as single uint32 value
   */
  uint32 S_addr = 0;
};

/**
 * @brief definition of sockaddr_in for CcOS Framework to work platform
 *        independant with same data formates.
 */
class CcKernelSHARED CcTypes_sockaddr_in
{
public:
  int16 sin_family = 0;     //!< Family type of this address
  uint16 sin_port = 0;      //!< Target port of socket
  CcTypes_in_addr sin_addr; //!< Address data for socket
  uint64 sin_zero = 0;      //!< Fill up data for ipv6
};

#pragma pack(pop)
/**
 * @brief Socket Adress Informations with Ip-Address, Port and Ip Family
 */
class CcKernelSHARED CcSocketAddressInfo
{
public:
  /**
   * @brief Constructor
   */
  CcSocketAddressInfo();

  /**
   * @brief Create CcSocketAddressInfo and initialize with default value for SocketType
   * @param eSocketType: Type of socket for setting default values.
   */
  CcSocketAddressInfo(ESocketType eSocketType);

  /**
   * @brief Create CcSocketAddressInfo and initialize with default values and additionally
   *        set IP and Port.
   * @param eSocketType: Type of socket for setting default values.
   * @param rIp: Set I -Data
   * @param uiPort: set Port Data
   */
  CcSocketAddressInfo(ESocketType eSocketType, const CcIp& rIp, uint16 uiPort);

  /**
   * @brief CopyConstructor
   * @param oToCopy: Object to copy from
   */
  CcSocketAddressInfo(const CcSocketAddressInfo& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcSocketAddressInfo();

  /**
   * @brief Move assignment operator
   * @param oToMove: Object to move from
   * @return Reference to this Object
   */
  CcSocketAddressInfo& operator=(CcSocketAddressInfo&& oToMove);

  /**
   * @brief Copy assignment operator
   * @param oToCopy: Object to copy from
   * @return Reference to this Object
   */
  CcSocketAddressInfo& operator=(const CcSocketAddressInfo& oToCopy);

  /**
   * @brief Initialze current Object with predefined Socket data.
   * @param eSocketType: Target Socket Type of this object.
   */
  void init(ESocketType eSocketType);

  /**
   * @brief Initialze current Object with predefined Socket data.
   * @param eSocketType: Target Socket Type of this object.
   */
  void setAddressData(CcTypes_sockaddr_in *pData, size_t uiSizeofData);

  /**
   * @brief Change Portnumber to new value stored in string.
   * @param sPort: New Port to set.
   */
  void setPort(const CcString& sPort)
    { setPort(sPort.toUint16());}

  /**
   * @brief Change Portnumber to new value stored in string.
   * @param sPort: New Port to set.
   */
  void setPort(uint16 uiPort);

  /**
   * @brief Change Portnumber to new value stored in string.
   * @param sPort: New Port to set.
   */
  void setIp(const CcIp& Ip);

  /**
   * @brief Set IP to current connection info
   * @param sIpString: It has to be in format like 127.0.0.1 as localhost
   * return void
   */
  void setIp(const CcString& sIp)
    { setIp(CcIp(sIp)); }

  /**
   * @brief Set IP to current connection info
   *        Example call would be setIp(127, 0, 0, 1) for localhost
   * @param uiIp3: Highest Ip value
   * @param uiIp2: Higher middle Ip value
   * @param uiIp1: Lower middle Ip value
   * @param uiIp0: Lowest Ip value
   * return void
   */
  void setIpV4(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0)
    { setIp(CcIp(uiIp3, uiIp2, uiIp1, uiIp0)); }

  /**
   * @brief Set Ip and Port defined in string of format x.x.x.x:ppp
   * @param sIpString: String to read data from
   * @param[out] pbOk: will be set to true if read succeeded, otherwise false.
   */
  void setIpPort(const CcString& sIpString, bool* pbOk = nullptr);

  /**
   * @brief Set Ip and Port in one step
   * @param uiIp3: Highest Ip value
   * @param uiIp2: Higher middle Ip value
   * @param uiIp1: Lower middle Ip value
   * @param uiIp0: Lowest Ip value
   * @param uiPort: Port value
   */
  inline void setIpPort(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0, uint16 uiPort)
    { setIpV4(uiIp3, uiIp2, uiIp1, uiIp0); setPort(uiPort); }

  /**
   * @brief Get stored Ip
   * @return ip
   */
  CcIp getIp() const;

  /**
   * @brief Get stored Ip as string
   * @return Ip as string
   */
  CcString getIpString() const
    { return getIp().getString(); }

  /**
   * @brief Get stored Port
   * @return Port
   */
  uint16 getPort() const;

  /**
   * @brief Get stored Port as string
   * @return Port as string
   */
  CcString getPortString() const;

  /**
   * @brief Get sockaddr_in buffer
   * @return pointer to buffer
   */
  inline void* sockaddr()
    { return static_cast<void*>(&ai_addr); }

  /**
   * @brief Get sockaddr_in buffer as const
   * @return pointer to buffer as const
   */
  inline const void* getSockaddr() const
    { return static_cast<const void*>(&ai_addr); }

  /**
   * @brief Swap uint16 to network byte order
   * @param uiToSwap: uint16 to swap
   * @return swapped value
   */
  static uint16 htons(uint16 uiToSwap)
    { return  CcStatic::swapUint16(uiToSwap); }

  /**
   * @brief Swap uin32 to network byte order
   * @param uiToSwap: uint32 to swap
   * @return swapped value
   */
  static uint32 htonl(uint32 uiToSwap)
    { return CcStatic::swapUint32(uiToSwap); }

public:
  int                  ai_flags = 0;    //!< Flags with info about content
  int                  ai_family = 0;   //!< Ip family
  int                  ai_socktype = 0; //!< Type of socket
  int                  ai_protocol = 0; //!< Protocol of socket
  size_t               ai_addrlen = 0;  //!< Length of ai_addr
  char*                ai_canonname = nullptr; //!< name of host if not null
  CcTypes_sockaddr_in  ai_addr;         //! stored address data with ip and port
  CcSocketAddressInfo *ai_next = nullptr; //! next Address info.
};

#endif /* _CcSocketAddressInfo_H_ */
