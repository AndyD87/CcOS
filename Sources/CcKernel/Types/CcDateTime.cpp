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
 * @brief     Implemtation of class CcDateTime
 */
#include "CcDateTime.h"
#include "CcString.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"

//!< Epoc time with number of 100ns since 1601-01-01 until 1970-01-01
#define SEC_TO_UNIX_EPOCH 116444736000000000LL

CcDateTime::CcDateTime(uint64 uiFileTimeUs)
{
  setTimestampUs(uiFileTimeUs);
}

CcDateTime CcDateTime::operator+(int64 iAddSeconds) const
{
  return CcDateTime(m_iTimeStampUs + iAddSeconds);
}

CcDateTime CcDateTime::operator+(uint64 iAddSeconds) const
{
  return CcDateTime(m_iTimeStampUs + iAddSeconds);
}

CcDateTime CcDateTime::operator+(const CcDateTime& oDateTimeToAdd) const
{
  return CcDateTime(m_iTimeStampUs + oDateTimeToAdd.m_iTimeStampUs);
}


CcDateTime CcDateTime::operator-(int64 iAddSeconds) const
{
  return CcDateTime(m_iTimeStampUs + iAddSeconds);
}

CcDateTime CcDateTime::operator-(uint64 iAddSeconds) const
{
  return CcDateTime(m_iTimeStampUs - iAddSeconds);
}

CcDateTime CcDateTime::operator-(const CcDateTime& oDateTimeToAdd) const
{
  return CcDateTime(m_iTimeStampUs - oDateTimeToAdd.m_iTimeStampUs);
}

void CcDateTime::setFiletime(uint64 uiFileTime)
{
  m_iTimeStampUs = (uiFileTime - SEC_TO_UNIX_EPOCH)/10;
}

uint64 CcDateTime::getFiletime() const
{
  return ((m_iTimeStampUs * 10) + SEC_TO_UNIX_EPOCH);
}

int32 CcDateTime::getYear() const
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
      return (uint16) uiYearsFirstComplex + 1970;
    }
    else
    {
      return (uint16) uiYearsFirstComplex + 1969;
    }
  }
  else
    return uiYearsSimple + 1970;
}

uint8 CcDateTime::getMonth() const
{
  uint8 uiMonth;
  uint8 uiDay;
  getMonthDay(uiMonth, uiDay);
  return uiMonth;
}

uint8 CcDateTime::getDay() const
{
  uint8 uiMonth;
  uint8 uiDay;
  getMonthDay(uiMonth, uiDay);
  return uiDay;
}

int8 CcDateTime::getHour() const
{
  return static_cast<int8>(((getTimestampS() % (3600 * 24)) / 3600));
}

int8 CcDateTime::getMinute() const
{
  return static_cast<int8>(((getTimestampS() % 3600) / 60));
}

int8 CcDateTime::getSecond() const
{
  return static_cast<int8>((getTimestampS() % 60));
}

int16 CcDateTime::getMSecond() const
{
  return static_cast<int16>((getTimestampMs() % 1000));
}

int16 CcDateTime::getUSecond() const
{
  return (uint16) (getTimestampUs() % 1000);
}

int32 CcDateTime::getDaysSinceEpoch() const
{
  return (uint32) (getTimestampS() / (3600 * 24)) + 1;
}

uint16 CcDateTime::getDaysSinceYear() const
{
  uint32 uiYears = getYear() - 1970;
  uint32 uiSwitchYears = 0;
  if (uiYears > 1)
    uiSwitchYears = ((uiYears - 2) / 4) + 1;
  uint16 uiRet = (uint16)( getDaysSinceEpoch() - (uiYears * 365) - uiSwitchYears);
  return uiRet;
}

