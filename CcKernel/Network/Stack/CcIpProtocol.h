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
#ifndef H_CcIpProtocol_H_
#define H_CcIpProtocol_H_

#include <Network/Stack/INetworkProtocol.h>
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcIp.h"

class CcTcpProtocol;
class CcUdpProtocol;
class CcIcmpProtocol;

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
    uint8 uiTypeOfService;              //! TOS
    uint16 uiTotalLength;               //! packet len
    uint16 uiIdOfFragment;              //! identifiere of fragment
    uint16 uiFragmentOffset;            //! offset
    uint8 uiTimeToLive;                 //! time to live
    uint8 uiProtocol;                   //! next protocol - here icmp or tcp
    uint16 uiHeaderCksum;               //! ip header checksum
    uint8 puiSourceAddress[4];          //! ip source address
    uint8 puiDestAddress[4];            //! ip destination address


    uint8 getVersion()
      { return (uiVersionAndIpHeaderLength & 0xf0) >> 4; }
    uint8 getHeaderLength()
      { return (uiVersionAndIpHeaderLength & 0x0f)*sizeof(uint32); }
    uint16 getTotalLength()
      { return CcStatic::swapUint16(uiTotalLength); }
    uint16 getContentLength()
      { return CcStatic::swapUint16(uiTotalLength) - getHeaderLength(); }
    uint16 getIdentifier()
      { return CcStatic::swapUint16(uiIdOfFragment); }
    uint16 getFragmentOffset()
      { return CcStatic::swapUint16(uiFragmentOffset); }
    uint8 getTimeToLive()
      { return uiTimeToLive; }
    uint8 getProtocol()
      { return uiProtocol; }
    CcIp getSource()
      { return CcIp(puiSourceAddress, true); }
    CcIp getDestination()
      { return CcIp(puiDestAddress, true); }
    void generateChecksum();
  };
#pragma pack(pop)

public:
  CcIpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcIpProtocol();

  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  static uint16 generateChecksum(uint16* pData, size_t uiSize);
  static uint16 generateChecksumSwapped(uint16* pData, size_t uiSize);

private:
  CcIpProtocol(const CcIpProtocol& oToCopy) = delete;
  CcIpProtocol(CcIpProtocol&& oToMove) = delete;

  static uint16 s_uiId;
  CcTcpProtocol* m_pTcpProtocol = nullptr;
  CcUdpProtocol* m_pUdpProtocol = nullptr;
  CcIcmpProtocol* m_pIcmpProtocol = nullptr;
};

#endif //H_CcIpProtocol_H_
