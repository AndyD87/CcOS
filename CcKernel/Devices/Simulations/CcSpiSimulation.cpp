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
 * @brief     Class CcSpiSimulation
 */

#include "Devices/Simulations/CcSpiSimulation.h"

CcStatus CcSpiSimulation::setMode(EMode eMode)
{
  m_eMode = eMode;
  return true;
}

size_t CcSpiSimulation::read(void* pBuffer, size_t uSize)
{
  m_oDataIn.read(pBuffer, uSize);
  m_oEventHandler.call(nullptr);
  return uSize;
}

size_t CcSpiSimulation::write(const void* pBuffer, size_t uSize)
{
  m_oDataOut.write(pBuffer, uSize);
  m_oEventHandler.call(nullptr);
  return uSize;
}

CcStatus CcSpiSimulation::setFrequency(uint32 uiFrequency)
{
  m_uiFrequency = uiFrequency;
  return true;
}

uint32 CcSpiSimulation::getFrequency()
{
  return m_uiFrequency;
}

size_t CcSpiSimulation::writeRead(void* pBuffer, size_t uSize)
{
  if (m_oDataIn.size() < uSize)
    m_oDataIn.resize(uSize);
  if (m_oDataOut.size() < uSize)
    m_oDataOut.resize(uSize);
  m_oDataOut.write(pBuffer, uSize);
  m_oDataIn.read(pBuffer, uSize);
  m_oEventHandler.call(nullptr);
  return uSize;
}
