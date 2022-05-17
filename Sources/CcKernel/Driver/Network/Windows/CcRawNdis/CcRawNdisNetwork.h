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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class CcRawNdisNetwork
 */
#pragma once

#include "CcBase.h"
#include "Devices/INetwork.h"

/**
 * @brief This Device interface creates an connection to an installed
 *        NDIS Protocol filter driver to inject or extract raw packets.
 */
class CcRawNdisNetwork : public INetwork
{
public: //methods
  /**
   * @brief Create a new index on device to generate different devices
   * @param uiIndex: Target index of this device.
   */
  CcRawNdisNetwork(unsigned long uiIndex);

  /**
   * @brief Create a new device by name.
   * @param sName: Name to set for device
   */
  CcRawNdisNetwork(const CcString& sName);
  virtual ~CcRawNdisNetwork();

  //! @return Get phyiscal address of this simulated device
  virtual const CcMacAddress& getMacAddress() override;
  //! @return True if device connection is established
  virtual bool isConnected() override;
  virtual uint32 getChecksumCapabilities() override;

  virtual void readFrame() override;
  virtual bool writeFrame(const CcNetworkPacketRef oFrame) override;
  virtual size_t sendFrames() override
  { return m_uiSendFrames; }
  virtual size_t receivedFrames()override
  { return m_uiReceivedFrames; }

  /**
   * @brief Start device and capture trafic
   * @return True if device is tarden on simulated device
   */
  bool start();

  //! @return Number of simulated devices
  unsigned long getAdapterCount();

  //! @return True if ndis filter driver was found and is connectable
  bool isNdisAvailable();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
  CcStatus m_oState = false;
  size_t m_uiReceivedFrames = 0;
  size_t m_uiSendFrames = 0;
};
