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
    if((static_cast<size_t>(1) << i) | uiPinMask)
    {
      m_eDirections[i] = eDirection;
    }
  }
  return bSuccess;
}

bool CcGpioPortSimulation::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
{
  m_eDirections[uiPin] = eDirection;
  CCUNUSED(uiValue);
  return true;
}

IGpioPin::EDirection CcGpioPortSimulation::getDirection(size_t uiPin)
{
  return m_eDirections[uiPin];
}

bool CcGpioPortSimulation::setValue(size_t uiPin, bool bValue)
{
  if(uiPin < count())
  {
    m_aValues[uiPin] = bValue;
    return true;
  }
  else
  {
    return false;
  }
}

bool CcGpioPortSimulation::getValue(size_t uiPin)
{
  return m_aValues[uiPin];
}
