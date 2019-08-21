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
 * @subpage   CcUdpProtocol
 *
 * @page      CcUdpProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUdpProtocol
 */
#ifndef H_CcUdpProtocol_H_
#define H_CcUdpProtocol_H_

#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "CcIp.h"

class CcNetworkSocketUdp;

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

    uint16 getSourcePort()
      { return CcStatic::swapInt16(uiSrcPort); }
    uint16 getDestinationPort()
      { return CcStatic::swapInt16(uiDestPort); }
    uint16 getLength()
      { return CcStatic::swapInt16(uiLength); }
    uint16 getChecksum()
      { return CcStatic::swapInt16(uiChecksum); }

    void setSourcePort(uint16 uiPort)
      { uiSrcPort = CcStatic::swapInt16(uiPort); }
    void setDestinationPort(uint16 uiPort)
      { uiDestPort = CcStatic::swapInt16(uiPort); }
    void setLength(uint16 uiNewLength)
      { uiLength = CcStatic::swapInt16(uiNewLength); }
    void generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp);
  };
#pragma pack(pop)
public:
  CcUdpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcUdpProtocol();

  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;
  CcStatus registerSocket(CcNetworkSocketUdp* pSocket);
  void removeSocket(CcNetworkSocketUdp* pSocket);

private: // Types
  class CPrivate;
private: // Methods
  CcUdpProtocol(const CcUdpProtocol& oToCopy) = delete;
  CcUdpProtocol(CcUdpProtocol&& oToMove) = delete;
private: // Member
  CPrivate* m_pPrivate;
};

#endif //H_CcUdpProtocol_H_
