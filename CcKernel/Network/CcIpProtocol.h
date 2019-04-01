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
 * @subpage   CcIpProtocol
 *
 * @page      CcIpProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIpProtocol
 */
#ifndef _CcIpProtocol_H_
#define _CcIpProtocol_H_

#include <Network/INetworkProtocol.h>
#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcIp.h"

class CcKernelSHARED CcIpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for ip header
   */
  class CHeader
  {
  public:
    uint8 uiVersionAndIpHeaderLength;   //! ip version
    uint8 uiTOS;                        //! TOS
    uint16 uiTotalLength;               //! packet len
    uint16 uiIdOfFragment;              //! identifiere of fragment
    uint16 uiFragmentOffset;            //! offset
    uint8 uiTimeToLive;                 //! time to live
    uint8 uiProtocol;                   //! next protocol - here icmp or tcp
    uint16 uiHeaderCksum;               //! ip header checksum
    uint8 puiSourceAddress[4];          //! ip source address
    uint8 puiDestAddress[4];            //! ip destination address
    uint32 ulOptions;                   //! Options
  };
#pragma pack(pop)

public:
  CcIpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcIpProtocol();

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacket* pPacket) override;
  virtual bool receive(CcNetworkPacket* pPacket) override;

  static uint8 getVersion(CHeader* pHeader)
    { return (pHeader->uiVersionAndIpHeaderLength & 0xf0) >> 4; }
  static uint8 getHeaderLength(CHeader* pHeader)
    { return (pHeader->uiVersionAndIpHeaderLength & 0x0f)*sizeof(uint32_t); }
  static uint16 getTotalLength(CHeader* pHeader)
    { return CcStatic::swapUint16(pHeader->uiTotalLength); }
  static uint16 getContentLength(CHeader* pHeader)
    { return CcStatic::swapUint16(pHeader->uiTotalLength) - getHeaderLength(pHeader); }
  static uint16 getIdentifier(CHeader* pHeader)
    { return CcStatic::swapUint16(pHeader->uiIdOfFragment); }
  static uint16 getFragmentOffset(CHeader* pHeader)
    { return CcStatic::swapUint16(pHeader->uiFragmentOffset); }
  static uint8 getTimeToLive(CHeader* pHeader)
    { return pHeader->uiTimeToLive; }
  static uint8 getProtocol(CHeader* pHeader)
    { return pHeader->uiProtocol; }
  static CcIp getSource(CHeader* pHeader)
    { return CcIp(pHeader->puiSourceAddress, true); }
  static CcIp getDestination(CHeader* pHeader)
    { return CcIp(pHeader->puiDestAddress, true); }

  static void generateChecksum(CHeader* pHeader);

private:
  CcIpProtocol(const CcIpProtocol& oToCopy) = delete;
  CcIpProtocol(CcIpProtocol&& oToMove) = delete;
};

#endif //_CcIpProtocol_H_
