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
 * @brief     Class CWorker
 **/
#pragma once

#include "CcBase.h"
#include "CcDhcp.h"
#include "IWorker.h"
#include "Network/Protocols/Dhcp/CcDhcpPacket.h"

class CcDhcpPacket;
class CcDhcpServer;

namespace NDhcp
{

namespace NServer
{

class CConfig;
class CData;

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CWorker : public IWorker
{
public:
  /**
   * @brief Constructor
   */
  CWorker(CcDhcpServer& oServer, CData &oData);

  /**
   * @brief Destructor
   */
  virtual ~CWorker();

  virtual void run() override;

  CcDhcpPacket* getPacket()
  { return &m_oPacket; }

private: // Methods
  CWorker(const CWorker&) = delete;
  void operator=(const CWorker&) = delete;

  void send();
  void processIpV4Discover(bool bIsRequest);

  void setupRequestOption(size_t uiPos);

private: // Types
  class CPrivate;
private: // Member
  CPrivate*           m_pPrivate = nullptr;
  CcDhcpServer&       m_oServer;
  CData&              m_oData;
  CcDhcpPacket        m_oPacket;
};

}

}
