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
 * @subpage   INetworkProtocol
 *
 * @page      INetworkProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class INetworkProtocol
 */
#ifndef _INetworkProtocol_H_
#define _INetworkProtocol_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcVector.h"
#include "CcBufferList.h"
#include "CcEventHandler.h"

class CcKernelSHARED INetworkProtocol : protected CcVector<INetworkProtocol*>
{
public:
  INetworkProtocol(INetworkProtocol* pParentProtocol) :
    m_pParentProtocol(pParentProtocol)
    {}
  virtual ~INetworkProtocol()
    {}
  virtual uint16 getProtocolType() const = 0;
  virtual bool transmit(const CcBufferList& oBuffer) = 0;
  virtual bool receive(const CcBufferList& oBuffer) = 0;

  virtual void registerOnReceive(IEvent* pEvent)
    { m_oReceiveHandler.prepend(pEvent); }

  INetworkProtocol* findProtocol(uint16 uiProtocolType);
  
private: // Methods
  INetworkProtocol(const INetworkProtocol& oToCopy) = delete;
  INetworkProtocol(INetworkProtocol&& oToMove) = delete;
  
private: // Member
  INetworkProtocol* m_pParentProtocol;
  CcEventHandler m_oReceiveHandler;
};

#endif //_INetworkProtocol_H_
