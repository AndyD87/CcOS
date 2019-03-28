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
#ifndef _CcUdpProtocol_H_
#define _CcUdpProtocol_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcEventHandleMap.h"
#include "Network/INetworkProtocol.h"
#include "CcIp.h"

class CcKernelSHARED CcUdpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for TCP Header
   */
  typedef struct
  {
    uint16 uiSrcPort;  //!< udp source port
    uint16 uiDestPort; //!< udp destinaltion port
    uint16 uiLength;   //!< udp package length, including tcp header
    uint16 uiChecksum; //!< udp checksum
  } SHeader;
#pragma pack(pop)
public:
  CcUdpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcUdpProtocol();

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcBufferList& oBuffer) override;
  virtual bool receive(CcBufferList& oBuffer) override;
  bool registerOnReceive(const CcIp& oIp, uint16 uiPort, IEvent* pEvent);

  static uint16 getSourcePort(SHeader* pHeader)
    { return CcStatic::swapInt16(pHeader->uiSrcPort); }
  static uint16 getDestinationPort(SHeader* pHeader)
    { return CcStatic::swapInt16(pHeader->uiDestPort); }
  static uint16 getLength(SHeader* pHeader)
    { return CcStatic::swapInt16(pHeader->uiLength); }
  static uint16 getChecksumh(SHeader* pHeader)
    { return CcStatic::swapInt16(pHeader->uiChecksum); }

  static uint16 generateChecksum(SHeader* pHeader, const CcIp& oDestIp, const CcIp& oSourceIp);

private: // Types
  class CcUdpProtocolPrivate;
private: // Methods
  CcUdpProtocol(const CcUdpProtocol& oToCopy) = delete;
  CcUdpProtocol(CcUdpProtocol&& oToMove) = delete;
private: // Member
  CcUdpProtocolPrivate* m_pPrivate;
  static const uint16 c_uiProtocolType;
};

#endif //_CcUdpProtocol_H_
