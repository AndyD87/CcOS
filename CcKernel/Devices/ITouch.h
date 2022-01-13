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
 * @file
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ITouch
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "IIo.h"
#include "CcObject.h"
#include "CcDateTime.h"

/**
 * @brief Matrix build through Calibration to get correct X/Y Values
 */
typedef struct{
  int32 A;    //!< Converion matrix A value
  int32 B;    //!< Converion matrix B value
  int32 C;    //!< Converion matrix C value
  int32 D;    //!< Converion matrix D value
  int32 E;    //!< Converion matrix E value
  int32 F;    //!< Converion matrix F value
  int32 Div;  //!< Converion matrix Div value
} STouchMatrix;

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED ITouch : public IDevice
{
public:
  /**
   * @brief Create touch object with initial values for Calibration.
   */
  ITouch();
  virtual ~ITouch() = default;

  /**
   * @brief Get raw postion of last touch
   * @param uiX: X Coordinate of last press
   * @param uiY: Y Coordinate of last press
   */
  virtual void getTouchState(uint16& uiX, uint16& uiY) = 0;

  /**
   * @brief Get value if touch is currently pressed
   * @return
   */
  virtual bool getPressState() = 0;

  /**
   * @brief Start converting raw input to real data
   */
  void startConversion();

  /**
   * @brief On interrupt of receive data from device.
   */
  void onInterrupt();
  //! @return Get last converted X value
  uint16 getX() const
  { return m_X; }

  //! @return Get last converted Y value
  uint16 getY() const
  { return m_Y; }

  //! @return Get last raw X value
  uint16 getXAbsolute() const
  { return m_AbsoluteX; }

  //! @return Get last raw Y value
  uint16 getYAbsolute() const
  { return m_AbsoluteY; }

  /**
   * @brief Overwrite raw postion values, than conversion can be executed.
   * @param x: New absolute X to set.
   * @param y: New absolute Y to set.
   */
  void setPosition(uint16 x, uint16 y);

  /**
   * @brief Set calibration matrix for converstion.
   * @param oMatrix: Conversion matrix.
   * @return True if Matrix is valid
   */
  bool setCalibration(STouchMatrix oMatrix);

protected:
  STouchMatrix m_CalibMatrix; //!< Calibration matrix to convert every raw value.

private:
  uint16 m_AbsoluteX;
  uint16 m_AbsoluteY;
  uint16 m_X;
  uint16 m_Y;
  CcDateTime m_NextPoll;
};
