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
 * @subpage   CcDhcpLeaseList
 *
 * @page      CcDhcpLeaseList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpLeaseList
 **/
#ifndef CcDhcpLeaseList_H_
#define CcDhcpLeaseList_H_

#include "CcBase.h"
#include "CcList.h"
#include "CcDhcp.h"
#include "CcDhcpLeaseItem.h"

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpLeaseList : public CcList<CcDhcpLeaseItem>
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpLeaseList(void);

  /**
   * @brief Destructor
   */
  ~CcDhcpLeaseList(void);

  size_t findMacAddress(const CcMacAddress& oMacAddres) const;
  size_t findIpAddress(const CcIp& oIp) const;

  CcIp getNextFree(const CcIp& oBegin, const CcIp& oEnd);

private:
};
#endif /* CcDhcpLeaseList_H_ */
