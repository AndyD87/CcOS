/**
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
 * @subpage   LinuxTouch
 *
 * @page      LinuxTouch
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class LinuxTouch
 */
#ifndef LinuxTouch_H_
#define LinuxTouch_H_

#include "CcBase.h"
#include "Devices/CcTouch.h"
class LinuxTouch : public CcTouch
{
public:
  LinuxTouch();
  virtual ~LinuxTouch();

  bool open(EOpenFlags flags = EOpenFlags::NoFlag);
  bool close(void){return true;}
  void getTouchState(uint16 *x, uint16 *y);
  bool getPressState(void);
};

#endif /* LinuxTouch_H_ */
