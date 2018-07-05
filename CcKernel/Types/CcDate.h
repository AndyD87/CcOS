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
 * @page      Types
 * @subpage   CcDate
 *
 * @page      CcDate
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDate
 **/
#ifndef _CcDate_H_
#define _CcDate_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcDate
{
public:
  /**
   * @brief Constructor
   */
  CcDate(void );

  /**
   * @brief Destructor
   */
  ~CcDate(void );

  int32 getYear() const;
  uint8 getMonth() const;
  uint8 getDay() const;
  uint8 getHour() const;
  uint8 getMinute() const;
  uint8 getSecond() const;
  uint32 getDaysSinceEpoch() const;
  uint16 getDaysSinceYear() const;

  int64& timestamp()
    { return m_iTimeStamp; }

  void setTimestamp(int64 uiTimeStamp)
    { m_iTimeStamp = uiTimeStamp; }
  int64 getTimestamp() const
    { return m_iTimeStamp; }

private:
  void getMonthDay(uint8& uiMon, uint8& uiDay) const;
private:
  int64 m_iTimeStamp = 0;
};

#endif /* _CcDate_H_ */
