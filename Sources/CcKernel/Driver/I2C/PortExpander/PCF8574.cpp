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
  * @brief     Implementations for Class PCF8574
  **/

#include "PCF8574.h"

PCF8574::PCF8574(II2CSlave* pIfc) :
  m_pIfc(pIfc)
{
}

PCF8574::~PCF8574()
{
}

void PCF8574::setPin(uint8 uiNr, bool bValue)
{
  if(m_pIfc)
  {
    if(bValue)
    {
      m_uiValue |= 1 << uiNr;
    }
    else
    {
      m_uiValue &= ~(1 << uiNr);
    }
  }
}

bool PCF8574::getPin(uint8 uiNr)
{
  bool bValue =false;
  if(m_pIfc)
  {
    m_uiValue = getPort();
    bValue = (m_uiValue | (1 << uiNr)) != 0;
  }
  return bValue;
}

void PCF8574::setPort(uint8 uiValue)
{
  if(m_pIfc)
  {
    m_uiValue = uiValue;
    m_pIfc->writeByte(uiValue);
  }
}

uint8 PCF8574::getPort()
{
  uint8 uiValue = 0;
  if(m_pIfc)
  {
    m_pIfc->readByte(m_uiValue);
    uiValue = m_uiValue;
  }
  return uiValue;
}

bool PCF8574::write()
{
  bool bSuccess = false;
  if(m_pIfc)
  {
    if(m_pIfc &&
       1 == m_pIfc->writeByte(m_uiValue))
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}
