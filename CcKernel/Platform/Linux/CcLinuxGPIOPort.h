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
 * @subpage   CcLinuxGPIOPort
 *
 * @page      CcLinuxGPIOPort
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxGPIOPort
 */

#ifndef _CcLinuxGPIOPort_H_
#define _CcLinuxGPIOPort_H_

#include "Devices/CcGPIOPort.h"
#include "CcLinuxGPIOPin.h"
#include "CcVector.h"

typedef struct
{
  uint8 uiNr;
  CcGPIOPin *cPin;
} SGPIOPinItem;

class CcLinuxGPIOPort : public CcGPIOPort {
public:
  CcLinuxGPIOPort();
  virtual ~CcLinuxGPIOPort();

  void init(void );

  inline uint8 count(){return 0xff;}
  CcGPIOPin* getPin(uint8 uiNr);

private:
  CcVector<SGPIOPinItem> m_lcPins;
};

#endif /* _CcLinuxGPIOPort_H_ */
