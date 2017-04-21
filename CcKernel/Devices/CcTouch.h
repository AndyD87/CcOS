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
 * @subpage   CcTouch
 *
 * @page      CcTouch
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTouch
 */

#ifndef CCTOUCH_H_
#define CCTOUCH_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "CcObject.h"
#include "CcDateTime.h"
#include <vector>

/**
 * @brief Matrix build through Calibration to get correct X/Y Values
 */
typedef struct{
  int32 A;
  int32 B;
  int32 C;
  int32 D;
  int32 E;
  int32 F;
  int32 Div;
} sCcTouchMatrix;

/**
 * @brief Callback values for CcTouch
 */
typedef enum {
  CCTOUCH_ONCLICK = 0,
}eCcTouchCBNr;

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED CcTouch : public CcIODevice {
public:
  /**
   * @brief Constructor
   */
  CcTouch();

  /**
   * @brief Destructor
   */
  virtual ~CcTouch();

  /**
   * @brief Init Touchpanel, must be implemented by DeviceClass
   */
  virtual bool open(EOpenFlags flags) = 0;
  virtual void getTouchState(uint16 *x, uint16 *y) = 0;
  virtual size_t read(char* buffer, size_t size) override
  {
    CCUNUSED(buffer);
    return size;
  }
  virtual size_t write(const char* buffer, size_t size) override
  {
    CCUNUSED(buffer);
    return size;
  }
  virtual bool getPressState( void ) = 0;
  static void startPolling( void );
  void startConversion();
  void onInterrupt( void );
  uint16 getX( void ) const
    {return m_X;}
  uint16 getY( void ) const
    {return m_Y;}
  uint16 getXAbsolute( void ) const
    {return m_AbsoluteX;}
  uint16 getYAbsolute( void ) const
    {return m_AbsoluteY;}
  void setPosition(uint16 x, uint16 y);
  bool setCalibration(sCcTouchMatrix Matrix);
protected:
  sCcTouchMatrix m_CalibMatrix;

private:
  uint16 m_AbsoluteX;
  uint16 m_AbsoluteY;
  uint16 m_X;
  uint16 m_Y;
  CcDateTime m_NextPoll;
};

#endif /* CCBUTTON_H_ */