void CcDateTime::getMonthDay(uint8& uiMonth, uint8& uiDay) const
{
  uint16 uiDaysYear = getDaysSinceYear();
  uint16 uiSwitchYear = (getYear() % 4 == 0) ? 1 : 0;
  if (uiDaysYear < 32)
  {
    uiMonth = 1;
    uiDay = (uint8) (uiDaysYear);
  }
  else if (uiDaysYear < 60 + uiSwitchYear)
  {
    uiMonth = 2;
    uiDay = (uint8) (uiDaysYear - 31);
  }
  else if (uiDaysYear < 91 + uiSwitchYear)
  {
    uiMonth = 3;
    uiDay = (uint8) (uiDaysYear - (59 + uiSwitchYear));
  }
  else if (uiDaysYear < 121 + uiSwitchYear)
  {
    uiMonth = 4;
    uiDay = (uint8) (uiDaysYear - (90 + uiSwitchYear));
  }
  else if (uiDaysYear < 152 + uiSwitchYear)
  {
    uiMonth = 5;
    uiDay = (uint8) (uiDaysYear - (120 + uiSwitchYear));
  }
  else if (uiDaysYear < 182 + uiSwitchYear)
  {
    uiMonth = 6;
    uiDay = (uint8) (uiDaysYear - (151 + uiSwitchYear));
  }
  else if (uiDaysYear < 213 + uiSwitchYear)
  {
    uiMonth = 7;
    uiDay = (uint8) (uiDaysYear - (181 + uiSwitchYear));
  }
  else if (uiDaysYear < 244 + uiSwitchYear)
  {
    uiMonth = 8;
    uiDay = (uint8) (uiDaysYear - (212 + uiSwitchYear));
  }
  else if (uiDaysYear < 274 + uiSwitchYear)
  {
    uiMonth = 9;
    uiDay = (uint8) (uiDaysYear - (243 + uiSwitchYear));
  }
  else if (uiDaysYear < 305 + uiSwitchYear)
  {
    uiMonth = 10;
    uiDay = (uint8) (uiDaysYear - (273 + uiSwitchYear));
  }
  else if (uiDaysYear < 335 + uiSwitchYear)
  {
    uiMonth = 11;
    uiDay = (uint8) (uiDaysYear - (304 + uiSwitchYear));
  }
  else if (uiDaysYear < 366 + uiSwitchYear)
  {
    uiMonth = 12;
    uiDay = (uint8) (uiDaysYear - (334 + uiSwitchYear));
  }
  else
  {
    uiMonth = 0;
    uiDay = 0;
  }
}

void CcDateTime::setTimestampS(int64 uiTimeStamp)
{
  m_iTimeStampUs = uiTimeStamp * 1000000;
}

void CcDateTime::setTimestampMs(int64 uiTimeStamp)
{
  m_iTimeStampUs = uiTimeStamp * 1000;
}

void CcDateTime::setTimestampUs(int64 uiTimeStamp)
{
  m_iTimeStampUs = uiTimeStamp;
}

int64 CcDateTime::getTimestampS() const
{
  return m_iTimeStampUs / 1000000;
}

int64 CcDateTime::getTimestampMs() const
{
  return m_iTimeStampUs / 1000;
}

int64 CcDateTime::getTimestampUs() const
{
  return m_iTimeStampUs;
}

int64 CcDateTime::getTimestampNs() const
{
  return m_iTimeStampUs * 1000;
}

CcDateTime& CcDateTime::add(const CcDateTime &oDateTimeToAdd)
{
  m_iTimeStampUs += oDateTimeToAdd.m_iTimeStampUs; 
  return *this;
}

CcDateTime& CcDateTime::addHours(int64 iHours)
{
  m_iTimeStampUs += iHours * 60 * 60 * 1000 * 1000;
  return *this;
}

CcDateTime& CcDateTime::addMinutes(int64 iHours)
{
  m_iTimeStampUs += iHours * 60 * 1000 * 1000;
  return *this;
}

CcDateTime& CcDateTime::addDays(int64 iDays)
{
  m_iTimeStampUs += iDays * 24 * 60 * 60 * 1000 * 1000;
  return *this;
}

CcDateTime& CcDateTime::subSeconds(int64 iTimeStamp)
{
  m_iTimeStampUs -= 1000000 * iTimeStamp;
  return *this;
}

CcDateTime& CcDateTime::subMSeconds(int64 iTimeStamp)
{
  m_iTimeStampUs -= 1000 * iTimeStamp;
  return *this;
}

CcDateTime& CcDateTime::subUSeconds(int64 iTimeStamp)
{
  m_iTimeStampUs -= iTimeStamp;
  return *this;
}

CcDateTime& CcDateTime::sub(CcDateTime oDateTimeToAdd)
{
  m_iTimeStampUs -= oDateTimeToAdd.m_iTimeStampUs;
  return *this;
}

