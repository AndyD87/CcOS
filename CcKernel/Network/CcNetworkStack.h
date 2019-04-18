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
 * @subpage   CcNetworkStack
 *
 * @page      CcNetworkStack
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcNetworkStack
 */
#ifndef _CcNetworkStack_H_
#define _CcNetworkStack_H_

#include "Network/CcMacAddress.h"
#include "Network/INetworkProtocol.h"
#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcObject.h"
#include "CcIp.h"
#include "CcDateTime.h"
#include "ISocket.h"

class INetwork;
class CcIpSettings;

class CcKernelSHARED CcNetworkStack : public CcObject, public INetworkProtocol
{
public: // Typedefs
#pragma pack(push, 1)
  /**
   * @brief typedef for ethernet header
   */
  class CEthernetHeader
  {
  public:
    uint8 puiEthernetPacketDest[6]; //!< mac destination
    uint8 puiEthernetPacketSrc[6];  //!< mac source
    uint16 uiProtocolType;          //!< protocol
    
  CcMacAddress getDestination()
    { return CcMacAddress(puiEthernetPacketDest); }
  CcMacAddress getSource()
    { return CcMacAddress(puiEthernetPacketSrc); }
  };
#pragma pack(pop)

public:
  CcNetworkStack();
  virtual ~CcNetworkStack();

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacket* pPacket) override;
  virtual bool receive(CcNetworkPacket* pPacket) override;
  void onReceive(CcNetworkPacket* pBuffer);
  void addNetworkDevice(INetwork* pNetworkDevice);
  size_t getAdapterCount();
  ISocket* getSocket(ESocketType eType);
  CcIpSettings* getInterfaceForIp(const CcIp& oIp);
  bool isInterfaceIpMatching(INetwork* pInterface, const CcIp& oIp);
  void arpInsert(const CcIp& oIp, const CcMacAddress& oMac, bool bWasReply);
  const CcMacAddress* arpGetMacFromIp(const CcIp& oIp, bool bDoRequest) const;
  const CcIp* arpGetIpFromMac(const CcMacAddress& oMac, bool bDoRequest) const;
private:
  class CPrivate;
private:
  CcNetworkStack(const CcNetworkStack& oToCopy) = delete;
  CcNetworkStack(CcNetworkStack&& oToMove) = delete;

private:
  CPrivate* m_pPrivate = nullptr;
};

#endif //_CcNetworkStack_H_
