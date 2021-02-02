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
 * @brief     Class CcUdpProtocol
 */
#pragma once

#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "CcIp.h"

class CcNetworkSocketUdp;

/**
 * @brief Udp protocol implementation for CcOS Network stack.
 */
class CcKernelSHARED CcUdpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for TCP Header
   */
  class CcKernelSHARED CHeader
  {
  public:
    uint16 uiSrcPort = 0;  //!< udp source port
    uint16 uiDestPort = 0; //!< udp destinaltion port
    uint16 uiLength = 0;   //!< udp package length with header
    uint16 uiChecksum = 0; //!< udp checksum

    //! @return Get source port already swapped
    uint16 getSourcePort()
    { return CcStatic::swapInt16(uiSrcPort); }
    //! @return Get destination port already swapped
    uint16 getDestinationPort()
    { return CcStatic::swapInt16(uiDestPort); }
    //! @return Get length of udp packet already swapped
    uint16 getLength()
    { return CcStatic::swapInt16(uiLength); }
    //! @return Get checksum already swapped
    uint16 getChecksum()
    { return CcStatic::swapInt16(uiChecksum); }

    //! @param uiPort: Set source port and swap it
    void setSourcePort(uint16 uiPort)
    { uiSrcPort = CcStatic::swapInt16(uiPort); }
    //! @param uiPort: Set destination port and swap it
    void setDestinationPort(uint16 uiPort)
    { uiDestPort = CcStatic::swapInt16(uiPort); }
    //! @param uiNewLength: Set packet length and swap it
    void setLength(uint16 uiNewLength)
    { uiLength = CcStatic::swapInt16(uiNewLength); }

    /**
     * @brief Generate checksum for this packet and use seperate source and
     *        destination ip for calculation.
     * @param oDestIp:    Overwrite destination ip with this
     * @param oSourceIp:  Overwrite source ip with this
     */
    void generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp);
  };
#pragma pack(pop)
public:
  /**
   * @brief Create udp protocol object with parent protocol
   * @param pParentProtocol: Parent protocol to get packets from and send packets to
   */
  CcUdpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcUdpProtocol();

  /**
   * @brief Initialize udp functionality
   * @return True if udp was setup well
   */
  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  /**
   * @brief Register UdpSocket type for applications.
   *        It will receives packets if listen.
   * @param pSocket
   * @return
   */
  CcStatus registerSocket(CcNetworkSocketUdp* pSocket);

  /**
   * @brief Remove listening socket from protocol
   * @param pSocket: Socket handle to remove
   * @return True if socket was registered
   */
  bool removeSocket(CcNetworkSocketUdp* pSocket);

private: // Types
  class CPrivate;
private: // Methods
  CcUdpProtocol(const CcUdpProtocol& oToCopy) = delete;
  CcUdpProtocol(CcUdpProtocol&& oToMove) = delete;
private: // Member
  CPrivate* m_pPrivate;
};
