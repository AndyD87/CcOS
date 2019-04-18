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
 * @subpage   CcTcpEchoServer
 *
 * @page      CcTcpEchoServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTcpEchoServer
 */
#ifndef _CcTcpEchoServer_H_
#define _CcTcpEchoServer_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcApp.h"

class CcSocket;

class CcKernelSHARED CcTcpEchoServer : public CcApp
{
public:
  CcTcpEchoServer(uint16 uiPort):
    m_uiPort(uiPort)
    {}
  CcTcpEchoServer(uint16 uiPort, CcSocket* pSocket):
    m_uiPort(uiPort),
    m_pSocket(pSocket)
    {}
  virtual ~CcTcpEchoServer()
    {}

  virtual void run() override;
private:
  uint16 m_uiPort;
  CcSocket* m_pSocket = nullptr;
  bool m_bSocketCreated = false;
};

#endif //_CcTcpEchoServer_H_
