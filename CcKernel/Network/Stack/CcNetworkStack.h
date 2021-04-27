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
 * @brief     Class CcNetworkStack
 */
#pragma once

#include "Network/INetworkStack.h"
#include "Network/CcMacAddress.h"
#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcObject.h"
#include "CcIp.h"
#include "CcDateTime.h"
#include "Network/ISocket.h"
#include "Devices/INetwork.h"

class CcIpInterface;

/**
 * @brief CcOS own network stack implementation for managing protocols from socket to
 *        network device.
 */
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

    //! @return Get destinatio physical address from packet
    CcMacAddress getDestination()
    { return CcMacAddress(puiEthernetPacketDest); }
    //! @return Get source physical address from packet
    CcMacAddress getSource()
    { return CcMacAddress(puiEthernetPacketSrc); }
    };
  #pragma pack(pop)

public:
  CcNetworkStack();
  virtual ~CcNetworkStack();

  virtual bool init() override;
  virtual void deinit() override
  {}
  virtual uint16 getProtocolType() const override;

  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

  virtual ISocket* getSocket(ESocketType eType) override;
  virtual const CcIpInterface* getInterfaceForIp(const CcIp& oIp) const override;

  /**
   * @brief Inject packet from network device.
   * @param pBuffer: pBuffer of packet to transfer
   */
  void onReceive(INetwork::CPacket* pBuffer);

  /**
   * @brief Inject device event from driver like connect or disconnect.
   * @param pDevice: Transmitting device
   */
  void onDeviceEvent(IDevice* pDevice);

  /**
   * @brief Add network device to be managed from this stack
   * @param pNetworkDevice: Target network device to add
   */
  void addNetworkDevice(INetwork* pNetworkDevice);

  /**
   * @brief Remove network device from network stack.
   * @param pNetworkDevice: Target network deivce to remove
   */
  void removeNetworkDevice(INetwork* pNetworkDevice);

  //! @return Get number of adapters registered on this network stack
  size_t getAdapterCount();

  /**
   * @brief Check if IP is matching Interface connecting.
   * @param pInterface: Interface to search in
   * @param oIp: IP to search for
   * @return True if interface
   */
  bool isInterfaceIpMatching(INetwork* pInterface, const CcIp& oIp);

  /**
   * @brief Insert IP and Physical address to arp table
   * @param oIp:        Ip to add
   * @param oMac:       Mac to add
   * @param bWasReply
   */
  void arpInsert(const CcIp& oIp, const CcMacAddress& oMac, bool bWasReply);

  /**
   * @brief Update Ip and mac
   * @param oIp:  Target IP to set
   * @param oMac: Target physical addres to set.
   */
  void arpUpdate(const CcIp& oIp, const CcMacAddress& oMac);

  /**
   * @brief Search in ARP List for an IP-Address
   * @param oIp:        IP to search for
   * @param bDoRequest: If true end request to network to discovery.
   * @return Pointer to MacAdress found in list or nullptr if not
   */
  const CcMacAddress* arpGetMacFromIp(const CcIp& oIp, bool bDoRequest) const;

  /**
   * @brief Search in ARP List for an physical Address
   * @param oMac:       IP to search for
   * @param bDoRequest: If true end request to network to discovery.
   * @return Pointer to IP-Adress found in list or nullptr if not
   */
  const CcIp* arpGetIpFromMac(const CcMacAddress& oMac, bool bDoRequest) const;

  /**
   * @brief There should be only one instance of network stack on system.
   *        This one in only instance can be nullptr if stack was not already started
   *        from system. For example it is disabled, or it was called from an global static
   *        Method.
   * @return
   */
  static CcNetworkStack* instance()
  { return s_pInstance; }
private:
  class CPrivate;
private:
  CcNetworkStack(const CcNetworkStack& oToCopy) = delete;
  CcNetworkStack(CcNetworkStack&& oToMove) = delete;

private:
  static CcNetworkStack* s_pInstance;
  CPrivate* m_pPrivate = nullptr;
};
