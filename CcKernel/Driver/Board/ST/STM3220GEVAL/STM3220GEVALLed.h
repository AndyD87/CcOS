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
 * @page      STM3220GEVAL
 * @subpage   STM3220GEVALLed
 *
 * @page      STM3220GEVALLed
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef H_STM3220GEVALLed_H_
#define H_STM3220GEVALLed_H_

#include <Driver/Board/ST/STM3220GEVAL/STM3220GEVAL.h>
#include "Devices/ILed.h"

class CcByteArray;

class STM3220GEVALLedPrivate;

/**
 * @brief Generate SM32F407V CPU Device
 */
class STM3220GEVALLed : public ILed
{
public:
  /**
   * @brief Constructor
   */
  STM3220GEVALLed(uint8 uiLedNr);

  /**
   * @brief Destructor
   */
  virtual ~STM3220GEVALLed();

  virtual CcStatus setMaxBirghtness(uint16 uiBrightness) override;

  virtual CcStatus on(uint16 uiBrightness = 0) override;

  virtual CcStatus off() override;

  virtual CcStatus toggle() override;

  virtual bool IsOn();
private:
  void mapPortPin(uint8 uiPort, uint8 uiPin);
private:
  STM3220GEVALLedPrivate* m_pPrivate;
};

#endif // H_STM3220GEVALLed_H_
