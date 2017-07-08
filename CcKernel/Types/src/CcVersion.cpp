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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of class CcVersion
 */
#include "CcVersion.h"
#include "CcStringList.h"

CcVersion::CcVersion() :
  m_uiMajor(0),
  m_uiMinor(0),
  m_uiBuild(0),
  m_uiRevision(0)
{

}

CcVersion::CcVersion(const CcString& sVersion) :
  m_uiMajor(0),
  m_uiMinor(0),
  m_uiBuild(0),
  m_uiRevision(0)
{
  setVersionString(sVersion);
}

CcVersion::CcVersion(uint8 uiMajor, uint8 uiMinor) :
  m_uiMajor(uiMajor),
  m_uiMinor(uiMinor),
  m_uiBuild(0),
  m_uiRevision(0)
{

}
CcVersion::CcVersion(uint8 uiMajor, uint8 uiMinor, uint16 uiBuild, uint16 uiRevision) :
  m_uiMajor(uiMajor),
  m_uiMinor(uiMinor),
  m_uiBuild(uiBuild),
  m_uiRevision(uiRevision)
{

}

CcVersion::~CcVersion()
{

}


bool CcVersion::setVersionString(const CcString& sVersion)
{
  bool bRet = false;
  CcStringList slVersionParts(sVersion.split("."));
  if (slVersionParts.size() > 1)
  {
    uint8 uiVersionCnt = 0;
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

void CcVersion::setVersionByPosition(uint8 uiPos, uint16 uiVersionNr)
{
  switch (uiPos)
  {
    case 0:
      m_uiMajor = static_cast<uint8>(uiVersionNr);
      break;
    case 1:
      m_uiMinor = static_cast<uint8>(uiVersionNr);
      break;
    case 2:
      m_uiBuild = uiVersionNr;
      break;
    case 3:
      m_uiRevision = uiVersionNr;
      break;
  }
}