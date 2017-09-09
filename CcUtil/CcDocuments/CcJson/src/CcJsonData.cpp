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
 * @brief     Implemtation of class CcJsonData
 */
#include "CcJson/CcJsonData.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcSharedPointer.h"

CcJsonData c_CcJsonNullNode;
class CcJsonDataPrivate
{
public:
  CcJsonDataPrivate() {}
  ~CcJsonDataPrivate() {}
  union
  {
    CcSharedPointer<CcVariant>    m_ovValue = nullptr;
    CcSharedPointer<CcJsonObject> m_poJsonObject;
    CcSharedPointer<CcJsonArray>  m_poJsonArray;
  };
};

CcJsonData::CcJsonData()
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcJsonData::CcJsonData(const CcJsonData& oToCopy)
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
  operator=(oToCopy);
}

CcJsonData::CcJsonData(CcJsonData&& oToMove)
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
  operator=(std::move(oToMove));
}

CcJsonData::CcJsonData(const CcJsonObject& oOject, const CcString& sName)
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
  setJsonObject(oOject, sName);
}

CcJsonData::CcJsonData(const CcJsonArray& oOject, const CcString& sName)
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
  setJsonArray(oOject, sName);
}

CcJsonData::CcJsonData(const CcString& sName)
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
  setName(sName);
}

CcJsonData::CcJsonData(const CcString& sName, const CcVariant& vToSet)
{
  m_pPrivate = new CcJsonDataPrivate();
  CCMONITORNEW(m_pPrivate);
  setName(sName);
  setValue(vToSet);
}

CcJsonData::~CcJsonData()
{
  if (m_pPrivate != nullptr)
  {
    deleteCurrent();
    CCMONITORDELETE(m_pPrivate);
    delete m_pPrivate;
    m_pPrivate = nullptr;
  }
}

CcVariant& CcJsonData::value()
{
  return *m_pPrivate->m_ovValue;
}
CcJsonObject& CcJsonData::object()
{
  return *m_pPrivate->m_poJsonObject;
}
CcJsonArray& CcJsonData::array()
{
  return *m_pPrivate->m_poJsonArray;
}
EJsonDataType CcJsonData::type() const
{
  return m_eType;
}

const CcString& CcJsonData::getName() const
{
  return m_sName;
}
const CcVariant& CcJsonData::getValue() const
{
  return *m_pPrivate->m_ovValue;
}
const CcJsonObject& CcJsonData::getJsonObject() const
{
  return *m_pPrivate->m_poJsonObject;
}
const CcJsonArray& CcJsonData::getJsonArray() const
{
  return *m_pPrivate->m_poJsonArray;
}

void CcJsonData::setValue(const CcVariant& vValue)
{
  if (m_eType != EJsonDataType::Value)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Value;
    m_pPrivate->m_ovValue = new CcVariant(vValue);
    CCMONITORNEW(m_pPrivate->m_ovValue.getPtr());
  }
  else
  {
    *m_pPrivate->m_ovValue = vValue;
  }
}

void CcJsonData::setJsonObject()
{
  if (m_eType != EJsonDataType::Object)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    m_pPrivate->m_poJsonObject = new CcJsonObject();
    CCMONITORNEW(m_pPrivate->m_poJsonObject.getPtr());
  }
}

void CcJsonData::setJsonObject(const CcJsonObject& oObject, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Object)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    m_pPrivate->m_poJsonObject = new CcJsonObject();
    CCMONITORNEW(m_pPrivate->m_poJsonObject.getPtr());
  }
  *m_pPrivate->m_poJsonObject = oObject;
}

void CcJsonData::setJsonArray()
{
  if (m_eType != EJsonDataType::Array)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    m_pPrivate->m_poJsonArray = new CcJsonArray();
    CCMONITORNEW(m_pPrivate->m_poJsonArray.getPtr());
  }
}

void CcJsonData::setJsonArray(const CcJsonArray& oArray, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Array)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    m_pPrivate->m_poJsonArray = new CcJsonArray();
    CCMONITORNEW(m_pPrivate->m_poJsonArray.getPtr());
  }
  *m_pPrivate->m_poJsonArray = oArray;
}

CcJsonData& CcJsonData::operator[](const CcString& sSearchName)
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_pPrivate->m_poJsonObject->operator[](sSearchName);
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
      return m_pPrivate->m_poJsonObject->operator[](sSearchName);
    case EJsonDataType::Array:
    case EJsonDataType::Value:
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return c_CcJsonNullNode;
}

CcJsonData& CcJsonData::operator=(const CcJsonData& vToCopy)
{
  m_sName = vToCopy.m_sName;
  switch (vToCopy.m_eType)
  {
    case EJsonDataType::Value:
      setValue(*vToCopy.m_pPrivate->m_ovValue);
      break;
    case EJsonDataType::Array:
      setJsonArray(*vToCopy.m_pPrivate->m_poJsonArray, vToCopy.getName());
      break;
    case EJsonDataType::Object:
      setJsonObject(*vToCopy.m_pPrivate->m_poJsonObject, vToCopy.getName());
      break;
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return *this;
}

CcJsonData& CcJsonData::operator=(CcJsonData&& oToMove)
{
  if (this != &oToMove)
  {
    deleteCurrent();
    delete m_pPrivate;
    CCMONITORDELETE(m_pPrivate);
    m_eType = oToMove.m_eType;
    m_pPrivate = oToMove.m_pPrivate;
    oToMove.m_pPrivate = nullptr;
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
        bRet = *m_pPrivate->m_ovValue == *oToCompare.m_pPrivate->m_ovValue;
        break;
      case EJsonDataType::Array:
        bRet = *m_pPrivate->m_poJsonArray == *oToCompare.m_pPrivate->m_poJsonArray;
        break;
      case EJsonDataType::Object:
        bRet = *m_pPrivate->m_poJsonObject == *oToCompare.m_pPrivate->m_poJsonObject;
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
  switch (m_eType)
  {
    case EJsonDataType::Array:
      m_eType = EJsonDataType::Unknown;
      m_pPrivate->m_poJsonArray.deleteCurrent();
      break;
    case EJsonDataType::Value:
      m_eType = EJsonDataType::Unknown;
      m_pPrivate->m_ovValue.deleteCurrent();
      break;
    case EJsonDataType::Object:
      m_eType = EJsonDataType::Unknown;
      m_pPrivate->m_poJsonObject.deleteCurrent();
      break;
    case EJsonDataType::Unknown:
    default:
      break;
  }
}
