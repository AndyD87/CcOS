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
 * @subpage   CcTargetSystem
 * 
 * @page      CcTargetSystem
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcTargetSystem
 */

#ifndef _CcTargetSystem_H_
#define _CcTargetSystem_H_

#include "CcBase.h"
#include "CcSystem.h"
#include "CcTimerTarget.h"
#include "dev/CcTimer.h"
#include "dev/CcGPIO.h"
#include "dev/CcDisplay.h"
#include "dev/CcTouch.h"
#include "CcThread.h"

class CcTargetSystem: public CcSystem {
public:
  CcTargetSystem();
  virtual ~CcTargetSystem();

  void init(void);
  bool start( void );
  bool initGUI(void);
  bool initCLI(void);
  time_t getTime( void );
  bool createThread(CcThread *object);
private:
  void initSystem(void);
  void initTimer( void );
  void initGPIO( void );
  void initDisplay( void );
  void initTouch( void );

  CcGPIO*    m_GPIO;
  CcDisplay* m_Display;
  CcTouch*   m_Touch;
  CcTimerTarget*   m_Timer;
};

#endif /* _CcTargetSystem_H_ */
