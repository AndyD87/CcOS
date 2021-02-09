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
 * @brief     Class CcDate
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcDate
{
public:
  /**
   * @brief Constructor
   */
  CcDate() = default;

  /**
   * @brief Destructor
   */
  ~CcDate() = default;

  //! @return Get current year of this datetime
  int32 getYear() const;
  //! @return Get month since last full year
  uint8 getMonth() const;
  //! @return Get days since last full month
  uint8 getDay() const;
  //! @return Get hours since last full day
  uint8 getHour() const;
  //! @return Get minutes since last full seconds
  uint8 getMinute() const;
  //! @return Get seconds since last full minuts
  uint8 getSecond() const;
  //! @return Get days since 1970-01-01
  uint32 getDaysSinceEpoch() const;
  //! @return Get days since last full year
  uint16 getDaysSinceYear() const;

  //! @return Get editable timestamp in seconds
  int64& timestamp()
  { return m_iTimeStamp; }

  //! @param uiTimeStamp: Set timestamp in seconds
  void setTimestamp(int64 uiTimeStamp)
  { m_iTimeStamp = uiTimeStamp; }
  //! @return Get timestamp in seconds
  int64 getTimestamp() const
  { return m_iTimeStamp; }

private:
  void getMonthDay(uint8& uiMon, uint8& uiDay) const;
private:
  int64 m_iTimeStamp = 0;
};
