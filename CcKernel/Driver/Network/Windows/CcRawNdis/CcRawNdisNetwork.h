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
 * @page      STM32F407V
 * @subpage   CcRawNdisNetwork
 * 
 * @page      CcRawNdisNetwork
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcRawNdisNetwork
 */
#ifndef _CcRawNdisNetwork_H_
#define _CcRawNdisNetwork_H_

#include "CcBase.h"
#include "Devices/INetwork.h"

class CcRawNdisNetwork : public INetwork
{
public: //methods
  CcRawNdisNetwork(unsigned long uiIndex);
  CcRawNdisNetwork(const CcString& sName);
  virtual ~CcRawNdisNetwork();

  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;

  virtual uint32 getChecksumCapabilities() override;

  virtual void readFrame();
  virtual bool writeFrame(const CcNetworkPacket& oFrame);
  virtual size_t sendFrames()
      { return m_uiSendFrames; }
  virtual size_t receivedFrames()
      { return m_uiReceivedFrames; }

  unsigned long getAdapterCount();
  bool isNdisAvailable();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
  CcStatus m_oState = false;
  size_t m_uiReceivedFrames = 0;
  size_t m_uiSendFrames = 0;
};

#endif /* _CcRawNdisNetwork_H_ */
