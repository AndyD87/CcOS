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
 * @brief     Implementation of class CcVersion
 */
#include "CcVersion.h"
#include "CcString.h"
#include "CcStringList.h"

CcVersion::CcVersion(const CcString& sVersion, const CcString& sSeperator) :
  m_uiMajor(0),
  m_uiMinor(0),
  m_uiBuild(0),
  m_uiRevision(0)
{
  setVersionString(sVersion, sSeperator);
}

CcVersion::CcVersion(uint16 uiMajor, uint16 uiMinor, uint16 uiBuild, uint16 uiRevision) :
  m_uiMajor(uiMajor),
  m_uiMinor(uiMinor),
  m_uiBuild(uiBuild),
  m_uiRevision(uiRevision)
{

}

bool CcVersion::operator>(const CcVersion& oToCompare) const
{
  bool bRet = false;
  if (m_uiMajor > oToCompare.m_uiMajor)
  {
    bRet = true;
  }
  else if (m_uiMajor == oToCompare.m_uiMajor)
  {
    if (m_uiMinor > oToCompare.m_uiMinor)
    {
      bRet = true;
    }
    else if (m_uiMinor == oToCompare.m_uiMinor)
    {
      if (m_uiBuild > oToCompare.m_uiBuild)
      {
        bRet = true;
      }
      else if (m_uiBuild == oToCompare.m_uiBuild)
      {
        if (m_uiRevision > oToCompare.m_uiRevision)
        {
          bRet = true;
        }
      }
    }
  }
  return bRet;
}

bool CcVersion::operator<(const CcVersion& oToCompare) const
{
  bool bRet = false;
  if (m_uiMajor < oToCompare.m_uiMajor)
  {
    bRet = true;
  }
  else if (m_uiMajor == oToCompare.m_uiMajor)
  {
    if (m_uiMinor < oToCompare.m_uiMinor)
    {
      bRet = true;
    }
    else if (m_uiMinor == oToCompare.m_uiMinor)
    {
      if (m_uiBuild < oToCompare.m_uiBuild)
      {
        bRet = true;
      }
      else if (m_uiBuild == oToCompare.m_uiBuild)
      {
        if (m_uiRevision < oToCompare.m_uiRevision)
        {
          bRet = true;
        }
      }
    }
  }
  return bRet;
}

bool CcVersion::operator==(const CcVersion& oToCompare) const
{
  bool bRet = false;
  if (m_uiMajor == oToCompare.m_uiMajor &&
      m_uiMinor == oToCompare.m_uiMinor &&
      m_uiBuild == oToCompare.m_uiBuild &&
      m_uiRevision == oToCompare.m_uiRevision)
  {
    bRet = true;
  }
  return bRet;
}

bool CcVersion::setVersionString(const CcString& sVersion, const CcString& sSeperator)
{
  bool bRet = false;
  CcStringList slVersionParts(sVersion.split(sSeperator));
  if (slVersionParts.size() > 1)
  {
    uint16 uiVersionCnt = 0;
    for (size_t i = 0; i < slVersionParts.size() && uiVersionCnt < 4; i++)
    {
      bool bConversionOk = false;
      uint16 uiVersionNumber = slVersionParts[i].trim().toUint16(&bConversionOk);
      if (bConversionOk)
      {
        bRet = true;
        setVersionByPosition(uiVersionCnt, uiVersionNumber);
        uiVersionCnt++;
      }
    }
  }
  return bRet;
}

CcString CcVersion::getVersionString(size_t iDepth, const CcString& sSeperator) const
{
  CcString sRet;
  if (iDepth > 0)
  {
    sRet << CcString::fromNumber(m_uiMajor);
    if (iDepth > 1)
    {
      sRet << sSeperator << CcString::fromNumber(m_uiMinor);
      if (iDepth > 2)
      {
        sRet << sSeperator << CcString::fromNumber(m_uiBuild);
        if (iDepth > 3)
        {
          sRet << sSeperator << CcString::fromNumber(m_uiRevision);
        }
      }
    }
  }
  else
  {
    CCDEBUG("Version String requested with length of 0");
  }
  return sRet;
}

void CcVersion::setVersionByPosition(uint16 uiPos, uint16 uiVersionNr)
{
  switch (uiPos)
  {
    case 0:
      m_uiMajor = static_cast<uint16>(uiVersionNr);
      break;
    case 1:
      m_uiMinor = static_cast<uint16>(uiVersionNr);
      break;
    case 2:
      m_uiBuild = uiVersionNr;
      break;
    case 3:
      m_uiRevision = uiVersionNr;
      break;
  }
}