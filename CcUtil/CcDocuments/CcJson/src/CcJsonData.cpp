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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcJsonData
 */
#include "CcJson/CcJsonData.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"

CcJsonData c_CcJsonNullNode;

CcJsonData::CcJsonData()
{
}

CcJsonData::CcJsonData(const CcJsonObject& oOject, const CcString& sName)
{
  setJsonObject(oOject, sName);
}

CcJsonData::CcJsonData(const CcJsonArray& oOject, const CcString& sName)
{
  setJsonArray(oOject, sName);
}

CcJsonData::CcJsonData(const CcString& sName)
{
  setName(sName);
}

CcJsonData::CcJsonData(const CcString& sName, const CcVariant& vToSet)
{
  setName(sName);
  setValue(vToSet);
}

CcJsonData::~CcJsonData()
{
  deleteCurrent();
}

void CcJsonData::setValue(const CcVariant& vValue)
{
  if (m_eType != EJsonDataType::Value)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Value;
    m_ovValue = new CcVariant(vValue); CCMONITORNEW(m_ovValue);
  }
  else
    *m_ovValue = vValue;
}

void CcJsonData::setJsonObject()
{
  if (m_eType != EJsonDataType::Object)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    m_poJsonObject = new CcJsonObject();
    CCMONITORNEW(m_poJsonObject);
  }
}

void CcJsonData::setJsonObject(const CcJsonObject& oObject, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Object)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    m_poJsonObject = new CcJsonObject(); CCMONITORNEW(m_poJsonObject);
  }
  *m_poJsonObject = oObject;
}

void CcJsonData::setJsonArray()
{
  if (m_eType != EJsonDataType::Array)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    m_poJsonArray = new CcJsonArray(); CCMONITORNEW(m_poJsonArray);
  }
}

void CcJsonData::setJsonArray(const CcJsonArray& oArray, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Array)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    m_poJsonArray = new CcJsonArray(); CCMONITORNEW(m_poJsonArray);
  }
  *m_poJsonArray = oArray;
}

CcJsonData& CcJsonData::operator[](const CcString& sSearchName)
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_poJsonObject->operator[](sSearchName);
  }
  return c_CcJsonNullNode;
}

const CcJsonData& CcJsonData::operator[](const CcString& sSearchName) const
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_poJsonObject->operator[](sSearchName);
  }
  return c_CcJsonNullNode;
}

CcJsonData& CcJsonData::operator=(const CcJsonData& vToCopy)
{
  m_sName = vToCopy.m_sName;
  switch (vToCopy.m_eType)
  {
    case EJsonDataType::Value:
      setValue(*vToCopy.m_ovValue);
      break;
    case EJsonDataType::Array:
      setJsonArray();
      *m_poJsonArray = *vToCopy.m_poJsonArray;
      break;
    case EJsonDataType::Object:
      setJsonObject();
      *m_poJsonObject = *vToCopy.m_poJsonObject;
      break;
  }
  return *this;
}

CcJsonData& CcJsonData::operator=(CcJsonData&& oToMove)
{
  if (this != &oToMove)
  {
    deleteCurrent();
    m_eType = oToMove.m_eType;
    m_ovValue = oToMove.m_ovValue;
    m_sName = std::move(oToMove.m_sName);
    oToMove.m_eType = EJsonDataType::Unknown;
  }
  return *this;
}

bool CcJsonData::operator==(const CcJsonData& oToCompare) const
{
  bool bRet = false;
  if (oToCompare.m_eType == m_eType)
  {
    switch (m_eType)
    {
    case EJsonDataType::Value:
      bRet = *m_ovValue == *oToCompare.m_ovValue;
      break;
    case EJsonDataType::Array:
      bRet = *m_poJsonArray == *oToCompare.m_poJsonArray;
      break;
    case EJsonDataType::Object:
      bRet = *m_poJsonObject == *oToCompare.m_poJsonObject;
      break;
    }
  }
  return bRet;

}

void CcJsonData::deleteCurrent()
{
  switch (m_eType)
  {
    case EJsonDataType::Array:
      m_eType = EJsonDataType::Unknown;
      CCMONITORDELETE(m_poJsonArray); delete m_poJsonArray;
      break;
    case EJsonDataType::Value:
      m_eType = EJsonDataType::Unknown;
      CCMONITORDELETE(m_ovValue); delete m_ovValue;
      break;
    case EJsonDataType::Object:
      m_eType = EJsonDataType::Unknown;
      CCMONITORDELETE(m_poJsonObject); delete m_poJsonObject;
      break;
  }
}
