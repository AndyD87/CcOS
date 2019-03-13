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
#include "Devices/ITimer.h"

class STM32F407VSystemTimer : public ITimer
{
public: //methods
  /**
   * Load default clock configuration.
   * @brief  System Clock Configuration
   *         The system Clock is configured as follow :
   *            System Clock source            = PLL (HSE)
   *            SYSCLK(Hz)                     = 168000000
   *            HCLK(Hz)                       = 168000000
   *            AHB Prescaler                  = 1
   *            APB1 Prescaler                 = 4
   *            APB2 Prescaler                 = 2
   *            HSE Frequency(Hz)              = 8000000
   *            PLL_M                          = 8
   *            PLL_N                          = 336
   *            PLL_P                          = 2
   *            PLL_Q                          = 7
   *            VDD(V)                         = 3.3
   *            Main regulator output voltage  = Scale1 mode
   *            Flash Latency(WS)              = 5
   */
  STM32F407VSystemTimer();
  virtual ~STM32F407VSystemTimer();

  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;
  virtual CcStatus start() override;
  virtual CcStatus stop() override;
};

#endif /* _CCLIB_STM32F407VSystemTimer_H_ */