CcDateTime& CcDateTime::fromDateTime(uint16 uiYear, uint8 uiMonth, uint8 uiDay, uint8 uiHours, uint8 uiMinutes, uint8 uiSeconds, uint16 uiMillisecconds)
{
  uint32 uiYears = uiYear - 1970;
  this->addDays(uiYears * 365);

  uint32 uiSwitchYears = 0;
  if (uiYears > 1)
    uiSwitchYears = ((uiYears - 2) / 4);
  this->addDays(uiSwitchYears);

  if (uiMonth > 2)
  {
    // We had an additional day this year
    this->addDays(1);
  }

  switch (uiMonth)
  {
    case 12:
      this->addDays(30);
      CCFALLTHROUGH;
    case 11:
      this->addDays(31);
      CCFALLTHROUGH;
    case 10:
      this->addDays(30);
      CCFALLTHROUGH;
    case 9:
      this->addDays(31);
      CCFALLTHROUGH;
    case 8:
      this->addDays(31);
      CCFALLTHROUGH;
    case 7:
      this->addDays(30);
      CCFALLTHROUGH;
    case 6:
      this->addDays(31);
      CCFALLTHROUGH;
    case 5:
      this->addDays(30);
      CCFALLTHROUGH;
    case 4:
      this->addDays(31);
      CCFALLTHROUGH;
    case 3:
      this->addDays(28);
      CCFALLTHROUGH;
    case 2:
      this->addDays(31);
      CCFALLTHROUGH;
    default:
      break;
  };
  

  this->addDays(uiDay - 1);
  this->addHours(uiHours);
  this->addMinutes(uiMinutes);
  this->addSeconds(uiSeconds);
  this->addMSeconds(uiMillisecconds);
  return *this;
}

CcStatus CcDateTime::fromIso8601(const CcString& sValue)
{
  CcStatus oStatus;
  // Try it with milliseconds:
  oStatus = fromFormat("yyyy-MM-ddThh:mm:ss.zzzZ", sValue);
  // If not working, try it with seconds
  if (!oStatus)
  {
    oStatus = fromFormat("yyyy-MM-ddThh:mm:ssZ", sValue);
  }
  return oStatus;
}

