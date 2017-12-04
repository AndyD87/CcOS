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
 * @page      Camera
 * @subpage   RaspiCam
 *
 * @page      RaspiCam
 * @author    Andreas Dirmeier
 * @copyright Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcDriverV4L
 */
#ifndef CcDriverV4L_H_
#define CcDriverV4L_H_

#include "CcDriverV4L.h"

/**
 * @brief Example Class impelmentation
 */
class CcDriverV4L {
public:
  /**
   * @brief Constructor
   */
  CcDriverV4L( void );

  /**
   * @brief Destructor
   */
  virtual ~CcDriverV4L( void );
};

#endif /* CcDriverV4L_H_ */
