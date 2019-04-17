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
#ifndef _CcTcpProtocol_H_
#define _CcTcpProtocol_H_

#include "Network/INetworkProtocol.h"
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
  class CHeader
  {
  public:
    uint16  uiSrcPort;          //! tcp source port
    uint16  uiDestPort;         //! destinaltion port
    uint32  uiSeqnum;           //! sequence number
    uint32  uiAcknum;           //! ack number
    uint16  uiHdrLenAndFlags;   //! header length and flags
    uint16  uiWindow;           //! max size
    uint16  uiChecksum;         //! checksum
    uint16  uiUrgentPtr;        //! is acked

    uint16 getSourcePort()
      { return CcStatic::swapInt16(uiSrcPort); }
    uint16 getDestinationPort()
      { return CcStatic::swapInt16(uiDestPort); }
    uint32 getSequence()
      { return CcStatic::swapInt32(uiSeqnum); }
    uint32 getAcknowledge()
      { return CcStatic::swapInt32(uiAcknum); }
    uint16 getHeaderLength()
      { return (CcStatic::swapInt16(uiHdrLenAndFlags & 0xf000) ) << 2; }
    uint8  getFlags()
      { return (CcStatic::swapInt16(uiHdrLenAndFlags) & 0x3f); }
    uint16 getChecksum()
      { return CcStatic::swapInt16(uiChecksum); }

    void setSourcePort(uint16 uiPort)
      { uiSrcPort = CcStatic::swapInt16(uiPort); }
    void setDestinationPort(uint16 uiPort)
      { uiDestPort = CcStatic::swapInt16(uiPort); }
    void setHeaderLength(uint16 uiNewLength)
      { uiHdrLenAndFlags = (uiHdrLenAndFlags & 0xf0) | CcStatic::swapInt16((uiNewLength<<2) & 0xf000); }
    void generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp);
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

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacket* pPacket) override;
  virtual bool receive(CcNetworkPacket* pPacket) override;
  CcStatus registerSocket(CcNetworkSocketTcp* pSocket);
  void removeSocket(CcNetworkSocketTcp* pSocket);

  void sendSynAck(CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge);
  void sendAck(CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge);
  void sendPshAck(CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge);
private: // Types
  class CPrivate;
private: // Methods
  CHeader* setupTcpHeader(CcNetworkPacket* pPacket);
  CcTcpProtocol(const CcTcpProtocol& oToCopy) = delete;
  CcTcpProtocol(CcTcpProtocol&& oToMove) = delete;
private: // Member
  CPrivate* m_pPrivate;
};

#endif //_CcTcpProtocol_H_
