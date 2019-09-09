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
 * @page      CcDhcp
 * @subpage   CcDhcpServerWorker
 *
 * @page      CcDhcpServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpServerWorker
 **/
#ifndef H_CcDhcpServerWorker_H_
#define H_CcDhcpServerWorker_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "IWorker.h"
#include "Network/Protocols/Dhcp/CcDhcpPacket.h"

class CcDhcpServerConfig;
class CcDhcpServerData;
class CcDhcpPacket;

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpServerWorker : public IWorker
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpServerWorker(const CcDhcpServerConfig& oConfig, CcDhcpServerData &oData, CcDhcpPacket&& oPacket);

  /**
   * @brief Destructor
   */
  virtual ~CcDhcpServerWorker();

  virtual void run() override;

private: // Methods
  CcDhcpServerWorker(const CcDhcpServerWorker&) = delete;
  void operator=(const CcDhcpServerWorker&) = delete;

  void send();
  void processIpV4Discover(bool bIsRequest);

  void setupRequestOption(size_t uiPos);

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
  const CcDhcpServerConfig& m_oConfig;
  CcDhcpServerData& m_oData;
  CcDhcpPacket m_oPacket;
};
#endif // H_CcDhcpServerWorker_H_
