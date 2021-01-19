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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class INetwork
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "IDevice.h"
#include "Network/Stack/CcNetworkPacket.h"
#include "CcEventHandler.h"
#include "Network/CcMacAddress.h"
#include "CcList.h"
#include "Network/CcIpInterface.h"

/**
 * @brief Network class for devices with network functionality
 */
class CcKernelSHARED INetwork : public IDevice
{
public: // Types
  /**
   * @brief Network event for registered object to receive status changes
   */
  class CNetworkEvent
  {
  public: // Types
    /**
     * @brief Available status codes
     */
    enum class EType : uint32
    {
      Alive = 0,  //!< This event is has no meaning!
                  ///  It can be send to trigger receiver and tell them we are alive
      Started,
      Stopped,
      Connected,
      Disconnected
    };
  public: // Methods
    /**
     * @brief Create a network event by type and sending device
     * @param eType:    Type of status to publish
     * @param pSender:  Device which status is assigned to.
     */
    CNetworkEvent(EType eType, INetwork* pSender) :
      eType(eType),
      pSender(pSender)
      {}
  public: // Member
    EType     eType;    //!< Status as enum
    INetwork* pSender;  //!< Source device
    union uData         //!< Additional data if required.
    {
      char pData[1];    //!< First byte of additional data
    };
  };

  /**
   * @brief Storage for Network packet for internal handling.
   */
  class CPacket
  {
  public:
    CcNetworkPacket*  pPacket;  //!< Packet to carry
  };

  /**
   * @brief Flag definitions for checksum capabilities for several protocols
   *        a device has supported.
   *        It saves resources if hardware is already enabled to calculate checksum.
   */
  class CChecksumCapabilities
  {
  public:
    const static uint32 uETH  = 0x01; //!< Ethernet protocol checksum suported
    const static uint32 uIP   = 0x02; //!< IP protocol checksum suported
    const static uint32 uICMP = 0x04; //!< ICMP protocol checksum suported
    const static uint32 uUDP  = 0x08; //!< UDP protocol checksum suported
    const static uint32 uTCP  = 0x10; //!< TCP protocol checksum suported
  };
public:
  //! @brief Create empty object.
  INetwork() = default;
  virtual ~INetwork() = default;

  /**
   * @brief Get physical address of device
   * @return Valid physical address or broadcast if not available
   */
  virtual const CcMacAddress& getMacAddress() = 0;

  /**
   * @brief Get media connection status
   * @return True if connection is established.
   */
  virtual bool isConnected() = 0;

  /**
   * @brief Get checksum capabilites from CChecksumCapabilities
   * @return ORed flags of supported hardware checksums.
   */
  virtual uint32 getChecksumCapabilities()
  { return 0; }

  /**
   * @brief Read frame from device, this will be called from Networkstack to
   *        trigger package read from device.
   */
  virtual void readFrame()
  {}

  /**
   * @brief Write frame to network device.
   * @param oFrame: Frame with network data for device to write.
   * @return True if packet is queued successfully
   */
  virtual bool writeFrame(CcNetworkPacketRef oFrame)
  { CCUNUSED(oFrame); return false; }

  //! @brief Number of frames send, if no implementation available return value is 0
  virtual size_t sendFrames()
  { return 0; }
  //! @brief Number of frames received, if no implementation available return value is 0
  virtual size_t receivedFrames()
  { return 0; }

  /**
   * @brief Register on packet receiver. Only one can be assigned.
   * @param oEvent: Event receiver objectect.
   */
  virtual void registerOnReceive(const CcEvent& oEvent)
  { m_oReceiver = oEvent; }

  /**
   * @brief Remove current registered receiver from device
   */
  virtual void removeOnReceive()
  { m_oReceiver.clear(); }

  /**
   * @brief Get assigned interfaces to this device
   * @return Stored interface list.
   */
  const CcIpInterfaceList& getInterfaceList() const
  { return m_oInterfaces; }

  /**
   * @brief Register event receiver if device status changed.
   * @param oEvent: Event receiver objectect.
   */
  void registerOnNetworkEvents(const CcEvent& oEvent)
  { m_oEventHandler.append(oEvent); }

protected:
  /**
   * @brief Internaly call registerd event handler
   * @param pEvent
   */
  inline void callNetworkEventHandler(CNetworkEvent* pEvent)
  { getNetworkEventHandler().call(pEvent);}

  /**
   * @brief Get stored eventhandler.
   * @return Reference to event handler.
   */
  inline CcEventHandler& getNetworkEventHandler()
  { return m_oEventHandler; }

protected:
  CcIpInterfaceList m_oInterfaces;  //!< System interfaces of device
  CcEvent           m_oReceiver;    //!< Event handler for received packets.
                                    //!< Depending on system, the handler could be called on interrupt,
                                    //!< the implementation must consider this.
  CcEventHandler    m_oEventHandler;//!< Registered handler to call on connection changes.
};
