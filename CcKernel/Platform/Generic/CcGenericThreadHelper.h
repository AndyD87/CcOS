/*
 *
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
 * @page      Linux
 * @subpage   CcGenericThreadHelper
 *
 * @pag       CcGenericThreadHelper
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcGenericThreadHelper
 */
#ifndef H_CcGenericThreadHelper_H_
#define H_CcGenericThreadHelper_H_

#include "CcBase.h"

/**
 * @brief Button for GUI Applications
 */
class CcGenericThreadHelper
{
public:
  /**
   * @brief Constructor
   */
  CcGenericThreadHelper();

  /**
   * @brief Destructor
   */
  ~CcGenericThreadHelper();

  void insert(void* pContext, void* pStackPointer, const char* pName);
  void current(void* pContext);
  void remove(void* pContext);
};

#endif /* H_CcGenericThreadHelper_H_ */
