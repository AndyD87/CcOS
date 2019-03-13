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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxTouch
 */
#include "CcLinuxTouch.h"
#include "CcKernel.h"

CcLinuxTouch::CcLinuxTouch() {

}

CcLinuxTouch::~CcLinuxTouch() {
}

CcStatus CcLinuxTouch::open(EOpenFlags flags){
  CCUNUSED(flags);
  return true;
}

void CcLinuxTouch::getTouchState(uint16 *x, uint16 *y){
  *x=getXAbsolute();
  *y=getYAbsolute();
}

bool CcLinuxTouch::getPressState(){
  return true; 
}
