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
 * @brief     Implemtation of class CcDate
 */
#include "CcDate.h"

CcDate::CcDate( void )
{
}

CcDate::~CcDate( void )
{
}

int32 CcDate::getYear() const
{
  uint32 uiAllDays = getDaysSinceEpoch();
  uint32 uiYearsSimple = uiAllDays / 365;
  uint32 uiSwitchYears = 0;
  if (uiYearsSimple > 1)
  {
    uiSwitchYears = (uiYearsSimple - 2) / 4;
    uint32 uiDaysNegSwitches = uiAllDays - uiSwitchYears;
    uint32 uiYearsFirstComplex = uiDaysNegSwitches / 365;
    if (uiYearsFirstComplex == uiYearsSimple)
    {
      return (uint16)uiYearsFirstComplex + 1970;
    }
    else
    {
      return (uint16)uiYearsFirstComplex + 1969;
    }
  }
  else
    return uiYearsSimple + 1970;
}

uint8 CcDate::getMonth() const
{
  uint8 uiMonth;
  uint8 uiDay;
  getMonthDay(uiMonth, uiDay);
  return uiMonth;
}

uint8 CcDate::getDay() const
{
  uint8 uiMonth;
  uint8 uiDay;
  getMonthDay(uiMonth, uiDay);
  return uiDay;
}

uint8 CcDate::getHour() const
{
  return (uint8)((m_iTimeStamp % (3600 * 24)) / 3600);
}

uint8 CcDate::getMinute() const
{
  return (uint8) ((m_iTimeStamp % 3600) / 60);
}

uint8 CcDate::getSecond() const
{
  return (uint8) (m_iTimeStamp % 60);
}

uint32 CcDate::getDaysSinceEpoch() const
{
  return (uint32) (m_iTimeStamp / (3600 * 24)) + 1;
}

uint16 CcDate::getDaysSinceYear() const
{
  uint32 uiYears = getYear() - 1970;
  uint32 uiSwitchYears = 0;
  if(uiYears > 1)
    uiSwitchYears = (uiYears - 2) / 4;
  return (uint16)(getDaysSinceEpoch() - (uiYears * 365) - uiSwitchYears);
}

void CcDate::getMonthDay(uint8& uiMonth, uint8& uiDay) const
{
  uint16 uiDaysYear = getDaysSinceYear();
  uint16 uiSwitchYear = (getYear() % 4 == 0) ? 1 : 0;
  if (uiDaysYear < 32)
  {
    uiMonth = 1;
    uiDay = (uint8)(uiDaysYear);
  }
  else if (uiDaysYear < 60 + uiSwitchYear)
  {
    uiMonth = 2;
    uiDay = (uint8)(uiDaysYear - 31);
  }
  else if (uiDaysYear < 91 + uiSwitchYear)
  {
    uiMonth = 3;
    uiDay = (uint8)(uiDaysYear - (59 + uiSwitchYear));
  }
  else if (uiDaysYear < 121 + uiSwitchYear)
  {
    uiMonth = 4;
    uiDay = (uint8)(uiDaysYear - (90 + uiSwitchYear));
  }
  else if (uiDaysYear < 152 + uiSwitchYear)
  {
    uiMonth = 5;
    uiDay = (uint8)(uiDaysYear - (120 + uiSwitchYear));
  }
  else if (uiDaysYear < 182 + uiSwitchYear)
  {
    uiMonth = 6;
    uiDay = (uint8)(uiDaysYear - (151 + uiSwitchYear));
  }
  else if (uiDaysYear < 213 + uiSwitchYear)
  {
    uiMonth = 7;
    uiDay = (uint8)(uiDaysYear - (181 + uiSwitchYear));
  }
  else if (uiDaysYear < 244 + uiSwitchYear)
  {
    uiMonth = 8;
    uiDay = (uint8)(uiDaysYear - (212 + uiSwitchYear));
  }
  else if (uiDaysYear < 274 + uiSwitchYear)
  {
    uiMonth = 9;
    uiDay = (uint8)(uiDaysYear - (243 + uiSwitchYear));
  }
  else if (uiDaysYear < 305 + uiSwitchYear)
  {
    uiMonth = 10;
    uiDay = (uint8)(uiDaysYear - (273 + uiSwitchYear));
  }
  else if (uiDaysYear < 335 + uiSwitchYear)
  {
    uiMonth = 11;
    uiDay = (uint8)(uiDaysYear - (304 + uiSwitchYear));
  }
  else if (uiDaysYear < 366 + uiSwitchYear)
  {
    uiMonth = 12;
    uiDay = (uint8)(uiDaysYear - (334 + uiSwitchYear));
  }
  else
  { 
    uiMonth = 0;
    uiDay = 0;
  }
}
