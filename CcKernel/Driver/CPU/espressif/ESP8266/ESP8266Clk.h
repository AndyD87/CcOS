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
 * @brief     Class ESP8266Clk
 */
#ifndef H_ESP8266Clk_H_
#define H_ESP8266Clk_H_

#include "CcBase.h"
#include "Devices/IClock.h"

class ESP8266Clk : public IClock
{
public:
  ESP8266Clk();
  virtual ~ESP8266Clk();

  virtual CcStatus setFrequency(uint64 uiHz) override;
};

#endif // H_ESP8266Clk_H_
