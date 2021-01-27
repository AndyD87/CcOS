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
 * @brief     Class CcIpProtocol
 */
#pragma once

#include <Network/Stack/INetworkProtocol.h>
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcIp.h"

class CcTcpProtocol;
class CcUdpProtocol;
class CcIcmpProtocol;

/**
 * @brief Ip network protocol.
 *        Common protocol for network communications based on ethernet.
 */
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
    uint8 uiVersionAndIpHeaderLength;   //!< ip version
    uint8 uiTypeOfService;              //!< TOS
    uint16 uiTotalLength;               //!< packet len
    uint16 uiIdOfFragment;              //!< identifiere of fragment
    uint16 uiFragmentOffset;            //!< offset
    uint8 uiTimeToLive;                 //!< time to live
    uint8 uiProtocol;                   //!< next protocol - here icmp or tcp
    uint16 uiHeaderCksum;               //!< ip header checksum
    uint8 puiSourceAddress[4];          //!< ip source address
    uint8 puiDestAddress[4];            //!< ip destination address

    //! @return Version of ip header, already shifted
    uint8 getVersion()
    { return (uiVersionAndIpHeaderLength & 0xf0) >> 4; }
    //! @return Get Header length in bytes, version is masked out.
    uint8 getHeaderLength()
    { return (uiVersionAndIpHeaderLength & 0x0f)*sizeof(uint32); }
    //! @return Get total lenght of ip packet
    uint16 getTotalLength()
    { return CcStatic::swapUint16(uiTotalLength); }
    //! @return Get legth of content
    uint16 getContentLength()
    { return CcStatic::swapUint16(uiTotalLength) - getHeaderLength(); }
    //! @return Get identifier for packet order
    uint16 getIdentifier()
    { return CcStatic::swapUint16(uiIdOfFragment); }
    //! @return Get offset of fragments already swapped
    uint16 getFragmentOffset()
    { return CcStatic::swapUint16(uiFragmentOffset); }
    //! @return Get time to live until it packet get's rejected
    uint8 getTimeToLive()
    { return uiTimeToLive; }
    //! @return Get identifier for next protocol already swapped
    uint8 getProtocol()
    { return uiProtocol; }
    //! @return Get source ip address
    CcIp getSource()
    { return CcIp(puiSourceAddress, true); }
    //! @return Get destination ip
    CcIp getDestination()
    { return CcIp(puiDestAddress, true); }

    /**
     * @brief Generate checksum over the whole header.
     */
    void generateChecksum();
  };
#pragma pack(pop)

public:
  /**
   * @brief Initialize ip protocol with ethernet as parent protocol
   * @param pParentProtocol: Parent protocol, default ethernet.
   */
  CcIpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcIpProtocol();

  /**
   * @brief Initialize protocol
   * @return True if protocol will work without any errors.
   */
  bool init();

  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  /**
   * @brief Generate a checksum over an array of uint16 values.
   * @param pData:  Array to generate checksum from
   * @param uiSize: Number of bytes in pData, not the number of elements in array.
   * @return Generated Checksum
   */
  static uint16 generateChecksum(uint16* pData, size_t uiSize);

  /**
   * @brief Generate a checksum over an array of uint16 values.
   *        Every uin16 value gets swapped before calculation.
   * @param pData:  Array to generate checksum from
   * @param uiSize: Number of bytes in pData, not the number of elements in array.
   * @return Generated Checksum
   */
  static uint16 generateChecksumSwapped(uint16* pData, size_t uiSize);

private:
  CcIpProtocol(const CcIpProtocol& oToCopy) = delete;
  CcIpProtocol(CcIpProtocol&& oToMove) = delete;

  static uint16 s_uiId;
  CcTcpProtocol* m_pTcpProtocol = nullptr;
  CcUdpProtocol* m_pUdpProtocol = nullptr;
  CcIcmpProtocol* m_pIcmpProtocol = nullptr;
};
