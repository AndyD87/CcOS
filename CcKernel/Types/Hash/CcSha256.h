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
 * @page      Hash
 * @subpage   CcSha256
 *
 * @page      CcSha256
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-02
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSha256
 */
#ifndef CCSHA256_H_
#define CCSHA256_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcSha256
{
public:
  /**
   * @brief Constructor
   */
  CcSha256( void );

  /**
   * @brief Destructor
   */
  virtual ~CcSha256( void );
};

#endif /* CCSHA256_H_ */