CcStatus CcDateTime::fromFormat(const CcString& sFormat, const CcString& sValue)
{
  uint16 uiYear = 0;
  uint8 uiMonth = 0;
  uint8 uiDay = 0;
  uint8 uiHours = 0;
  uint8 uiMinutes = 0;
  uint8 uiSeconds=0;
  uint16 uiMillisecconds = 0;
  uint16 uiMicroseconds = 0;

  CcStatus oSuccess;
  size_t uiOffset = 0;
  size_t uiOffsetValue = 0;
  while (uiOffset < sFormat.length() &&
         uiOffsetValue < sValue.length())
  {
    if (sFormat.isStringAtOffset("yyyy", uiOffset))
    {
      uiYear = sValue.substr(uiOffset, 4).toUint16();
      uiOffset += 4;
      uiOffsetValue += 4;
    }
    else if (sFormat.isStringAtOffset("MM", uiOffset))
    {
      uiMonth = sValue.substr(uiOffset, 2).toUint8();
      uiOffset += 2;
      uiOffsetValue += 2;
    }
    else if (sFormat.isStringAtOffset("M", uiOffset))
    {
      if (sValue.size() > uiOffsetValue + 1)
      {
        if (CcStringUtil::isNumber(sValue[uiOffsetValue + 1]))
        {
          uiMonth = sValue.substr(uiOffsetValue, 2).toUint8();
          uiOffsetValue += 2;
        }
        else
        {
          uiMonth = sValue.substr(uiOffsetValue, 1).toUint8();
          uiOffsetValue += 1;
        }
      }
      else
      {
        uiMonth = sValue.substr(uiOffsetValue, 1).toUint8();
        uiOffsetValue += 1;
      }
      uiOffset += 1;
    }
    else if (sFormat.isStringAtOffset("dd", uiOffset))
    {
      uiDay = sValue.substr(uiOffsetValue, 2).toUint8();
      uiOffsetValue += 2;
      uiOffset      += 2;
    }
    else if (sFormat.isStringAtOffset("d", uiOffset))
    {
      if (sValue.size() > uiOffsetValue + 1)
      {
        if (CcStringUtil::isNumber(sValue[uiOffsetValue + 1]))
        {
          uiDay = sValue.substr(uiOffsetValue, 2).toUint8();
          uiOffsetValue += 2;
        }
        else
        {
          uiDay = sValue.substr(uiOffsetValue, 1).toUint8();
          uiOffsetValue += 1;
        }
      }
      else
      {
        uiDay = sValue.substr(uiOffsetValue, 1).toUint8();
        uiOffsetValue += 1;
      }
      uiOffset += 1;
    }
    else if (sFormat.isStringAtOffset("hh", uiOffset))
    {
      uiHours = sValue.substr(uiOffsetValue, 2).toUint8();
      uiOffsetValue += 2;
      uiOffset += 2;
    }
    else if (sFormat.isStringAtOffset("h", uiOffset))
    {
      if (sValue.size() > uiOffsetValue + 1)
      {
        if (CcStringUtil::isNumber(sValue[uiOffsetValue + 1]))
        {
          uiHours = sValue.substr(uiOffsetValue, 2).toUint8();
          uiOffsetValue += 2;
        }
        else
        {
          uiHours = sValue.substr(uiOffsetValue, 1).toUint8();
          uiOffsetValue += 1;
        }
      }
      else
      {
        uiHours = sValue.substr(uiOffsetValue, 1).toUint8();
        uiOffsetValue += 1;
      }
      uiOffset += 1;
    }
    else if (sFormat.isStringAtOffset("mm", uiOffset))
    {
      uiMinutes = sValue.substr(uiOffsetValue, 2).toUint8();
      uiOffsetValue += 2;
      uiOffset += 2;
    }
    else if (sFormat.isStringAtOffset("m", uiOffset))
    {
      if (sValue.size() > uiOffsetValue + 1)
      {
        if (CcStringUtil::isNumber(sValue[uiOffsetValue + 1]))
        {
          uiMinutes = sValue.substr(uiOffsetValue, 2).toUint8();
          uiOffsetValue += 2;
        }
        else
        {
          uiMinutes = sValue.substr(uiOffsetValue, 1).toUint8();
          uiOffsetValue += 1;
        }
      }
      else
      {
        uiMinutes = sValue.substr(uiOffsetValue, 1).toUint8();
        uiOffsetValue += 1;
      }
      uiOffset += 1;
    }
    else if (sFormat.isStringAtOffset("ss", uiOffset))
    {
      uiSeconds = sValue.substr(uiOffsetValue, 2).toUint8();
      uiOffsetValue += 2;
      uiOffset += 2;
    }
    else if (sFormat.isStringAtOffset("s", uiOffset))
    {
      if (sValue.size() > uiOffsetValue + 1)
      {
        if (CcStringUtil::isNumber(sValue[uiOffsetValue + 1]))
        {
          uiSeconds = sValue.substr(uiOffsetValue, 2).toUint8();
          uiOffsetValue += 2;
        }
        else
        {
          uiSeconds = sValue.substr(uiOffsetValue, 1).toUint8();
          uiOffsetValue += 1;
        }
      }
      else
      {
        uiSeconds = sValue.substr(uiOffsetValue, 1).toUint8();
        uiOffsetValue += 1;
      }
      uiOffset += 1;
    }
    else if (sFormat.isStringAtOffset("zzz", uiOffset))
    {
      uiMillisecconds = sValue.substr(uiOffsetValue, 3).toUint16();
      uiOffsetValue += 3;
      uiOffset += 3;
    }
    else if (sFormat.isStringAtOffset("uuu", uiOffset))
    {
      uiMicroseconds = sValue.substr(uiOffsetValue, 3).toUint16();
      uiOffsetValue += 3;
      uiOffset += 3;
    }
    else
    {
      if (sValue[uiOffsetValue] != sFormat[uiOffset])
      {
        oSuccess = false;
      }
      // Ignore this sign
      uiOffsetValue += 1;
      uiOffset += 1;
    }
  }
  fromDateTime(uiYear, uiMonth, uiDay, uiHours, uiMinutes, uiSeconds, uiMillisecconds);
  m_iTimeStampUs += uiMicroseconds; 
  return oSuccess;
}

