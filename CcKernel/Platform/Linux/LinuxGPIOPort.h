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
 * @page      Linux
 * @subpage   LinuxGPIOPort
 *
 * @page      LinuxGPIOPort
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class LinuxGPIOPort
 */

#ifndef LinuxGPIOPort_H_
#define LinuxGPIOPort_H_

#include "Devices/CcGPIOPort.h"
#include "LinuxGPIOPin.h"
#include "CcVector.h"

typedef struct
{
  uint8 uiNr;
  CcGPIOPin *cPin;
} SGPIOPinItem;

class LinuxGPIOPort : public CcGPIOPort {
public:
  LinuxGPIOPort();
  virtual ~LinuxGPIOPort();

  void init( void );

  inline uint8 count(){return 0xff;}
  CcGPIOPin* getPin(uint8 uiNr);

private:
  CcVector<SGPIOPinItem> m_lcPins;
};

#endif /* LinuxGPIOPort_H_ */
