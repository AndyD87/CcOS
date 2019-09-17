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
 * @brief     Implemtation of class CcJsonNode
 */
#include "CcJson/CcJsonNode.h"
#include "CcJson/CcJsonObject.h"
#include "CcJson/CcJsonArray.h"
#include "CcSharedPointer.h"

CcJsonNode c_CcJsonNullNode(EJsonDataType::Unknown);


CcJsonNode::CcJsonNode(EJsonDataType eType) :
  m_eType(eType)
{
  m_uData.m_pVoid = nullptr;
  switch (eType)
  {
    case EJsonDataType::Object:
      setJsonObject();
      break;
    case EJsonDataType::Array:
      setJsonArray();
      break;
    case EJsonDataType::Value:
      setValue();
      break;
    case EJsonDataType::Unknown:
    default:
      break;
  }
}

CcVariant& CcJsonNode::setValue(const CcVariant& vValue)
{
  if (m_eType != EJsonDataType::Value ||
      m_uData.m_ovValue == nullptr)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Value;
    CCNEW(m_uData.m_ovValue, CcVariant, vValue);
  }
  else
  {
    *m_uData.m_ovValue = vValue;
  }
  return *m_uData.m_ovValue;
}

CcVariant& CcJsonNode::setValue(CcVariant&& vValue)
{
  if (m_eType != EJsonDataType::Value ||
      m_uData.m_ovValue == nullptr)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Value;
    CCNEW(m_uData.m_ovValue, CcVariant, std::move(vValue));
  }
  else
  {
    *m_uData.m_ovValue = vValue;
  }
  return *m_uData.m_ovValue;
}

CcJsonObject& CcJsonNode::setJsonObject()
{
  if (m_eType != EJsonDataType::Object ||
      m_uData.m_poJsonObject == nullptr)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    CCNEW(m_uData.m_poJsonObject, CcJsonObject);
  }
  return *m_uData.m_poJsonObject;
}

CcJsonObject& CcJsonNode::setJsonObject(const CcJsonObject& oObject, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Object ||
      m_uData.m_poJsonObject == nullptr)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Object;
    CCNEW(m_uData.m_poJsonObject, CcJsonObject);
  }
  *m_uData.m_poJsonObject = oObject;
  return *m_uData.m_poJsonObject;
}

CcJsonArray& CcJsonNode::setJsonArray()
{
  if (m_eType != EJsonDataType::Array ||
      m_uData.m_poJsonArray == nullptr)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    CCNEW(m_uData.m_poJsonArray, CcJsonArray);
  }
  return *m_uData.m_poJsonArray;
}

CcJsonArray& CcJsonNode::setJsonArray(const CcJsonArray& oArray, const CcString& sName)
{
  m_sName = sName;
  if (m_eType != EJsonDataType::Array ||
      m_uData.m_poJsonArray == nullptr)
  {
    deleteCurrent();
    m_eType = EJsonDataType::Array;
    CCNEW(m_uData.m_poJsonArray, CcJsonArray);
  }
  *m_uData.m_poJsonArray = oArray;
  return *m_uData.m_poJsonArray;
}

CcJsonNode& CcJsonNode::operator[](const CcString& sSearchName)
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

const CcJsonNode& CcJsonNode::operator[](const CcString& sSearchName) const
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

CcJsonNode& CcJsonNode::operator[](size_t uiIndex)
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_uData.m_poJsonObject->operator[](uiIndex);
    case EJsonDataType::Array:
      return m_uData.m_poJsonArray->operator[](uiIndex);
    case EJsonDataType::Value:
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return c_CcJsonNullNode;
}

const CcJsonNode& CcJsonNode::operator[](size_t uiIndex) const
{
  switch (m_eType)
  {
    case EJsonDataType::Object:
      return m_uData.m_poJsonObject->operator[](uiIndex);
    case EJsonDataType::Array:
      return m_uData.m_poJsonArray->operator[](uiIndex);
    case EJsonDataType::Value:
    case EJsonDataType::Unknown:
    default:
      break;
  }
  return c_CcJsonNullNode;
}

CcJsonNode& CcJsonNode::operator=(CcJsonNode&& oToMove)
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

CcJsonNode& CcJsonNode::operator=(const CcJsonNode& oToCopy)
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

bool CcJsonNode::operator==(const CcJsonNode& oToCompare) const
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

void CcJsonNode::deleteCurrent()
{
  switch (m_eType)
  {
  case EJsonDataType::Value:
    CCDELETE( m_uData.m_ovValue);
    break;
  case EJsonDataType::Array:
    CCDELETE(m_uData.m_poJsonArray);
    break;
  case EJsonDataType::Object:
    CCDELETE(m_uData.m_poJsonObject);
    break;
  case EJsonDataType::Unknown:
  default:
    break;
  }
  m_eType = EJsonDataType::Unknown;
}
