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
 * @subpage   CcDhcpLeaseItem
 *
 * @page      CcDhcpLeaseItem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpLeaseItem
 **/
#ifndef H_CcDhcpLeaseItem_H_
#define H_CcDhcpLeaseItem_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "Network/CcMacAddress.h"
#include "CcIp.h"
#include "CcDateTime.h"
#include "CcByteArray.h"

/**
 * @brief Control openssl library
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

  bool operator==(const CcDhcpLeaseItem& oToCompare) const;
  bool operator!=(const CcDhcpLeaseItem& oToCompare) const
    { return !operator==(oToCompare); }

  const CcIp& getIp() const
    { return m_oIpAddress; }
  const CcMacAddress& getMac() const
    { return m_oMacAddress; }
  const CcDateTime& getTimestamp() const
    { return m_oTimestamp; }
  const CcDateTime& getLeaseTime() const
    { return m_oLeaseTime; }
  const CcDateTime& getRenewTime() const
    { return m_oRenewTime; }
  const CcDateTime& getRebindTime() const
    { return m_oRebindTime; }
  const CcString& getName() const
    { return m_sName; }
  
  CcIp& ip()
    { return m_oIpAddress; }
  CcMacAddress& mac()
    { return m_oMacAddress; }
  CcDateTime& timestamp()
    { return m_oTimestamp; }
  CcDateTime& leaseTime()
    { return m_oLeaseTime; }
  CcDateTime& renewTime()
    { return m_oRenewTime; }
  CcDateTime& rebindTime()
    { return m_oRebindTime; }
  CcString& name()
    { return m_sName; }
  CcByteArray& clientId()
    { return m_oClientId; }
  uint32& transactionId()
    { return m_uiTransactionId; }

private:
  CcMacAddress m_oMacAddress;
  CcIp         m_oIpAddress;
  CcDateTime   m_oTimestamp;
  CcDateTime   m_oLeaseTime;
  CcDateTime   m_oRenewTime;
  CcDateTime   m_oRebindTime;
  CcString     m_sName;
  CcByteArray  m_oClientId;
  uint32       m_uiTransactionId = 0;
};
#endif // H_CcDhcpLeaseItem_H_
