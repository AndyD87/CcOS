/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @file      LinuxTouch
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class LinuxTouch
 */
#include "LinuxTouch.h"
#include "CcKernel.h"

LinuxTouch::LinuxTouch() {

}

LinuxTouch::~LinuxTouch() {
}

bool LinuxTouch::open(EOpenFlags flags){
  CCUNUSED(flags);
  return true;
}

void LinuxTouch::getTouchState(uint16 *x, uint16 *y){
  *x=getXAbsolute();
  *y=getYAbsolute();
}

bool LinuxTouch::getPressState(void){
  return true; 
}
