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
#ifndef H_CcNetworkStack_H_
#define H_CcNetworkStack_H_

#include "Network/INetworkStack.h"
#include "Network/CcMacAddress.h"
#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcObject.h"
#include "CcIp.h"
#include "CcDateTime.h"
#include "Network/ISocket.h"
#include "Devices/INetwork.h"

class CcIpInterface;

class CcKernelSHARED CcNetworkStack : public INetworkStack, public INetworkProtocol
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

  virtual bool init() override;
  virtual uint16 getProtocolType() const override;

  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  virtual ISocket* getSocket(ESocketType eType) override;
  virtual CcIpInterface* getInterfaceForIp(const CcIp& oIp) override;
  virtual CcVector<CcIpInterface> getIpSettingsForInterface(const INetwork* pInterface) override;

  void onReceive(INetwork::CPacket* pBuffer);
  void onDeviceEvent(IDevice* pDevice);
  void addNetworkDevice(INetwork* pNetworkDevice);
  void removeNetworkDevice(INetwork* pNetworkDevice);
  size_t getAdapterCount();
  bool isInterfaceIpMatching(INetwork* pInterface, const CcIp& oIp);
  void arpInsert(const CcIp& oIp, const CcMacAddress& oMac, bool bWasReply);
  void arpUpdate(const CcIp& oIp, const CcMacAddress& oMac);
  const CcMacAddress* arpGetMacFromIp(const CcIp& oIp, bool bDoRequest) const;
  const CcIp* arpGetIpFromMac(const CcMacAddress& oMac, bool bDoRequest) const;
  static CcNetworkStack* instance()
    {return s_pInstance;}
private:
  class CPrivate;
private:
  CcNetworkStack(const CcNetworkStack& oToCopy) = delete;
  CcNetworkStack(CcNetworkStack&& oToMove) = delete;

private:
  static CcNetworkStack* s_pInstance;
  CPrivate* m_pPrivate = nullptr;
};

#endif //H_CcNetworkStack_H_
