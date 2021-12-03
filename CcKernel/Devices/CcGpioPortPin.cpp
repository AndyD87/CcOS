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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGpioPortPin
 */

#include "Devices/CcGpioPortPin.h"
#include "Devices/IGpioPort.h"

bool CcGpioPortPin::setDirection(EDirection eDirection, size_t uiValue)
{
  return m_pPort->setDirection(m_uiPinNr, eDirection, uiValue);
}

CcGpioPortPin::EDirection CcGpioPortPin::getDirection()
{
  return m_pPort->getDirection(m_uiPinNr);
}

void CcGpioPortPin::setValue(bool bValue)
{
  m_pPort->setPinValue(m_uiPinNr, bValue);
}

bool CcGpioPortPin::getValue()
{
  return m_pPort->getPinValue(m_uiPinNr);
}

bool CcGpioPortPin::setSpeedValue(size_t uiValue)
{
  return m_pPort->setSpeedValue(m_uiPinNr, uiValue);
}
