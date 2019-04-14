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
 * @subpage   CcUdpEchoServer
 *
 * @page      CcUdpEchoServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUdpEchoServer
 */
#ifndef _CcUdpEchoServer_H_
#define _CcUdpEchoServer_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcApp.h"

class CcKernelSHARED CcUdpEchoServer : public CcApp
{
public:
  CcUdpEchoServer(uint16 uiPort):
    m_uiPort(uiPort)
    {}
  virtual ~CcUdpEchoServer()
    {}

  virtual void run() override;
private:
  uint16 m_uiPort;
};

#endif //_CcUdpEchoServer_H_