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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSocketAddressInfoAddressInfo
 */
#ifndef CcSocketAddressInfo_H_
#define CcSocketAddressInfo_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcTypes.h"
#include "CcIp.h"
#include "CcString.h"

enum class ESocketType;

union CcTypes_in_addr
{
  struct
  {
    uchar s_b1;
    uchar s_b2;
    uchar s_b3;
    uchar s_b4;
  } S_un_b;
  struct
  {
    uint16 s_w1;
    uint16 s_w2;
  } S_un_w;
  uint32 S_addr = 0;
};

class CcTypes_sockaddr_in
{
public:
  int16 sin_family = 0;
  uint16 sin_port = 0;
  CcTypes_in_addr sin_addr;
  char sin_zero[8] = { 0 };
};

/**
* @brief Button for GUI Applications
*/
class CcKernelSHARED CcSocketAddressInfo
{
public:
  /**
   * @brief Constructor
   */
  CcSocketAddressInfo();

  /**
   * @brief Constructor
   */
  CcSocketAddressInfo(ESocketType eSocketType, const CcIp& rIp, uint16 uiPort);

  /**
  * @brief CopyConstructor
  */
  CcSocketAddressInfo(const CcSocketAddressInfo& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcSocketAddressInfo( void );

  CcSocketAddressInfo& operator=(const CcSocketAddressInfo& oToCopy);
  CcSocketAddressInfo& operator=(CcSocketAddressInfo&& oToMove);

  void init(ESocketType eSocketType);

  void setAddressData(CcTypes_sockaddr_in *pData, size_t uiSizeofData);

  void setPort(const CcString& sPort)
    { setPort(sPort.toUint16());}
  void setPort(uint16 uiPort);

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
  void setIpPort(const CcString& sIpString, bool* pbOk = nullptr);
  inline void setIpPort(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0, uint16 uiPort)
    { setIpV4(uiIp3, uiIp2, uiIp1, uiIp0); setPort(uiPort); }

  CcIp getIp(void) const;
  CcString getIpString() const
    { return getIp().getString(); }
  uint16 getPort(void) const;
  CcString getPortString() const;
  
  CcTypes_sockaddr_in* sockaddr()
    { return ai_addr; }
  const CcTypes_sockaddr_in* getSockaddr() const
    { return ai_addr; }

  static uint16 htons(uint16 uiToSwap);
public:
  int                  ai_flags = 0;
  int                  ai_family = 0;
  int                  ai_socktype = 0;
  int                  ai_protocol = 0;
  size_t               ai_addrlen = 0;
  char*                ai_canonname = nullptr;
  CcTypes_sockaddr_in *ai_addr = nullptr;
  CcSocketAddressInfo *ai_next = nullptr;
};

#endif /* CcSocketAddressInfo_H_ */
