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
 * @subpage   CcArpProtocol
 *
 * @page      CcArpProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcArpProtocol
 */
#ifndef H_CcArpProtocol_H_
#define H_CcArpProtocol_H_

#include <Network/Stack/INetworkProtocol.h>
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "CcStatic.h"

class CcIpInterface;

class CcKernelSHARED CcArpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for ARP Header
   */
  class CHeader
  {
  public:
    uint16  uiMacType       = 0x100;  //!< Default ethernet type
    uint16  uiProtocolType  = 0x8;    //!< Deftaul IPv4 in Network order
    uint8   uiMacSize = 6;        //!< Default mac address with 6 bytes
    uint8   uiProtocolSize = 4;   //!< Default IPv4 size with 4 bytes
    uint16  uiOperation = 0x200;  //!< Default set reply
    uint8   puiSourceMac[6];      //!<
    uint8   puiSourceIp[4];       //!<
    uint8   puiDestinationMac[6]; //!<
    uint8   puiDestinationIp[4];  //!<

    bool isRequest(){return uiOperation == 1;}
    bool isReply(){return uiOperation == 2;}

    void setRequest(){uiOperation = 1;}
    void setReply(){uiOperation = 2;}
  };
#pragma pack(pop)

public:
  CcArpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcArpProtocol();

  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  void queryMac(const CcIp& oQueryIp, const CcIpInterface& oInterface);
private: // Methods
  CcArpProtocol(const CcArpProtocol& oToCopy) = delete;
  CcArpProtocol(CcArpProtocol&& oToMove) = delete;
};

#endif //H_CcArpProtocol_H_
