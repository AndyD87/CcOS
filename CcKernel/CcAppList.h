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
 * @subpage   CcAppList
 *
 * @page      CcAppList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcAppList
 */
#ifndef CcAppList_H_
#define CcAppList_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcAppListItem.h"
#include "CcList.h"

#ifdef WIN32
template class CcKernelSHARED CcList<CcAppListItem>;
#endif

/**
 * @brief List of Applications and their instances.
 */
class CcKernelSHARED CcAppList : public CcList<CcAppListItem>
{
public:
  /**
   * @brief Constructor
   */
  CcAppList();

  /**
   * @brief Destructor
   */
  virtual ~CcAppList();

  /**
   * @brief Append a new Application to list by name
   * @param sName: Name of Application
   * @return Reference to this list
   */
  CcAppList& append(const CcAppHandle& hApplication);

  void stopAll();
};

#endif /* CcAppList_H_ */
