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
 * @subpage   INetworkStack
 *
 * @page      INetworkStack
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class INetworkStack
 */
#ifndef H_INetworkStack_H_
#define H_INetworkStack_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcObject.h"
#include "Network/ISocket.h"
#include "CcVector.h"

class CcIp;
class CcIpInterface;
class INetwork;

class CcKernelSHARED INetworkStack : public CcObject
{
public:
  virtual ~INetworkStack() = default;
  virtual bool init() = 0;
  virtual ISocket* getSocket(ESocketType eType) = 0;
  virtual CcIpInterface* getInterfaceForIp(const CcIp& oIp) = 0;
  virtual CcVector<CcIpInterface> getIpSettingsForInterface(const INetwork* pInterface) = 0;
};

#endif //H_INetworkStack_H_
