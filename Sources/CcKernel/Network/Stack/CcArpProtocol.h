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
 * @brief     Class CcArpProtocol
 */
#pragma once

#include <Network/Stack/INetworkProtocol.h>
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "CcStatic.h"

class CcIpInterface;

/**
 * @brief Arp network protocol service.
 *        This protocol will filter all arp pakets and handle them.
 *        Requests from system will be handled too.
 */
class CcKernelSHARED CcArpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief ARP Header type
   */
  class CHeader
  {
  public:
    uint16  uiMacType       = 0x100;  //!< Default ethernet type
    uint16  uiProtocolType  = 0x8;    //!< Deftaul IPv4 in Network order
    uint8   uiMacSize = 6;            //!< Default mac address with 6 bytes
    uint8   uiProtocolSize = 4;       //!< Default IPv4 size with 4 bytes
    uint16  uiOperation = 0x200;      //!< Default set reply
    uint8   puiSourceMac[6];          //!< Source Physical Address
    uint8   puiSourceIp[4];           //!< Source IP Address
    uint8   puiDestinationMac[6];     //!< Destination Physical Address
    uint8   puiDestinationIp[4];      //!< Destination IP Address

    //! @return True if casted packet is a request
    bool isRequest(){return uiOperation == 1;}
    //! @return True if casted packet is a response
    bool isReply(){return uiOperation == 2;}

    //! @brief Mark this paket as request
    void setRequest(){uiOperation = 1;}
    //! @brief Mark this paket as response
    void setReply(){uiOperation = 2;}
  };
#pragma pack(pop)

public:
  /**
   * @brief Setup Arp Protocol Service and insert it to parent
   * @param pParentProtocol: Handle to next protocol interface
   */
  CcArpProtocol(INetworkProtocol* pParentProtocol);

  /**
   * @brief Destructor
   */
  virtual ~CcArpProtocol();

  /**
   * @brief Initialize this protocol
   * @return True if init succeeded
   */
  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  /**
   * @brief Send an arp query
   * @param oQueryIp:   IP to query for
   * @param oInterface: Interface to send query to
   */
  void queryMac(const CcIp& oQueryIp, const CcIpInterface& oInterface);
private: // Methods
  CcArpProtocol(const CcArpProtocol& oToCopy) = delete;
  CcArpProtocol(CcArpProtocol&& oToMove) = delete;
};
