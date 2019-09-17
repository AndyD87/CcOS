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
 * @subpage   STM32F407VTimer
 * 
 * @page      STM32F407VTimer
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407VTimer
 */
#ifndef H_STM32F407VTimer_H_
#define H_STM32F407VTimer_H_

#include "CcBase.h"
#include "Devices/ITimer.h"

class STM32F407VTimerPrivate;

class STM32F407VTimer : public ITimer
{
public: //methods
  STM32F407VTimer();
  virtual ~STM32F407VTimer();

  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;
  virtual CcStatus start() override;
  virtual CcStatus stop() override;

  virtual bool timeout() override;
private: //member
  STM32F407VTimerPrivate* m_pPrivate;
};

#endif // H_STM32F407VTimer_H_
