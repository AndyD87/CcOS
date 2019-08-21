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
 * @page      STM32F207IG
 * @subpage   STM32F207IGNetwork
 * 
 * @page      STM32F207IGNetwork
 * @author    Andreas Dirmeier
 * @copyright Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F207IGNetwork
 */
#ifndef H_STM32F207IGNetwork_H_
#define H_STM32F207IGNetwork_H_

#include "CcBase.h"
#include "Devices/INetwork.h"

class STM32F207IGNetworkPrivate;

class STM32F207IGNetwork : public INetwork
{
public: //methods
  STM32F207IGNetwork();
  virtual ~STM32F207IGNetwork();

  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;

  virtual uint32 getChecksumCapabilities() override;

  virtual void readFrame();
  virtual bool writeFrame(CcNetworkPacketRef oFrame);
  virtual size_t sendFrames()
      { return m_uiSendFrames; }
  virtual size_t receivedFrames()
      { return m_uiReceivedFrames; }
public: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
  CcStatus m_oState = false;
  size_t m_uiReceivedFrames = 0;
  size_t m_uiSendFrames = 0;
};

#endif /* H_STM32F207IGNetwork_H_ */
