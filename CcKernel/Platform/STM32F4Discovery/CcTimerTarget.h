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
 * @page      STM32F4Discovery
 * @subpage   CcTimerTarget
 * 
 * @page      CcTimerTarget
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTimerTarget
 */
#ifndef CCLIB_CcTimerTarget_H_
#define CCLIB_CcTimerTarget_H_

#include "CcBase.h"
#include "dev/CcTimer.h"

class CcTimerTarget : public CcTimer {
public: //methods
  CcTimerTarget();
  virtual ~CcTimerTarget();
  void tick( void );
  time_t getTime(void);

private: //member
  time_t m_SystemTime;
  time_t m_CountDown;
};

#endif /* CCLIB_CcTimerTarget_H_ */
