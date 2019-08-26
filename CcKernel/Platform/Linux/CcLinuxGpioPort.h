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
 * @page      Linux
 * @subpage   CcLinuxGpioPort
 *
 * @page      CcLinuxGpioPort
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxGpioPort
 */

#ifndef H_CcLinuxGpioPort_H_
#define H_CcLinuxGpioPort_H_

#include "Devices/IGpioPort.h"
#include "CcLinuxGpioPin.h"
#include "CcVector.h"

typedef struct
{
  uint8 uiNr;
  IGpioPin *cPin;
} SGpioPinItem;

class CcLinuxGpioPort : public IGpioPort
{
public:
  CcLinuxGpioPort();
  virtual ~CcLinuxGpioPort();

  void init();

  virtual inline uint8 count() const override
    {return 0xff;}
  virtual IGpioPin* getPin(uint8 uiNr) override;
  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection) override;
  virtual IGpioPin::EDirection getDirection(size_t uiPin) override;
  virtual bool setValue(size_t uiPin, bool bValue) override;
  virtual bool getValue(size_t uiPin) override;
private:
  CcVector<SGpioPinItem> m_lcPins;
};

#endif /* H_CcLinuxGpioPort_H_ */
