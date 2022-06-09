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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpServer
 **/
#pragma once

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcApp.h"

namespace NDhcp
{
namespace NServer
{
class CConfig;
}
}

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpServer : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpServer();

  /**
   * @brief Destructor
   */
  virtual ~CcDhcpServer();

  virtual void run() override;

  /**
   * @brief Load config file form path
   * @param sPath: Path to configuration file
   * @return True if parsing succeeded
   */
  bool loadConfigFile(const CcString& sPath);

  size_t write(void* pBuf, size_t uiBufSize);
  const NDhcp::NServer::CConfig& getConfig() const;

private: // Methods
  void initPrivate();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};