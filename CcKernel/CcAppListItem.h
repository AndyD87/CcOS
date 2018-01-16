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
 * @subpage   CcAppListItem
 *
 * @page      CcAppListItem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcAppListItem
 */
#ifndef _CcAppListItem_H_
#define _CcAppListItem_H_

#include "CcBase.h"
#include "CcApp.h"
#include "CcString.h"
#include "CcKernelBase.h"
#include "CcHandle.h"


/**
 * @brief Item for CcAppList with containing all relevant data for an application.
 *        Just the Handle is stored with this Storage class, further data may follow in the future.
 */
class CcKernelSHARED CcAppListItem
{
public:
  /**
   * @brief Create Item with specific name
   * @param sName: Name to set for item, it can not be changed later
   */
  CcAppListItem() :
    m_hApp(nullptr)
    {}

  /**
   * @brief Create Item with specific name
   * @param sName: Name to set for item, it can not be changed later
   */
  CcAppListItem(const CcAppHandle& hApplication) :
    m_hApp(hApplication)
    {}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcAppListItem &oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcAppListItem &oToCompare) const
    { return !operator==(oToCompare); }

  inline const CcAppHandle& getApp()
    { return m_hApp; }
  
  inline void setName(const CcAppHandle& hApp)
    { m_hApp = hApp; }

private:
  CcAppHandle m_hApp; //!< Handle of Appplication
};

#endif /* _CcAppListItem_H_ */
