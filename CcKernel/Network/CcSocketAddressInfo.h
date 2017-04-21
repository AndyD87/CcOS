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
 * @par       Web: http://adirmeier.de/CcOS
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
  uint32 S_addr;
};

struct CcTypes_sockaddr_in
{
  int16 sin_family;
  uint16 sin_port;
  CcTypes_in_addr sin_addr;
  char sin_zero[8];
};

typedef struct  {
  int             ai_flags;
  int             ai_family;
  int             ai_socktype;
  int             ai_protocol;
  size_t          ai_addrlen;
  char            *ai_canonname;
  CcTypes_in_addr *ai_addr;
  CcTypes_in_addr *ai_next;
} Cc_addrinfo;

/**
* @brief Button for GUI Applications
*/
class CcKernelSHARED CcSocketAddressInfo
{
public:
  /**
  * @brief Constructor
  */
  CcSocketAddressInfo( );

  /**
  * @brief CopyConstructor
  */
  CcSocketAddressInfo(const CcSocketAddressInfo& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcSocketAddressInfo( void );

  void setAddressData(CcTypes_sockaddr_in *pData, size_t uiSizeofData);

  void setPort(uint16 uiPort);
  void setIp(const CcString& sIpString, bool& bOk);
  void setIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0);
  void setIpPort(const CcString& sIpString, bool& bOk);
  inline void setIpPort(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0, uint16 uiPort)
    { setIp(uiIp3, uiIp2, uiIp1, uiIp0); setPort(uiPort); }

  ipv4_t getIPv4(void) const;
  CcString getIPv4String() const;
  uint16 getPort(void) const;
  CcString getPortString() const;
private:
  Cc_addrinfo m_oAddressData;
};

#endif /* CcSocketAddressInfo_H_ */
