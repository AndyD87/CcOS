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
#ifndef _CcNetworkStack_H_
#define _CcNetworkStack_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "Network/INetworkProtocol.h"
#include "CcObject.h"

class CcKernelSHARED CcNetworkStack : public CcObject, public INetworkProtocol
{
public:
  CcNetworkStack();
  virtual ~CcNetworkStack();

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcBufferList& oBuffer) override;
  virtual bool receive(CcBufferList& oBuffer) override;
  void onReceive(CcBufferList* pBuffer)
    { receive(*pBuffer); }

private:
  CcNetworkStack(const CcNetworkStack& oToCopy) = delete;
  CcNetworkStack(CcNetworkStack&& oToMove) = delete;
};

#endif //_CcNetworkStack_H_
