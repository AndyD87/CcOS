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
 * @page      Camera
 * @subpage   IDriverV4L
 *
 * @page      IDriverV4L
 * @author    Andreas Dirmeier
 * @copyright Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class IDriverV4L
 */
#ifndef _IDriverV4L_H_
#define _IDriverV4L_H_

#include "IDriverV4L.h"

/**
 * @brief Example Class impelmentation
 */
class IDriverV4L {
public:
  /**
   * @brief Constructor
   */
  IDriverV4L();

  /**
   * @brief Destructor
   */
  virtual ~IDriverV4L();
};

#endif /* _IDriverV4L_H_ */
