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
 * @page      STM32F407V
 * @subpage   CcTimerTarget
 * 
 * @page      CcTimerTarget
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcTimerTarget
 */
#ifndef _CCLIB_CcTimerTarget_H_
#define _CCLIB_CcTimerTarget_H_

#include "CcBase.h"
#include "Devices/CcTimer.h"

class CcTimerTarget : public CcTimer
{
public: //methods
  CcTimerTarget();
  virtual ~CcTimerTarget();
  void tick( void );
  time_t getTime(void);

private: //member
  time_t m_SystemTime;
  time_t m_CountDown;
};

#endif /* _CCLIB_CcTimerTarget_H_ */
