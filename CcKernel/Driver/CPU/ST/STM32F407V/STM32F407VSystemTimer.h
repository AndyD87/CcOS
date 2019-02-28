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
 * @subpage   STM32F407VSystemTimer
 * 
 * @page      STM32F407VSystemTimer
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407VSystemTimer
 */
#ifndef _CCLIB_STM32F407VSystemTimer_H_
#define _CCLIB_STM32F407VSystemTimer_H_

#include "CcBase.h"
#include "Devices/CcTimer.h"

class STM32F407VSystemTimerPrivate;

class STM32F407VSystemTimer : public CcTimer
{
public: //methods
  STM32F407VSystemTimer();
  virtual ~STM32F407VSystemTimer();

  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;
  virtual CcStatus start() override;
  virtual CcStatus stop() override;

private: //member
  STM32F407VSystemTimerPrivate* m_pPrivate;
};

#endif /* _CCLIB_STM32F407VSystemTimer_H_ */
