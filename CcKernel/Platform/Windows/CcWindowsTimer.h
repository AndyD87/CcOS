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
 * @page      Windows
 * @subpage   CcWindowsTimer
 *
 * @page      CcWindowsTimer
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindowsTimer
 **/
#ifndef CcWindowsTimer_H_
#define CcWindowsTimer_H_

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "Devices/CcTimer.h"

class CcWindowsTimer : public CcTimer {
public: //methods
  CcWindowsTimer();
  virtual ~CcWindowsTimer();


  static void delayMs(uint32 uiDelay);
  void delayS(uint32 uiDelay);

  static void tick(void);
  CcStatus open(EOpenFlags flags) override;
  CcStatus close(void) override;
  size_t read(void* buffer, size_t size) override;
  size_t write(const void* buffer, size_t size) override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  CcStatus cancel() override {return false;}

private: //methods
  static uint32 getCounterState(void);

private: //member
  static uint32 s_CountDown;
};

#endif /* CcWindowsTimer_H_ */
