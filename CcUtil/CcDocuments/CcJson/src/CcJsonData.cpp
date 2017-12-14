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
 * @brief     Implemtation of class CcJsonData
 */
#include "CcJson/CcJsonData.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcSharedPointer.h"

CcJsonData c_CcJsonNullNode;

void CcJsonData::setValue(const CcVariant& vValue)
{
  if (m_eType != EJsonDataType::Value)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Value;
    m_uData.m_ovValue = new CcVariant(vValue);
    CCMONITORNEW(m_uData.m_ovValue.getPtr());
  }
  else
  {
    *m_uData.m_ovValue = vValue;
  }
}

void CcJsonData::setValue(CcVariant&& vValue)
{
  if (m_eType != EJsonDataType::Value)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Value;
    m_uData.m_ovValue = new CcVariant(std::move(vValue));
    CCMONITORNEW(m_uData.m_ovValue.getPtr());
  }
  else
  {
    *m_uData.m_ovValue = vValue;
  }
}

void CcJsonData::setJsonObject()
{
  if (m_eType != EJsonDataType::Object)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    m_uData.m_poJsonObject = new CcJsonObject();
    CCMONITORNEW(m_uData.m_poJsonObject.getPtr());
  }
}

void CcJsonData::setJsonObject(const CcJsonObject& oObject, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Object)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    m_uData.m_poJsonObject = new CcJsonObject();
    CCMONITORNEW(m_uData.m_poJsonObject.getPtr());
  }
  *m_uData.m_poJsonObject = oObject;
}

void CcJsonData::setJsonArray()
{
  if (m_eType != EJsonDataType::Array)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    m_uData.m_poJsonArray = new CcJsonArray();
    CCMONITORNEW(m_uData.m_poJsonArray.getPtr());
  }
}

void CcJsonData::setJsonArray(const CcJsonArray& oArray, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Array)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    m_uData.m_poJsonArray = new CcJsonArray();
    CCMONITORNEW(m_uData.m_poJsonArray.getPtr());
  }
  *m_uData.m_poJsonArray = oArray;
}

CcJsonData& CcJsonData::operator[](const CcString& sSearchName)
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_uData.m_poJsonObject->operator[](sSearchName);
    case EJsonDataType::Array:
    case EJsonDataType::Value:
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return c_CcJsonNullNode;
}

const CcJsonData& CcJsonData::operator[](const CcString& sSearchName) const
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_uData.m_poJsonObject->operator[](sSearchName);
    case EJsonDataType::Array:
    case EJsonDataType::Value:
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return c_CcJsonNullNode;
}

CcJsonData& CcJsonData::operator=(CcJsonData&& oToMove)
{
  if (this != &oToMove)
  {
    deleteCurrent();
    m_eType = oToMove.m_eType;
    m_uData.m_pVoid = oToMove.m_uData.m_pVoid;
    m_sName = std::move(oToMove.m_sName);
    // reset data from Moved
    oToMove.m_uData.m_pVoid = nullptr;
    oToMove.m_eType = EJsonDataType::Unknown;
  }
  return *this;
}

CcJsonData& CcJsonData::operator=(const CcJsonData& oToCopy)
{
  if (this != &oToCopy)
  {
    switch (oToCopy.m_eType)
    {
      case EJsonDataType::Value:
        setValue(*oToCopy.m_uData.m_ovValue);
        break;
      case EJsonDataType::Array:
        setJsonArray(*oToCopy.m_uData.m_poJsonArray);
        break;
      case EJsonDataType::Object:
        setJsonObject(*oToCopy.m_uData.m_poJsonObject);
        break;
      case EJsonDataType::Unknown:
      default:
        break;
    }
    m_eType = oToCopy.m_eType;
    m_sName = oToCopy.m_sName;
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
        bRet = *m_uData.m_ovValue == *oToCompare.m_uData.m_ovValue;
        break;
      case EJsonDataType::Array:
        bRet = *m_uData.m_poJsonArray == *oToCompare.m_uData.m_poJsonArray;
        break;
      case EJsonDataType::Object:
        bRet = *m_uData.m_poJsonObject == *oToCompare.m_uData.m_poJsonObject;
        break;
      case EJsonDataType::Unknown:
      default:
        break;
    }
  }
  return bRet;
}

void CcJsonData::deleteCurrent()
{
  
  if (m_uData.m_pVoid != nullptr)
  {
    switch (m_eType)
    {
    case EJsonDataType::Value:
      delete m_uData.m_ovValue;
      break;
    case EJsonDataType::Array:
      delete m_uData.m_poJsonArray;
      break;
    case EJsonDataType::Object:
      delete m_uData.m_poJsonObject;
      break;
    case EJsonDataType::Unknown:
    default:
      break;
    }
  }
}
