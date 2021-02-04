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
 * @brief     Class CcTcpProtocol
 */
#pragma once

#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "CcIp.h"

class CcNetworkSocketTcp;
class CcNetworkPacket;

/**
 * @brief Tcp protocol for CcOS Networkstack to handle tcp
 *        sockets from User application
 */
class CcKernelSHARED CcTcpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for TCP Header
   */
  class CcKernelSHARED CHeader
  {
  public:
    uint16  uiSrcPort = 0;          //!< tcp source port
    uint16  uiDestPort = 0;         //!< destinaltion port
    uint32  uiSeqnum = 0;           //!< sequence number
    uint32  uiAcknum = 0;           //!< ack number
    uint16  uiHdrLenAndFlags = 0;   //!< header length and flags
    uint16  uiWindow = 0;           //!< max size
    uint16  uiChecksum = 0;         //!< checksum
    uint16  uiUrgentPtr = 0;        //!< is acked

    //! @return Get source port form packet already swapped
    uint16 getSourcePort()
    { return CcStatic::swapInt16(uiSrcPort); }
    //! @return Get destination port form packet already swapped
    uint16 getDestinationPort()
    { return CcStatic::swapInt16(uiDestPort); }
    //! @return Get sequence id form packet already swapped
    uint32 getSequence()
    { return CcStatic::swapInt32(uiSeqnum); }
    //! @return Get acknowledge id form packet already swapped
    uint32 getAcknowledge()
    { return CcStatic::swapInt32(uiAcknum); }
    //! @return Get header length form packet already shifted
    uint16 getHeaderLength()
    { return (uiHdrLenAndFlags & 0xf0 ) >> 2; }
    //! @return Get flags form packet already swapped and masked
    uint8  getFlags()
    { return (CcStatic::swapInt16(uiHdrLenAndFlags) & 0x3f); }
    //! @return Get checksum already swapped
    uint16 getChecksum()
    { return CcStatic::swapInt16(uiChecksum); }

    //! @param uiPort: Insert source port, it will be swapped
    void setSourcePort(uint16 uiPort)
    { uiSrcPort = CcStatic::swapInt16(uiPort); }
    //! @param uiNewWindow: Insert window size, it will be swapped
    void setWindow(uint16 uiNewWindow)
    { this->uiWindow = CcStatic::swapInt16(uiNewWindow); }
    //! @param uiPort: Insert destination port, it will be swapped
    void setDestinationPort(uint16 uiPort)
    { uiDestPort = CcStatic::swapInt16(uiPort); }
    //! @param uiNewLength: Insert length of header, it will be swapped and shifted
    void setHeaderLength(uint16 uiNewLength)
    { uiHdrLenAndFlags = (uiHdrLenAndFlags & 0xf0) | CcStatic::swapInt16((uiNewLength<<2) & 0xf000); }

    /**
     * @brief Generate Checksume of packet.
     * @param oDestIp:    Target ip to replace in packet
     * @param oSourceIp:  Source ip to replace in packet
     * @param uiLength:   Length of packet
     * @param pData:      Packet data
     */
    void generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp, uint16 uiLength, void* pData);
  public:
    static const uint16 FIN = 0x01; //!< Finish flag for TCP Packet
    static const uint16 SYN = 0x02; //!< Sync flag for TCP Packet
    static const uint16 RST = 0x04; //!< Reset flag for TCP Packet
    static const uint16 PSH = 0x08; //!< Push flag for TCP Packet
    static const uint16 ACK = 0x10; //!< Acknoledge flag for TCP Packet
  };
#pragma pack(pop)

  /**
   * @brief Current mode of socket
   */
  enum class EMode
  {
    Listening,
    Accepting,
    Client
  };
public:

  /**
   * @brief Create protocol for Networkstack
   * @param pParentProtocol: Parent protocol of stack
   */
  CcTcpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcTcpProtocol();

  /**
   * @brief Initialize Protocol
   * @return Status of operation.
   */
  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  /**
   * @brief Register a socket on this port to receive and transmit packets
   * @param pSocket: Target socket to register
   * @return Status of operation
   */
  CcStatus registerSocket(CcNetworkSocketTcp* pSocket);

  /**
   * @brief Remove socket from protocol
   * @param pSocket: Target socket to remove
   * @return True if it was found and removed
   */
  bool removeSocket(CcNetworkSocketTcp* pSocket);

  /**
   * @brief Setup current packet to sync and acknoledge.
   * @param pPacket:      Target packet to setup
   * @param uiSequence:   Target sequence number
   * @param uiAcknoledge: Target acknoledge number
   */
  void sendSynAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);

  /**
   * @brief Setup current packet to acknoledge.
   * @param pPacket:      Target packet to setup
   * @param uiSequence:   Target sequence number
   * @param uiAcknoledge: Target acknoledge number
   */
  void sendAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);

  /**
   * @brief Setup current packet to Finish.
   * @param pPacket:      Target packet to setup
   * @param uiSequence:   Target sequence number
   * @param uiAcknoledge: Target acknoledge number
   */
  void sendFin(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);

  /**
   * @brief Setup current packet to Finisch and Acknoledge.
   * @param pPacket:      Target packet to setup
   * @param uiSequence:   Target sequence number
   * @param uiAcknoledge: Target acknoledge number
   */
  void sendFinAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);

  /**
   * @brief Setup current packet to Push and Acknoledge.
   * @param pPacket:      Target packet to setup
   * @param uiSequence:   Target sequence number
   * @param uiAcknoledge: Target acknoledge number
   */
  void sendPshAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);
private: // Types
  class CPrivate;
private: // Methods
  void sendFlags(uint16 uiFlags, CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);
  CHeader* setupTcpHeader(CcNetworkPacket* pPacket);
  CcTcpProtocol(const CcTcpProtocol& oToCopy) = delete;
  CcTcpProtocol(CcTcpProtocol&& oToMove) = delete;
private: // Member
  CPrivate* m_pPrivate;
};
