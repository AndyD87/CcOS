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
 * @subpage   CcAppKnownList
 *
 * @page      CcAppKnownList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcAppKnownList
 */
#ifndef H_CcAppKnownList_H_
#define H_CcAppKnownList_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcUuid.h"
#include "CcMap.h"

/**
 * @brief Class contains a list known Application and thier entry points.
 *        For example, Kernel has an static instance of all known application in framework.
 */
class CcKernelSHARED CcAppKnownList : public CcMap<CcString, CcUuid>
{
public:
  /**
   * @brief Constructor
   */
  CcAppKnownList();

  /**
   * @brief Destructor
   */
  ~CcAppKnownList();
};

#endif // H_CcAppKnownList_H_
