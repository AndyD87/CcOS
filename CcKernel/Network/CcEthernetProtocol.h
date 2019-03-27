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
 * @subpage   CcEthernetProtocol
 *
 * @page      CcEthernetProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEthernetProtocol
 */
#ifndef _CcEthernetProtocol_H_
#define _CcEthernetProtocol_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "Network/INetworkProtocol.h"

class CcKernelSHARED CcEthernetProtocol : public INetworkProtocol
{
public:
  CcEthernetProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcEthernetProtocol();

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(const CcBufferList& oBuffer) override;
  virtual bool receive(const CcBufferList& oBuffer) override;

private:
  CcEthernetProtocol(const CcEthernetProtocol& oToCopy) = delete;
  CcEthernetProtocol(CcEthernetProtocol&& oToMove) = delete;
};

#endif //_CcEthernetProtocol_H_
