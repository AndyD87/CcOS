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
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CInterfaceServer
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcByteArray.h"
#include "IThread.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/CcSocket.h"

class CcRemoteDeviceServer;

namespace NRemoteDevice
{
namespace Server
{

/**
 * @brief Worker implementation for broad cast messages
 */
class CcRemoteDeviceSHARED CInterfaceServer : public IThread
{
public:
  /**
   * @brief Constructor
   */
  CInterfaceServer(CcRemoteDeviceServer& pServer):
    m_oServer(pServer)
  {}

  /**
   * @brief Destructor
   */
  virtual ~CInterfaceServer();

  virtual void run() override;
  virtual void onStop() override;

private:
  CcRemoteDeviceServer& m_oServer;
  CcSocket              m_oSocket;
};

}
}
