/**
 * @author     Andreas Dirmeier
 * @copyright  Andreas Dirmeier (c) 2015
 * @version    0.01
 * @date       2015-08
 * @par        Language   C++ ANSI V3
 */
/**
 * @file     CcTouchTarget.h
 * @brief    Class CcTouchTarget
 **/
#ifndef CCTOUCHTARGET_H_
#define CCTOUCHTARGET_H_

#include "CcBase.h"
#include "STM32.h"
#include "dev/CcTouch.h"
#include "dev/CcTimer.h"

class CcTouchTarget : public CcTouch
{
public:
  CcTouchTarget();
  virtual ~CcTouchTarget();


  /**
   * @brief Open the Device, in this display nothing is to do
   * @param flags: not used
   * @return true if successfully opened.
   */
  bool open(OpenFlags::eOpenFlags flags);

  /**
   * @brief Close the Device
   * @return true if successfully done
   * @todo needs to be implemented
   */
  bool close(){ return true;}
  void initIO(void);
  void getTouchState(uint16 *x, uint16 *y);
  bool getPressState( void );

private:
  SPI_HandleTypeDef m_SpiHandle;
};

#endif /* CCTOUCHTARGET_H_ */
