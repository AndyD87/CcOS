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
 * @page      CcJsonData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcJsonData
 **/
#ifndef CcJsonData_H_
#define CcJsonData_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcVariant.h"
#include "CcString.h"
#include "CcList.h"

class CcJsonObject;
class CcJsonArray;
class CcJsonData;

enum class EJsonDataType
{
  Unknown,
  Value,
  Object,
  Array
};

extern CcJsonData c_CcJsonNullNode;

class CcDocumentsSHARED CcJsonData
{
public:
  CcJsonData();
  CcJsonData(const CcJsonData& oToCopy)
    { operator=(oToCopy); }
  CcJsonData(CcJsonData&& oToMove)
    { operator=(std::move(oToMove)); }

  CcJsonData(const CcJsonObject& oOject, const CcString& sName);
  CcJsonData(const CcJsonArray& oOject, const CcString& sName);

  CcJsonData(const CcString& sName);
  CcJsonData(const CcString& sName, const CcVariant& oToSet);

  virtual ~CcJsonData();
  
  inline CcString& name()
    {return m_sName;}
  inline CcVariant& value() 
    {return *m_ovValue;}
  inline CcJsonObject& object() 
    {return *m_poJsonObject;}
  inline CcJsonArray& array()
    {return *m_poJsonArray; }
  inline EJsonDataType type() const
    {return m_eType;}

  inline const CcString& getName() const
    {return m_sName;}
  inline const CcVariant& getValue() const
    {return *m_ovValue;}
  inline const CcJsonObject& getJsonObject() const
    {return *m_poJsonObject;}
  inline const CcJsonArray& getJsonArray() const
    {return *m_poJsonArray; }
  inline EJsonDataType getType() const
    {return m_eType;}
  
  void setName(const CcString& sName)
    { m_sName = sName; }
  void setValue(const CcVariant& vValue = EVariantType::NoType);
  void setJsonObject();
  void setJsonObject(const CcJsonObject& oObject, const CcString& sName = "");
  void setJsonArray();
  void setJsonArray(const CcJsonArray& oArray, const CcString& sName = "");

  inline bool isNull() const
    { return m_eType == EJsonDataType::Unknown; }
  inline bool isNotNull() const
    { return m_eType != EJsonDataType::Unknown; }
  inline bool isValue() const
    { return m_eType == EJsonDataType::Value; }
  inline bool isObject() const
    { return m_eType == EJsonDataType::Object; }
  inline bool isArray() const
    { return m_eType == EJsonDataType::Array; }

  CcJsonData& operator[](const CcString& sSearchName);
  const CcJsonData& operator[](const CcString& sSearchName) const;
  CcJsonData& operator=(const CcJsonData& oToCopy);
  CcJsonData& operator=(CcJsonData&& oToMove);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcJsonData& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcJsonData& oToCompare) const
    {return !operator==(oToCompare);}
private:
  void deleteCurrent();

private:
  EJsonDataType m_eType = EJsonDataType::Unknown;
  CcString m_sName;
  union
  {
    CcVariant*    m_ovValue = nullptr;
    CcJsonObject* m_poJsonObject;
    CcJsonArray*  m_poJsonArray;
  };
};

#ifdef WIN32
template class CcDocumentsSHARED CcList<CcJsonData>;
#endif

#endif /* CcJsonData_H_ */
