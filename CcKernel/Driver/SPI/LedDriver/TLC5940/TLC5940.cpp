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
 * @brief     Implemtation of class TLC5940
 */
#include "TLC5940.h"
#include "Devices/ISpi.h"

TLC5940::TLC5940(ISpi* pSpiDevice) :
  m_pSpiDevice(pSpiDevice)
{
}

TLC5940::~TLC5940()
{
}

void TLC5940::write()
{
}


void TLC5940::setChipCount(size_t uiNumberOfChips)
{
  m_uiChipNr = uiNumberOfChips;
}