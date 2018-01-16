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
 * @subpage   CcDhcpServerData
 *
 * @page      CcDhcpServerData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpServerData
 **/
#ifndef _CcDhcpServerData_H_
#define _CcDhcpServerData_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcDhcpLeaseList.h"

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpServerData
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpServerData(void);

  /**
   * @brief Destructor
   */
  ~CcDhcpServerData(void);

  const CcDhcpLeaseList& getIpV4LeaseList() const
    { return m_oPendingLeaseList; }
  const CcDhcpLeaseList& getIpV4PendingLeaseList() const
    { return m_oLeaseList; }
  
  CcDhcpLeaseList& getIpV4LeaseList()
    { return m_oPendingLeaseList; }
  CcDhcpLeaseList& getIpV4PendingLeaseList()
    { return m_oLeaseList; }

private:
  CcDhcpLeaseList m_oLeaseList;
  CcDhcpLeaseList m_oPendingLeaseList;
};
#endif /* _CcDhcpServerData_H_ */
