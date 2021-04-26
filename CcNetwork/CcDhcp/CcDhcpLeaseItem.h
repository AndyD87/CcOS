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
 * @brief     Class CcDhcpLeaseItem
 **/
#pragma once

#include "CcBase.h"
#include "CcDhcp.h"
#include "Network/CcMacAddress.h"
#include "CcIp.h"
#include "CcDateTime.h"
#include "CcByteArray.h"

/**
 * @brief Leastime values for Dhcp Leasetime Table
 */
class CcDhcpSHARED CcDhcpLeaseItem
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpLeaseItem();

  /**
   * @brief Destructor
   */
  ~CcDhcpLeaseItem();

  /**
   * @brief Compare if containing time values are same
   * @param oToCompare: Item to compare with
   * @return True if items are same
   */
  bool operator==(const CcDhcpLeaseItem& oToCompare) const;

  /**
   * @brief Compare if containing time values are not same
   * @param oToCompare: Item to compare with
   * @return True if items are not same
   */
  bool operator!=(const CcDhcpLeaseItem& oToCompare) const
    { return !operator==(oToCompare); }

  //! @return Get ip as index value of lease times
  const CcIp& getIp() const
  { return m_oIpAddress; }
  //! @return Get mac as second index value of lease times
  const CcMacAddress& getMac() const
  { return m_oMacAddress; }
  //! @return Get created timestamp
  const CcDateTime& getTimestamp() const
  { return m_oTimestamp; }
  //! @return Get lease timeout timestamp
  const CcDateTime& getLeaseTime() const
  { return m_oLeaseTime; }
  //! @return Get renew timestamp
  const CcDateTime& getRenewTime() const
  { return m_oRenewTime; }
  //! @return Get rebind timestamp
  const CcDateTime& getRebindTime() const
  { return m_oRebindTime; }
  //! @return Get name of client
  const CcString& getName() const
  { return m_sName; }

  //! @return Get editable ip as index value of lease times
  CcIp& ip()
  { return m_oIpAddress; }
  //! @return Get editable mac as second index value of lease times
  CcMacAddress& mac()
  { return m_oMacAddress; }
  //! @return Get editable created timestamp
  CcDateTime& timestamp()
  { return m_oTimestamp; }
  //! @return Get editable lease timeout timestamp
  CcDateTime& leaseTime()
  { return m_oLeaseTime; }
  //! @return Get editable renew timestamp
  CcDateTime& renewTime()
  { return m_oRenewTime; }
  //! @return Get editable rebind timestamp
  CcDateTime& rebindTime()
  { return m_oRebindTime; }
  //! @return Get editable name of client
  CcString& name()
  { return m_sName; }
  //! @return Get editable client id
  CcByteArray& clientId()
  { return m_oClientId; }

private:
  CcMacAddress m_oMacAddress;
  CcIp         m_oIpAddress;
  CcDateTime   m_oTimestamp;
  CcDateTime   m_oLeaseTime;
  CcDateTime   m_oRenewTime;
  CcDateTime   m_oRebindTime;
  CcString     m_sName;
  CcByteArray  m_oClientId;
};
