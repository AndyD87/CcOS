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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class ESP8266Cpu
 **/

#include <Driver/CPU/espressif/ESP8266/ESP8266Cpu.h>
#include "CcSystem.h" // we have to implement uptime function
#include "CcDateTime.h" // we have to implement uptime function
CCEXTERNC_BEGIN
#include <FreeRTOS.h>
#include <task.h>
#include "stdio.h"
CCEXTERNC_END

typedef void(*TaskFunction_t)(void* pParam);

CCEXTERNC void __cxa_pure_virtual() { while (1); }


CcDateTime CcSystem::getUpTime()
{
  return CcDateTimeFromSeconds(static_cast<int64>(clock())/CLOCKS_PER_SEC);
}

void CcSystem::sleep(uint32 uiTimeoutMs)
{
  if(uiTimeoutMs < portTICK_PERIOD_MS) uiTimeoutMs = portTICK_PERIOD_MS;
  vTaskDelay(uiTimeoutMs / portTICK_PERIOD_MS);
}

ESP8266Cpu::ESP8266Cpu()
{
}

ESP8266Cpu::~ESP8266Cpu()
{
}