CcString CcDateTime::getString(const CcString& sFormat) const
{
  uint8 uiMonth;
  uint8 uiDay;
  getMonthDay(uiMonth, uiDay);
  CcString sRet(sFormat);

  // Years
  if (sRet.find("yyyy") != SIZE_MAX)
  {
    CcString sYear = CcString::fromNumber(getYear());
    sYear.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 4);
    sRet = sRet.replace("yyyy", sYear);
  }
  if (sRet.find("yy") != SIZE_MAX)
  {
    CcString sYear = CcString::fromNumber(getYear()%100);
    sYear.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 2);
    sRet = sRet.replace("yy", sYear);
  }

  // Months
  if (sRet.find("MMMM") != SIZE_MAX)
  {
    // @todo: replace with long name for month form locale
    sRet = sRet.replace("MMMM", CcGlobalStrings::DateTime::Months[uiMonth - 1]);
  }
  if (sRet.find("MMM") != SIZE_MAX)
  {
    // @todo: replace with short name for month form locale
    sRet = sRet.replace("MMM", CcGlobalStrings::DateTime::MonthsShort[uiMonth - 1]);
  }
  if (sRet.find("MM") != SIZE_MAX)
  {
    CcString sMonth = CcString::fromNumber(uiMonth);
    sMonth.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 2);
    sRet = sRet.replace("MM", sMonth);
  }
  if (sRet.find("M") != SIZE_MAX)
  {
    CcString sMonth = CcString::fromNumber(uiMonth);
    sRet = sRet.replace("M", sMonth);
  }

  // Days
  if (sRet.find("dddd") != SIZE_MAX)
  {
    // @todo: replace with long name for month form locale
    sRet = sRet.replace("dddd", CcGlobalStrings::DateTime::Days[uiDay - 1]);
  }
  if (sRet.find("ddd") != SIZE_MAX)
  {
    // @todo: replace with short name for month form locale
    sRet = sRet.replace("ddd", CcGlobalStrings::DateTime::DaysShort[uiDay - 1]);
  }
  if (sRet.find("dd") != SIZE_MAX)
  {
    CcString sDay = CcString::fromNumber(uiDay);
    sDay.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 2);
    sRet = sRet.replace("dd", sDay);
  }
  if (sRet.find("d") != SIZE_MAX)
  {
    CcString sDay = CcString::fromNumber(uiDay);
    sRet = sRet.replace("d", sDay);
  }

  // Hours 24h clock
  // @todo H for Hours 12h clock
  if (sRet.find("hh") != SIZE_MAX)
  {
    CcString sHour = CcString::fromNumber(getHour());
    sHour.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 2);
    sRet = sRet.replace("hh", sHour);
  }
  if (sRet.find("h") != SIZE_MAX)
  {
    CcString sHour = CcString::fromNumber(getHour());
    sRet = sRet.replace("h", sHour);
  }

  // Minutes
  if (sRet.find("mm") != SIZE_MAX)
  {
    CcString sMinute = CcString::fromNumber(getMinute());
    sMinute.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 2);
    sRet = sRet.replace("mm", sMinute);
  }
  if (sRet.find("m") != SIZE_MAX)
  {
    CcString sMinute = CcString::fromNumber(getMinute());
    sRet = sRet.replace("m", sMinute);
  }

  // Seconds
  if (sRet.find("ss") != SIZE_MAX)
  {
    CcString sSecond = CcString::fromNumber(getSecond());
    sSecond.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 2);
    sRet = sRet.replace("ss", sSecond);
  }
  if (sRet.find("s") != SIZE_MAX)
  {
    CcString sSecond = CcString::fromNumber(getSecond());
    sRet = sRet.replace("s", sSecond);
  }

  // milliseconds
  if (sRet.find("zzz") != SIZE_MAX)
  {
    CcString sMillisecond = CcString::fromNumber(getMSecond());
    sMillisecond.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 3);
    sRet = sRet.replace("zzz", sMillisecond);
  }

  // milliseconds
  if (sRet.find("uuu") != SIZE_MAX)
  {
    CcString sMicroSecond = CcString::fromNumber(getUSecond());
    sMicroSecond.fillBeginUpToLength(CcGlobalStrings::Numbers::i0, 3);
    sRet = sRet.replace("uuu", sMicroSecond);
  }
  return sRet;
}

CcDateTime CcDateTime::getDayTime()
{
  return CcDateTimeFromUSeconds(m_iTimeStampUs % (24ll * 60 * 60 * 1000 * 1000));
}

CcString CcDateTime::getSqlDateTime(uint8 uiPrecision) const
{
  CcString sRet = getString("yyyy-MM-dd hh:mm:ss.zzzuuu");
  if (uiPrecision == 0)
  {
    sRet = sRet.substr(0, 19);
  }
  else if (uiPrecision < 6 )
  {
    sRet = sRet.substr(0, 20 + uiPrecision);
  }
  return sRet;
}
