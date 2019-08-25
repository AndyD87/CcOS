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
 * @brief     Class CcGpioPortSimulation
 */

#include "Devices/Simulations/CcGpioPortSimulation.h"

bool CcGpioPortSimulation::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
{
  CCUNUSED(uiValue);
  bool bSuccess = true;
  for(int i = 0; i < count(); i++)
  {
    if((1 << i) | uiPinMask)
    {
      m_eDirections[i] = eDirection;
    }
  }
  return bSuccess;
}

bool CcGpioPortSimulation::setDirection(size_t uiPin, IGpioPin::EDirection eDirection)
{
  m_eDirections[uiPin] = eDirection;
  return true;
}

IGpioPin::EDirection CcGpioPortSimulation::getDirection(size_t uiPin)
{
  return m_eDirections[uiPin];
}

void CcGpioPortSimulation::setValue(size_t uiPin, bool bValue)
{
  m_aValues[uiPin] = bValue;
}

bool CcGpioPortSimulation::getValue(size_t uiPin)
{
  return m_aValues[uiPin];
}
