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
 * @subpage   WindowsTimer
 *
 * @page      WindowsTimer
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsTimer
 **/
#ifndef WindowsTimer_H_
#define WindowsTimer_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include "Devices/CcTimer.h"

class WindowsTimer : public CcTimer {
public: //methods
  WindowsTimer();
  virtual ~WindowsTimer();


  static void delayMs(uint32 uiDelay);
  void delayS(uint32 uiDelay);

  static void tick(void);
  bool open(EOpenFlags flags) override;
  bool close(void) override;
  size_t read(char* buffer, size_t size) override;
  size_t write(const char* buffer, size_t size) override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  bool cancel()override {return false;}

private: //methods
  static uint32 getCounterState(void);

private: //member
  static uint32 s_CountDown;
};

#endif /* WindowsTimer_H_ */
