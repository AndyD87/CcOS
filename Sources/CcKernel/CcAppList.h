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
 * @brief     Class CcAppList
 */
#pragma once

#include "CcBase.h"
#include "CcAppListItem.h"
#include "CcList.h"

template class CcKernelSHARED CcList<CcAppListItem>;

/**
 * @brief List of Applications and their instances.
 */
class CcKernelSHARED CcAppList : public CcList<CcAppListItem>
{
public:
  /**
   * @brief Destructor
   */
  ~CcAppList() = default;

  /**
   * @brief Append a new Application to list by handl
   * @param hApplication: Handle of Application
   * @return Reference to this list.
   */
  CcAppList& append(const CcAppHandle& hApplication);

  /**
   * @brief Send stop to all stored appliactions in list.
   */
  void stopAll();
};