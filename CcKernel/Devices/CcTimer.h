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
 * @page      Devices
 * @subpage   CcTimer
 *
 * @page      CcTimer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTimer
 */

#ifndef CCTIMER_H_
#define CCTIMER_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"

class CcKernelSHARED CcTimer : public CcIODevice{
public: //methods
  CcTimer();
  virtual ~CcTimer();

  static void delayMs(uint32 uiDelay);
  void delayS(uint32 uiDelay);

  static void tick( void );

  virtual bool open(EOpenFlags flags) override;
  virtual bool close(void) override;
  virtual size_t read(char* buffer, size_t size) override;
  virtual size_t write(const char* buffer, size_t size) override;

private: //methods
  static uint32 getCounterState(void);

private: //member
  static volatile uint32 s_CountDown;
};

#endif /* CCTIMER_H_ */
