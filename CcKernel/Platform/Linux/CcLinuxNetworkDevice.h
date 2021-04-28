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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxNetworkDevice
 */
#pragma once

#include "CcBase.h"
#include "Network/INetworkStack.h"
#include "Devices/INetwork.h"

class CcIpInterface;

/**
 * @brief Network device for linux systems to get access to network adapter informations
 */
class CcKernelSHARED CcLinuxNetworkDevice : public INetwork
{
public:
  CcLinuxNetworkDevice(const CcString& sName, unsigned char pMacAddress[6]);
  virtual ~CcLinuxNetworkDevice() = default;

  virtual const CcMacAddress& getMacAddress() override
  { return m_oAddress; }
  virtual bool isConnected() override;
  virtual const CcString& getName() override
  { return m_sName; }

  void refreshInterfaces();
  static const char* getBasepath()
  { return c_sBasePath; }

private:
  CcString      m_sName;
  CcMacAddress  m_oAddress;

  static const char* c_sBasePath;
};
