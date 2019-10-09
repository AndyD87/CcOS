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
 * @page      Devices
 * @subpage   INetwork
 *
 * @page      INetwork
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class INetwork
 */

#ifndef H_INetwork_H_
#define H_INetwork_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IDevice.h"
#include "Network/Stack/CcNetworkPacket.h"
#include "CcEventHandler.h"
#include "Network/CcMacAddress.h"
#include "CcList.h"
#include "Network/CcIpInterface.h"

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED INetwork : public IDevice
{
public: // Types
  class CNetworkEvent
  {
  public: // Types
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
    CNetworkEvent(EType eType, INetwork* pSender) :
      eType(eType),
      pSender(pSender)
      {}
  public: // Member
    EType     eType;
    INetwork* pSender;
    union uData
    {
      char pData[1];
    };
  };
  class CPacket
  {
  public:
    CcNetworkPacket*  pPacket;
  };
  class CChecksumCapabilities
  {
  public:
    const static uint32 ETH  = 0x01;
    const static uint32 IP   = 0x02;
    const static uint32 ICMP = 0x04;
    const static uint32 UDP  = 0x08;
    const static uint32 TCP  = 0x10;
  };
public:
  /**
   * @brief Destructor
   */
  virtual ~INetwork() = default;

  virtual const CcMacAddress& getMacAddress() = 0;
  virtual bool isConnected() = 0;

  virtual uint32 getChecksumCapabilities()
    { return 0; }

  virtual void readFrame()
    {}
  virtual bool writeFrame(CcNetworkPacketRef oFrame)
    { CCUNUSED(oFrame); return false; }
  virtual size_t sendFrames()
    { return 0; }
  virtual size_t receivedFrames()
    { return 0; }

  virtual void registerOnReceive(IEvent* pEvent)
    { CCDELETE(m_pReceiver); m_pReceiver = pEvent; }
  virtual void removeOnReceive()
    { CCDELETE(m_pReceiver); }

  const CcIpInterfaceList& getInterfaceList() const
    { return m_oInterfaces; }
  void registerOnNetworkEvents(IEvent* pEvent)
    { m_oEventHandler.append(pEvent); }

protected:
  void callNetworkEventHandler(CNetworkEvent* pEvent)
    { m_oEventHandler.call(pEvent);}
  CcEventHandler& getNetworkEventHandler()
    { return m_oEventHandler; }
protected:
  CcIpInterfaceList m_oInterfaces;
private:
  IEvent* m_pReceiver = nullptr;
  CcEventHandler m_oEventHandler;
};

#endif // _INetwork_H_
