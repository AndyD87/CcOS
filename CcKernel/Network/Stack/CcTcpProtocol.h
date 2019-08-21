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
 * @subpage   CcTcpProtocol
 *
 * @page      CcTcpProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTcpProtocol
 */
#ifndef H_CcTcpProtocol_H_
#define H_CcTcpProtocol_H_

#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "CcIp.h"

class CcNetworkSocketTcp;
class CcNetworkPacket;

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
    uint16  uiSrcPort = 0;          //! tcp source port
    uint16  uiDestPort = 0;         //! destinaltion port
    uint32  uiSeqnum = 0;           //! sequence number
    uint32  uiAcknum = 0;           //! ack number
    uint16  uiHdrLenAndFlags = 0;   //! header length and flags
    uint16  uiWindow = 0;           //! max size
    uint16  uiChecksum = 0;         //! checksum
    uint16  uiUrgentPtr = 0;        //! is acked

    uint16 getSourcePort()
      { return CcStatic::swapInt16(uiSrcPort); }
    uint16 getDestinationPort()
      { return CcStatic::swapInt16(uiDestPort); }
    uint32 getSequence()
      { return CcStatic::swapInt32(uiSeqnum); }
    uint32 getAcknowledge()
      { return CcStatic::swapInt32(uiAcknum); }
    uint16 getHeaderLength()
      { return (uiHdrLenAndFlags & 0xf0 ) >> 2; }
    uint8  getFlags()
      { return (CcStatic::swapInt16(uiHdrLenAndFlags) & 0x3f); }
    uint16 getChecksum()
      { return CcStatic::swapInt16(uiChecksum); }

    void setSourcePort(uint16 uiPort)
      { uiSrcPort = CcStatic::swapInt16(uiPort); }
    void setWindow(uint16 uiNewWindow)
      { this->uiWindow = CcStatic::swapInt16(uiNewWindow); }
    void setDestinationPort(uint16 uiPort)
      { uiDestPort = CcStatic::swapInt16(uiPort); }
    void setHeaderLength(uint16 uiNewLength)
      { uiHdrLenAndFlags = (uiHdrLenAndFlags & 0xf0) | CcStatic::swapInt16((uiNewLength<<2) & 0xf000); }
    void generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp, uint16 uiLength, void* pData);
  public:
    static const uint16 FIN = 0x01;
    static const uint16 SYN = 0x02;
    static const uint16 RST = 0x04;
    static const uint16 PSH = 0x08;
    static const uint16 ACK = 0x10;
  };
#pragma pack(pop)

  enum class EMode
  {
    Listening,
    Accepting,
    Client
  };
public:
  CcTcpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcTcpProtocol();

  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;
  CcStatus registerSocket(CcNetworkSocketTcp* pSocket);
  void removeSocket(CcNetworkSocketTcp* pSocket);

  void sendSynAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);
  void sendAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);
  void sendFin(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);
  void sendFinAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge);
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

#endif //H_CcTcpProtocol_H_
