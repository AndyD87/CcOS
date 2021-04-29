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
 * @brief     Class CcDateTime
 **/
#pragma once

#include "CcBase.h"

//! Create CcDateTime with numer of seconds
#define CcDateTimeFromSeconds(SECONDS)  CcDateTime(SECONDS * 1000000)
//! Create CcDateTime with numer of miliseconds
#define CcDateTimeFromMSeconds(SECONDS) CcDateTime(SECONDS * 1000)
//! Create CcDateTime with numer of microseconds
#define CcDateTimeFromUSeconds(SECONDS) CcDateTime(SECONDS)
//! Create CcDateTime with numer of nanoseconds
#define CcDateTimeFromNSeconds(SECONDS) CcDateTime(SECONDS / 100)

class CcString;

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcDateTime
{
public:
  /**
   * @brief Constructor
   */
  CcDateTime() = default;

  /**
   * @brief Constructor
   */
  CcDateTime(uint64 uiFileTimeUs);

  /**
   * @brief Destructor
   */
  ~CcDateTime() = default;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcDateTime& oToCompare) const
    { return (oToCompare.m_iTimeStampUs == m_iTimeStampUs);}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcDateTime& oToCompare) const
    { return (oToCompare.m_iTimeStampUs != m_iTimeStampUs);}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator<(const CcDateTime& oToCompare) const
  { return m_iTimeStampUs < oToCompare.m_iTimeStampUs;}

  /**
   * @brief Compare with second item if this is lower than of even
   * @param oToCompare: Item to compare to
   * @return true if this is lower or even
   */
  inline bool operator<=(const CcDateTime& oToCompare) const
    { return operator<(oToCompare) | operator==(oToCompare);}

  /**
   * @brief Compare with second item if this is greater than
   * @param oToCompare: Item to compare to
   * @return true if this is greater
   */
  inline bool operator>(const CcDateTime& oToCompare) const
  { return m_iTimeStampUs > oToCompare.m_iTimeStampUs;}

  /**
   * @brief Compare with second item if this is greater than of even
   * @param oToCompare: Item to compare to
   * @return true if this is greater or even
   */
  inline bool operator>=(const CcDateTime& oToCompare) const
  { return operator>(oToCompare) | operator==(oToCompare);}

  /**
   * @brief Add number of seconds to this date time
   * @param iAddSeconds: Number of seconds to add
   * @return Handle to this
   */
  inline CcDateTime& operator+=(uint8 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(int8 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(uint16 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(int16 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(uint32 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(int32 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(uint64 iAddSeconds)
  { return addSeconds(iAddSeconds);}
  //! @copydoc operator+=()
  inline CcDateTime& operator+=(int64 iAddSeconds)
  { return addSeconds(iAddSeconds);}

  /**
   * @brief Add number of seconds to with this date time and create new
   * @param iAddSeconds: Number of seconds to add
   * @return New date time
   */
  inline CcDateTime operator+(uint8 iAddSeconds) const
  { return operator+((int64)iAddSeconds);}
  //! @copydoc operator+()
  inline CcDateTime operator+(int8 iAddSeconds) const
  { return operator+((int64)iAddSeconds);}
  //! @copydoc operator+()
  inline CcDateTime operator+(uint16 iAddSeconds) const
  { return operator+((int64)iAddSeconds);}
  //! @copydoc operator+()
  inline CcDateTime operator+(int16 iAddSeconds) const
  { return operator+((int64)iAddSeconds);}
  //! @copydoc operator+()
  inline CcDateTime operator+(uint32 iAddSeconds) const
  { return operator+((int64)iAddSeconds);}
  //! @copydoc operator+()
  inline CcDateTime operator+(int32 iAddSeconds) const
  { return operator+((int64)iAddSeconds);}
  //! @copydoc operator+()
  CcDateTime operator+(int64 iAddSeconds) const;
  //! @copydoc operator+()
  CcDateTime operator+(uint64 iAddSeconds) const;

  /**
   * @brief Add datetime to this date time and create new
   * @param oDateTimeToAdd: Datetime to add
   * @return New datetime
   */
  CcDateTime operator+(const CcDateTime& oDateTimeToAdd) const;

  /**
   * @brief Add time from another DateTime object to this:
   * @param oDateTimeToAdd: DateTime object to add
   * @return Handle to this
   */
  inline CcDateTime operator+=(const CcDateTime& oDateTimeToAdd)
  { return add(oDateTimeToAdd);}

  /**
   * @brief Remove number of seconds from this date time
   * @param iAddSeconds: Number of seconds to substract
   * @return Handle to this
   */
  inline CcDateTime& operator-=(uint8 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(int8 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(uint16 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(int16 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(uint32 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(int32 iAddSeconds)
  { return addSeconds((int64)iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(uint64 iAddSeconds)
  { return addSeconds(iAddSeconds);}
  //! @copydoc operator-=()
  inline CcDateTime& operator-=(int64 iAddSeconds)
  { return addSeconds(iAddSeconds);}

  /**
   * @brief Remove number of seconds from this date time and create new
   * @param iAddSeconds: Number of seconds to substract
   * @return New datetime
   */
  inline CcDateTime operator-(uint8 iAddSeconds) const
  { return operator-((int64)iAddSeconds);}
  //! @copydoc operator-()
  inline CcDateTime operator-(int8 iAddSeconds) const
  { return operator-((int64)iAddSeconds);}
  //! @copydoc operator-()
  inline CcDateTime operator-(uint16 iAddSeconds) const
  { return operator-((int64)iAddSeconds);}
  //! @copydoc operator-()
  inline CcDateTime operator-(int16 iAddSeconds) const
  { return operator-((int64)iAddSeconds);}
  //! @copydoc operator-()
  inline CcDateTime operator-(uint32 iAddSeconds) const
  { return operator-((int64)iAddSeconds);}
  //! @copydoc operator-()
  inline CcDateTime operator-(int32 iAddSeconds) const
  { return operator-((int64)iAddSeconds);}
  //! @copydoc operator-()
  CcDateTime operator-(int64 iAddSeconds) const;
  //! @copydoc operator-()
  CcDateTime operator-(uint64 iAddSeconds) const;

  /**
   * @brief Remove datetime from this date time and create new
   * @param oDateTimeToAdd: Datetime to substract
   * @return New datetime
   */
  CcDateTime operator-(const CcDateTime& oDateTimeToAdd) const;

  //! @param uiFiletime: Set time from file with 100ns value
  void setFiletime(uint64 uiFiletime);
  //! @return Conver timt to filetime
  uint64 getFiletime() const;

  //! @return Get value in microseconds
  int64& timestampUs()
  { return m_iTimeStampUs; }

  //! @return Get current year of this datetime
  int32 getYear() const;
  //! @return Get month since last full year
  uint8 getMonth() const;
  //! @return Get days since last full year
  uint16 getDaysSinceYear() const;
  //! @return Get days since last full month
  uint8 getDay() const;
  //! @return Get hours since last full day
  int8 getHour() const;
  //! @return Get minutes since last full seconds
  int8 getMinute() const;
  //! @return Get seconds since last full minuts
  int8 getSecond() const;
  //! @return Get milliseconds since last full seconds
  int16 getMSecond() const;
  //! @return Get microseconds since last full milisecond
  int16 getUSecond() const;
  //! @return Get number of days from this datetime
  int32 getDaysSinceEpoch() const;

  //! @param uiTimeStamp: Set timestamp in seconds
  void setTimestampS(int64 uiTimeStamp);
  //! @param uiTimeStamp: Set timestamp in milliseconds
  void setTimestampMs(int64 uiTimeStamp);
  //! @param uiTimeStamp: Set timestamp in microseconds
  void setTimestampUs(int64 uiTimeStamp);

  //! @return Get timestamp in seconds
  int64 getTimestampS() const;
  //! @return Get timestamp in milliseconds
  int64 getTimestampMs() const;
  //! @return Get timestamp in microseconds
  int64 getTimestampUs() const;
  //! @return Get timestamp in nanoseconds
  int64 getTimestampNs() const;

  /**
   * @brief Add seconds to this timestamp
   * @param iTimeStamp: number of seconds to add
   * @return Handle to this
   */
  CcDateTime& addSeconds(int64 iTimeStamp)
  { m_iTimeStampUs += 1000 * 1000 * iTimeStamp; return *this; }

  /**
   * @brief Add milliseconds to this timestamp
   * @param iTimeStamp: number of milliseconds to add
   * @return Handle to this
   */
  CcDateTime& addMSeconds(int64 iTimeStamp)
  { m_iTimeStampUs += 1000 * iTimeStamp; return *this; }

  /**
   * @brief Add microseconds to this timestamp
   * @param iTimeStamp: number of mcroseconds to add
   * @return Handle to this
   */
  CcDateTime& addUSeconds(int64 iTimeStamp)
  { m_iTimeStampUs += iTimeStamp; return *this; }

  /**
   * @brief Add another timestamp to this timestamp
   * @param oDateTimeToAdd: timestamp to add
   * @return Handle to this
   */
  CcDateTime& add(const CcDateTime& oDateTimeToAdd);

  /**
   * @brief Remove seconds from this timestamp
   * @param iTimeStamp: number of seconds to remove
   * @return Handle to this
   */
  CcDateTime& subSeconds(int64 iTimeStamp);

  /**
   * @brief Remove milliseconds from this timestamp
   * @param iTimeStamp: number of milliseconds to remove
   * @return Handle to this
   */
  CcDateTime& subMSeconds(int64 iTimeStamp);

  /**
   * @brief Remove microseconds from this timestamp
   * @param iTimeStamp: number of microseconds to remove
   * @return Handle to this
   */
  CcDateTime& subUSeconds(int64 iTimeStamp);

  /**
   * @brief Remove another datetime from this timestamp
   * @param oDateTimeToAdd: datetime to remove from this
   * @return Handle to this
   */
  CcDateTime& sub(CcDateTime oDateTimeToAdd);

  CcDateTime& fromDateTime(uint16 uiYear, uint8 uiMonth, uint8 uiDay, uint8 uiHours, uint8 uiMinutes, uint8 uiSeconds);

  /**
   * @brief Get time as formated string
   *        Format|  Result   | Description
   *        ------|-----------|------------
   *        yyyy  |  2021     | Year 4 number
   *        yyyy  |    21     | Year 2 number
   *        MMMM  |  January  | Month long name
   *        MMM   |   Jan     | Month short name
   *        MM    |    01     | Month 2 numbers
   *        M     |     1     | Month 1/2 numbers
   *        dddd  |  Saturday | Day long name
   *        ddd   |   Sat     | Day short name
   *        dd    |    06     | Day 2 numbers
   *        d     |     6     | Day 1/2 numbers
   *        hh    |    01     | Hours 2 numbers
   *        h     |     1     | Hours 1/2 numbers
   *        mm    |    05     | Minutes 2 numbers
   *        m     |     5     | Minutes 1/2 numbers
   *        ss    |    05     | Seconds 2 numbers
   *        s     |     5     | Seconds 1/2 numbers
   *        zzz   |     5     | Milliseconds 3 numbers
   *        uuu   |    05     | Microseconds 3 numbers
   *
   * @param sFormat: Format of datetime to get
   * @return String parsed as defined in format
   */
  CcString getString(const CcString& sFormat) const;

  /**
   * @brief Get sql datetime format.
   *        Used format: yyyy-MM-dd hh:mm:ss.zzzuuu
   * @param uiPrecision: persion of seconds, 3 for mili seconds, 6 for microseconds
   * @return Formated string
   */
  CcString getSqlDateTime(uint8 uiPrecision = 0) const;

private:
  void getMonthDay(uint8& uiMon, uint8& uiDay) const;

private:
  int64 m_iTimeStampUs = 0;
};
