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
 * @page      ESP8266
 * @subpage   ESP8266Timer
 *
 * @page      ESP8266Timer
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ESP8266Timer
 */
#ifndef H_ESP8266Timer_H_
#define H_ESP8266Timer_H_

#include "CcBase.h"
#include "CcDateTime.h"
#include "Devices/ITimer.h"

class ESP8266Timer : public ITimer
{
public:
  ESP8266Timer();
  virtual ~ESP8266Timer();

  virtual CcStatus setState(EState eState) override;
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;
  static void timeoutEvent(void* pArgv);

private:
  CcDateTime m_oTimeout;
};

#endif // H_ESP8266Timer_H_
