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
 * @subpage   CcLinuxTimer
 *
 * @page      CcLinuxTimer
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLinuxTimer
 */
#ifndef CcLinuxTimer_H_
#define CcLinuxTimer_H_

#include "CcBase.h"
#include "Devices/CcTimer.h"

class CcLinuxTimer : public CcTimer {
public: //methods
  CcLinuxTimer();
  virtual ~CcLinuxTimer();

  static void delayMs(uint32 uiDelay);
  void delayS(uint32 uiDelay);

  static void tick( void );

private: //methods
  static uint32 getCounterState(void);

private: //member
  static uint32 s_CountDown;
};

#endif /* CcLinuxTimer_H_ */
