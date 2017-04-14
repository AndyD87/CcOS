/**
 * @author     Andreas Dirmeier
 * @copyright  Andreas Dirmeier (c) 2015
 * @version    0.01
 * @date       2015-08
 * @par        Language   C++ ANSI V3
 */
/**
 * @file     CcTargetSystem.h
 * @brief    Class CcTargetSystem
 **/

#ifndef CcTargetSystem_H_
#define CcTargetSystem_H_

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

#endif /* CcTargetSystem_H_ */
