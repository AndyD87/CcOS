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
 * @brief     Class CData
 **/
#pragma once

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcDhcpLeaseList.h"

namespace NDhcp
{

namespace NServer
{

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CData
{
public:
  /**
   * @brief Constructor
   */
  CData();

  /**
   * @brief Destructor
   */
  ~CData();

  //! Get active lease list
  const CcDhcpLeaseList& getIpV4LeaseList() const
  { return m_oPendingLeaseList; }
  //! Get pending lease list of active requests
  const CcDhcpLeaseList& getIpV4PendingLeaseList() const
  { return m_oLeaseList; }

  //! Get active lease list
  CcDhcpLeaseList& getIpV4LeaseList()
  { return m_oPendingLeaseList; }
  //! Get pending lease list of active requests
  CcDhcpLeaseList& getIpV4PendingLeaseList()
  { return m_oLeaseList; }

private:
  CcDhcpLeaseList m_oLeaseList;
  CcDhcpLeaseList m_oPendingLeaseList;
};

}

}