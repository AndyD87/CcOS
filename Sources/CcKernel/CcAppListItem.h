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
 * @brief     Class CcAppListItem
 */
#pragma once

#include "CcBase.h"
#include "CcApp.h"
#include "CcString.h"
#include "CcHandle.h"


/**
 * @brief Item for CcAppList with containing all relevant data for an application.
 *        Just the Handle is stored with this Storage class, further data may follow in the future.
 */
class CcKernelSHARED CcAppListItem
{
public:
  /**
   * @brief Create empty item
   */
  CcAppListItem() = default;

  /**
   * @brief Create item with inital application handle
   * @param hApplication: Handle of application to add.
   */
  CcAppListItem(const CcAppHandle& hApplication) :
    m_hApp(hApplication)
  {}

  ~CcAppListItem() = default;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcAppListItem &oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcAppListItem &oToCompare) const
  { return !operator==(oToCompare); }

  //! @return Handle to stored application in list item.
  inline const CcAppHandle& getApp()
  { return m_hApp; }
  //! @param hApp: Change stored application with this new handle
  inline void setApp(const CcAppHandle& hApp)
  { m_hApp = hApp; }

private:
  CcAppHandle m_hApp; //!< Handle of Appplication
};