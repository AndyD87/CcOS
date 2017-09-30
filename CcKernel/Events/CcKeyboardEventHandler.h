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
 * @page      Events
 * @subpage   CcKeyboardEventHandler
 *
 * @page      CcKeyboardEventHandler
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcKeyboardEventHandler
 **/
#ifndef CcKeyboardEventHandler_H_
#define CcKeyboardEventHandler_H_

#include "CcBase.h"

/**
 * @brief Class impelmentation
 */
class CcKeyboardEventHandler {
public:
  /**
   * @brief Constructor
   */
  CcKeyboardEventHandler( void );

  /**
   * @brief Destructor
   */
  virtual ~CcKeyboardEventHandler( void );
};

#endif /* CcKeyboardEventHandler_H_ */