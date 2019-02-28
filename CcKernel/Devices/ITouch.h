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
 * @subpage   ITouch
 *
 * @page      ITouch
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ITouch
 */

#ifndef _ITouch_H_
#define _ITouch_H_

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
} STouchMatrix;

/**
 * @brief Callback values for ITouch
 */
typedef enum {
  ITouch_ONCLICK = 0,
}eITouchCBNr;

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED ITouch : public CcIODevice {
public:
  /**
   * @brief Constructor
   */
  ITouch();

  /**
   * @brief Destructor
   */
  virtual ~ITouch();

  /**
   * @brief Init Touchpanel, must be implemented by DeviceClass
   */
  virtual CcStatus open(EOpenFlags flags) override = 0;
  virtual void getTouchState(uint16 *x, uint16 *y) = 0;
  virtual size_t read(void* pBuffer, size_t uSize) override
    { CCUNUSED(pBuffer); return uSize; }
  virtual size_t write(const void* pBuffer, size_t uSize) override
    { CCUNUSED(pBuffer); return uSize;
  }
  virtual bool getPressState() = 0;
  static void startPolling();
  void startConversion();
  void onInterrupt();
  uint16 getX() const
    {return m_X;}
  uint16 getY() const
    {return m_Y;}
  uint16 getXAbsolute() const
    {return m_AbsoluteX;}
  uint16 getYAbsolute() const
    {return m_AbsoluteY;}
  void setPosition(uint16 x, uint16 y);
  bool setCalibration(STouchMatrix Matrix);
protected:
  STouchMatrix m_CalibMatrix;

private:
  uint16 m_AbsoluteX;
  uint16 m_AbsoluteY;
  uint16 m_X;
  uint16 m_Y;
  CcDateTime m_NextPoll;
};

#endif /* _CCBUTTON_H_ */
