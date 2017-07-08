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
 * @subpage   CcFileInfoList
 *
 * @page      CcFileInfoList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcFileInfoList
 */
#ifndef CcFileInfoList_H_
#define CcFileInfoList_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFileInfo.h"
#include "CcList.h"

#define SHOW_HIDDEN   0x01
#define SHOW_EXTENDED 0x02


/**
* @brief Handles all devices and Interfaces connected to Kernel
*/
class CcKernelSHARED CcFileInfoList : public CcList<CcFileInfo>
{
public:
  /**
   * @brief Constructor
   */
  CcFileInfoList();

  /**
   * @brief Destructor
   */
  virtual ~CcFileInfoList();

  bool find(const CcString& sName);

  CcStringList getFormatedList(uint8 uiShowFlags) const;
};

#endif /* CcFileInfoList_H_ */
