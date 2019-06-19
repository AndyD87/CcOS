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
 * @subpage   CcDateTime
 *
 * @page      CcDateTime
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDateTime
 **/
#ifndef _CcDateTime_H_
#define _CcDateTime_H_

#include "CcBase.h"
#include "CcKernelBase.h"

#define CcDateTimeFromSeconds(SECONDS) CcDateTime(SECONDS * 1000000)
#define CcDateTimeFromMSeconds(SECONDS) CcDateTime(SECONDS * 1000)
#define CcDateTimeFromNSeconds(SECONDS) CcDateTime(SECONDS / 100)

class CcString;

/**
 * @brief Class impelmentation
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
  inline bool operator<=(const CcDateTime& oToCompare) const
    { return operator<(oToCompare) | operator==(oToCompare);}
  
  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator>(const CcDateTime& oToCompare) const
    { return m_iTimeStampUs > oToCompare.m_iTimeStampUs;}
  inline bool operator>=(const CcDateTime& oToCompare) const
    { return operator>(oToCompare) | operator==(oToCompare);}

  inline CcDateTime operator+=(const CcDateTime& oDateTimeToAdd)
    { return add(oDateTimeToAdd);}
  inline CcDateTime& operator+=(uint8 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator+=(int8 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator+=(uint16 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator+=(int16 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator+=(uint32 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator+=(int32 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator+=(uint64 iAddSeconds)
    { return addSeconds(iAddSeconds);}
  inline CcDateTime& operator+=(int64 iAddSeconds)
    { return addSeconds(iAddSeconds);}
  inline CcDateTime operator+(uint8 iAddSeconds) const
    { return operator+((int64)iAddSeconds);}
  inline CcDateTime operator+(int8 iAddSeconds) const
    { return operator+((int64)iAddSeconds);}
  inline CcDateTime operator+(uint16 iAddSeconds) const
    { return operator+((int64)iAddSeconds);}
  inline CcDateTime operator+(int16 iAddSeconds) const
    { return operator+((int64)iAddSeconds);}
  inline CcDateTime operator+(uint32 iAddSeconds) const
    { return operator+((int64)iAddSeconds);}
  inline CcDateTime operator+(int32 iAddSeconds) const
    { return operator+((int64)iAddSeconds);}
  CcDateTime operator+(int64 iAddSeconds) const;
  CcDateTime operator+(uint64 iAddSeconds) const;
  CcDateTime operator+(const CcDateTime& oDateTimeToAdd) const;
  
  inline CcDateTime& operator-=(uint8 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator-=(int8 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator-=(uint16 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator-=(int16 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator-=(uint32 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator-=(int32 iAddSeconds)
    { return addSeconds((int64)iAddSeconds);}
  inline CcDateTime& operator-=(uint64 iAddSeconds)
    { return addSeconds(iAddSeconds);}
  inline CcDateTime& operator-=(int64 iAddSeconds)
    { return addSeconds(iAddSeconds);}
  inline CcDateTime operator-(uint8 iAddSeconds) const
    { return operator-((int64)iAddSeconds);}
  inline CcDateTime operator-(int8 iAddSeconds) const
    { return operator-((int64)iAddSeconds);}
  inline CcDateTime operator-(uint16 iAddSeconds) const
    { return operator-((int64)iAddSeconds);}
  inline CcDateTime operator-(int16 iAddSeconds) const
    { return operator-((int64)iAddSeconds);}
  inline CcDateTime operator-(uint32 iAddSeconds) const
    { return operator-((int64)iAddSeconds);}
  inline CcDateTime operator-(int32 iAddSeconds) const
    { return operator-((int64)iAddSeconds);}
  CcDateTime operator-(int64 iAddSeconds) const;
  CcDateTime operator-(uint64 iAddSeconds) const;
  CcDateTime operator-(const CcDateTime& oDateTimeToAdd) const;

  void setFiletime(uint64 uiFiletime);
  uint64 getFiletime() const;

  int64& timestampUs()
    { return m_iTimeStampUs; }

  int32 getYear() const;
  uint8 getMonth() const;
  uint16 getDaysSinceYear() const;
  uint8 getDay() const;
  int8 getHour() const;
  int8 getMinute() const;
  int8 getSecond() const;
  int16 getMSecond() const;
  int16 getUSecond() const;
  int32 getDaysSinceEpoch() const;

  void setTimestampS(int64 uiTimeStamp);
  void setTimestampMs(int64 uiTimeStamp);
  void setTimestampUs(int64 uiTimeStamp);
  int64 getTimestampS() const;
  int64 getTimestampMs() const;
  int64 getTimestampUs() const;

  CcDateTime& addSeconds(int64 iTimeStamp)
    { m_iTimeStampUs += 1000 * 1000 * iTimeStamp; return *this; }
  CcDateTime& addMSeconds(int64 iTimeStamp)
    { m_iTimeStampUs += 1000 * iTimeStamp; return *this; }
  CcDateTime& addUSeconds(int64 iTimeStamp)
    { m_iTimeStampUs += iTimeStamp; return *this; }
  CcDateTime& add(CcDateTime oDateTimeToAdd);

  CcDateTime& subSeconds(int64 iTimeStamp);
  CcDateTime& subMSeconds(int64 iTimeStamp);
  CcDateTime& subUSeconds(int64 iTimeStamp);
  CcDateTime& sub(CcDateTime oDateTimeToAdd);

  CcString getString(const CcString& sFormat) const;
  CcString getSqlDateTime(uint8 uiPrecision = 0) const;

private:
  void getMonthDay(uint8& uiMon, uint8& uiDay) const;

private:
  int64 m_iTimeStampUs = 0;
};

#endif /* _CcDateTime_H_ */
