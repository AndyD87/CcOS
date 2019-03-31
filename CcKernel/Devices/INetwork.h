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

#ifndef _INetwork_H_
#define _INetwork_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IDevice.h"
#include "Network/CcNetworkPacket.h"
#include "CcEventHandler.h"
#include "Network/CcMacAddress.h"

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED INetwork : public IDevice
{
public:
  /**
   * @brief Destructor
   */
  virtual ~INetwork() = default;

  virtual const CcMacAddress& getMacAddress() = 0;
  virtual bool isConnected() = 0;

  virtual void readFrame()
    {}
  virtual bool writeFrame(const CcNetworkPacket& oFrame)
    { CCUNUSED(oFrame); return false; }
  virtual size_t sendFrames()
    { return 0; }
  virtual size_t receivedFrames()
    { return 0; }

  virtual void registerOnReceive(IEvent* pEvent)
    { CCDELETE(m_pReceiver); m_pReceiver = pEvent; }

protected:
  IEvent* m_pReceiver = nullptr;
};

#endif /* _INetwork_H_ */
