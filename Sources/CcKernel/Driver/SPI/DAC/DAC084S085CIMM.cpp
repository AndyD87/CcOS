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
 * @brief     Implementations for Class DAC084S085CIMM
 **/

#include "DAC084S085CIMM.h"

DAC084S085CIMM::DAC084S085CIMM(ISpi* pSpiDevice) :
  m_pSpi(pSpiDevice)
{
}

DAC084S085CIMM::~DAC084S085CIMM()
{
}

bool DAC084S085CIMM::Init()
{
  bool bSuccess = true;
  // Init adc
  return bSuccess;
}

bool DAC084S085CIMM::SetChannel(uint8_t uiChannel, uint16_t uiValue)
{
  bool bSuccess = true;
  if(m_pSpi)
  {
    uint32_t uiDataField = 0;
    // Update only target DAC Command
    uiDataField = 1 << 12;
    if(uiChannel < 4)
    {
      uiDataField = uiChannel << 14;
    }
    else
    {
      bSuccess = false;
    }

    if(uiValue < 0x100)
    {
      uiDataField = uiValue << 4;
    }
    else
    {
      bSuccess = false;
    }

    if(bSuccess)
    {
      uchar ubData[2] = {0,0};
      // Copy data to right orderered byte array
      ubData[0] = static_cast<uchar>(uiDataField >> 8);
      ubData[1] = static_cast<uchar>(uiDataField >> 0);
      if(3 == m_pSpi->writeRead(ubData, sizeof(ubData)))
      {
        bSuccess = false;
      }
    }
  }
  else
  {
    bSuccess = false;
  }
  return bSuccess;
}
