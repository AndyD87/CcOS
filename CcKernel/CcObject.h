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
 * @subpage   CcObject
 *
 * @page      CcObject
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcObject
 */
#ifndef CCOBJECT_H_
#define CCOBJECT_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Basic Class for all Objects wich has receive callbacks
 */
class CcKernelSHARED  CcObject 
{
public:
  /**
   * @brief Constructor
   */
  CcObject()
    {}

  /**
   * @brief Destructor
   */
  virtual ~CcObject()
    {}

private:
};

#endif /* CCOBJECT_H_ */
