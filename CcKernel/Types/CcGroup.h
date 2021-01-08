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
 * @page      CcKernel
 * @subpage   CcGroup
 *
 * @page      CcGroup
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGroup
 */
#ifndef H_CCGROUP_H_
#define H_CCGROUP_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcHandle.h"

class CcGroup;

#ifdef _MSC_VER
template class CcKernelSHARED CcHandle<CcGroup>;
#endif

typedef class CcHandle<CcGroup> CcGroupHandle;

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcGroup 
{
public:
  /**
   * @brief Default-Constructor
   */
  CcGroup() = default;

  /**
   * @brief Constructor
   */
  CcGroup( const CcString& sName, uint32 uiId );

  /**
   * @brief Destructor
   */
  ~CcGroup() = default;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcGroup& oToCompare) const
    { return (m_uiGroupId == oToCompare.m_uiGroupId && m_sGroupName == oToCompare.m_sGroupName);}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcGroup& oToCompare) const
    { return !operator==(oToCompare); }

  const CcString& getName() const
    { return m_sGroupName; }
  uint32 getId() const
    { return m_uiGroupId; }

private:
  CcString m_sGroupName;
  uint32 m_uiGroupId = 0;
};

#endif // H_CcGROUP_H_
