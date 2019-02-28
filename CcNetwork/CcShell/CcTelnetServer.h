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
 * @page      CcShell
 * @subpage   CcTelnetServer
 *
 * @page      CcTelnetServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTelnetServer
 */
#ifndef _CcTelnetServer_H_
#define _CcTelnetServer_H_

#include "CcBase.h"
#include "CcShell.h"
#include "Network/CcSocket.h"
#include "Network/CcCommonPorts.h"

/**
 * @brief Button for GUI Applications
 */
class CcShellSHARED CcTelnetServer : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcTelnetServer(uint16 Port = CcCommonPorts::TELNET);

  /**
   * @brief Destructor
   */
  virtual ~CcTelnetServer();

  void run();

private:
  uint16    m_Port;
  CcSocket  m_Socket;
};

#endif /* _CcTelnetServer_H_ */
