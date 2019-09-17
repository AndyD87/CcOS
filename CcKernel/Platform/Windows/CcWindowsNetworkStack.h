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
 * @subpage   CcWindowsNetworkStack
 *
 * @page      CcWindowsNetworkStack
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsNetworkStack
 */
#ifndef H_CcWindowsNetworkStack_H_
#define H_CcWindowsNetworkStack_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Network/INetworkStack.h"
#include "Network/CcIpSettings.h"

class CcIpSettings;

class CcKernelSHARED CcWindowsNetworkStack : public INetworkStack
{
public:
  CcWindowsNetworkStack();
  virtual ~CcWindowsNetworkStack();

  virtual bool init() override;
  virtual ISocket* getSocket(ESocketType eType) override;
  virtual CcIpSettings* getInterfaceForIp(const CcIp& oIp) override;
  virtual CcVector<CcIpSettings> getIpSettingsForInterface(const INetwork* pInterface) override;
private: // Types
  class CPrivate;

private: // Member
  CPrivate* m_pPrivate = nullptr;
};

#endif //H_CcWindowsNetworkStack_H_
