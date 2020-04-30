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
 * @brief     Class CcEepromSimulation
 */

#include "Devices/Simulations/CcEepromSimulation.h"
#include "CcStatic.h"

CcEepromSimulation::CcEepromSimulation(size_t uiSize) :
  m_oData(uiSize),
  m_oStream(m_oData, true)
{
  CcStatic::memset(m_oData.getArray(), 0, m_oData.size());
}

size_t CcEepromSimulation::size() const
{
  return m_oData.size();
}

size_t CcEepromSimulation::read(void* pBuffer, size_t uSize)
{
  return m_oStream.read(pBuffer, uSize);
}

size_t CcEepromSimulation::write(const void* pBuffer, size_t uSize)
{
  return m_oStream.write(pBuffer, uSize);
}

CcStatus CcEepromSimulation::setPosition(size_t uiPosition)
{
  return m_oStream.setPosition(uiPosition);
}

CcStatus CcEepromSimulation::open(EOpenFlags eFlags)
{
  return m_oStream.open(eFlags);
}

CcStatus CcEepromSimulation::close()
{
  return true;
}

CcStatus CcEepromSimulation::cancel()
{
  return false;
}
