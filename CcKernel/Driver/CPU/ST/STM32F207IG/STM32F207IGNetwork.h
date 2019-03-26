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
#ifndef _CCLIB_STM32F207IGNetwork_H_
#define _CCLIB_STM32F207IGNetwork_H_

#include "CcBase.h"
#include "Devices/INetwork.h"

class STM32F207IGNetworkPrivate;

class STM32F207IGNetwork : public INetwork
{
public: //methods
  STM32F207IGNetwork();
  virtual ~STM32F207IGNetwork();

  virtual bool isConnected() override;

  virtual CcBufferList readFrame();
  virtual void writeFrame(const CcBufferList& oFrame);
  virtual size_t sendFrames()
      { return m_uiSendFrames; }
  virtual size_t receivedFrames()
      { return m_uiReceivedFrames; }

private: //member
  STM32F207IGNetworkPrivate* m_pPrivate;
  CcStatus m_oState = false;
  size_t m_uiReceivedFrames = 0;
  size_t m_uiSendFrames = 0;
};

#endif /* _CCLIB_STM32F207IGNetwork_H_ */
