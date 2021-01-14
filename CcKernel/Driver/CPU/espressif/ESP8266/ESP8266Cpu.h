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
 * @brief     Class ESP8266Cpu
 */
#ifndef H_ESP8266Cpu_H_
#define H_ESP8266Cpu_H_

#include "CcBase.h"
#include "Driver/CPU/Common/FreeRTOS/FreeRTOSCpu.h"

class ESP8266Cpu : public FreeRTOSCpu
{
public:
  ESP8266Cpu();
  virtual ~ESP8266Cpu();
};

#endif // H_ESP8266Cpu_H_
