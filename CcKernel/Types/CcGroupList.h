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
 * @brief     Class CcGroupList
 */
#pragma once

#include "CcBase.h"
#include "CcGroup.h"
#include "CcList.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcGroup>;
#endif

/**
 * @brief List of groups
 */
class CcKernelSHARED CcGroupList : public CcList<CcGroup>
{
public:
  CcGroupList() = default;
  ~CcGroupList() = default;

  /**
   * @brief Copy Constructor
   */
  CcGroupList(const CcGroupList& oToCopy);

  /**
   * @brief Find group by name and Get it's entry
   * @param Groupname: Group name to search for
   * @return Handle to found group.
   */
  CcGroup& findGroup(const CcString& Groupname);
  //! @copydoc findGroup()
  const CcGroup& findGroup(const CcString& Groupname) const;
};
