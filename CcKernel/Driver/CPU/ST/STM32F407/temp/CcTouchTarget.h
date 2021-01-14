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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcTouchTarget
 */
#ifndef H_CCTOUCHTARGET_H_
#define H_CCTOUCHTARGET_H_

#include "CcBase.h"
#include "STM32.h"
#include "dev/ITouch.h"
#include "dev/ITimer.h"

class CcTouchTarget : public ITouch
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
  void initIO;
  void getTouchState(uint16 *x, uint16 *y);
  bool getPressState();

private:
  SPI_HandleTypeDef m_SpiHandle;
};

#endif // H_CCTOUCHTARGET_H_
