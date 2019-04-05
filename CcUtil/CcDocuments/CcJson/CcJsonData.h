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
 * @page      CcJson
 * @subpage   CcJsonData
 *
 * @page      CcJsonData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcJsonData
 **/
#ifndef _CcJsonData_H_
#define _CcJsonData_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcVariant.h"
#include "CcString.h"
#include "CcList.h"

//! forward declaration
class CcJsonObject;
class CcJsonArray;
class CcJsonData;

/**
 * @brief CcJsonData contains on specifier JSON data type.
 *        This enum will define all available types and Unknown for
 *        init and error states
 */
enum class EJsonDataType
{
  Unknown = 0,
  Value,
  Object,
  Array
};

extern CcJsonData c_CcJsonNullNode;

/**
 * @brief The CcJsonData class contains an JSON Object, Array or Value.
 *        This class is used like an union for handling with json data types.
 */
class CcDocumentsSHARED CcJsonData
{
public:
  /**
   * @brief Create an empty object
   */
  CcJsonData() = default;

  /**
   * @brief Create an object by coping data from another.
   * @param oToCopy: Object to copy from
   */
  CcJsonData(const CcJsonData& oToCopy)
    {operator=(oToCopy);}

  /**
   * @brief Create an object and move data from another.
   * @param oToCopy: Object to move data from
   */
  CcJsonData(CcJsonData&& oToMove)
    {operator=(std::move(oToMove));}

  /**
   * @brief Create an Object of type CcJsonObject, and copy content from another object.
   *        Set a new name for this object with sName
   * @param oOject: Object to Copy data from
   * @param sName: Name to set for this Object
   */
  CcJsonData(const CcJsonObject& oOject, const CcString& sName)
    {setJsonObject(oOject, sName);}

  /**
   * @brief Create an Object of type CcJsonArray, and copy content from another CcJsonArray.
   *        Set a new name for this object with sName
   * @param oArray: Array to Copy data from
   * @param sName: Name to set for this Object
   */
  CcJsonData(const CcJsonArray& oArray, const CcString& sName)
    {setJsonArray(oArray, sName);}

  /**
   * @brief Create an empty element with name.
   * @param sName: Name to set for empty object.
   */
  CcJsonData(const CcString& sName)
    {setName(sName);}

  /**
   * @brief Create Object of type CcJsonValue, and directly set name and value.
   * @param sName: Name to set for this object.
   * @param oToSet: Value to set
   */
  CcJsonData(const CcString& sName, const CcVariant& vToSet)
    {setName(sName);setValue(vToSet);}
  
  /**
   * @brief Create Object of specified Type.
   * @param EType: 
   */
  CcJsonData(EJsonDataType eType);

  /**
   * @brief Desctructor
   */
  ~CcJsonData()
    {deleteCurrent();}
  
  /**
   * @brief Get editable name of this object.
   * @return Refernce to name
   */
  inline CcString& name()
    {return m_sName;}

  /**
   * @brief Get the editable value stored in this object.
   * @return Refernce to value
   */
  inline CcVariant& value()
    {return *m_uData.m_ovValue;}

  /**
   * @brief Get the editable object stored in this object.
   * @return Refernce to object
   */
  inline CcJsonObject& object()
    {return *m_uData.m_poJsonObject;}

  /**
   * @brief Get the editable array stored in this object.
   * @return Refernce to object
   */
  inline CcJsonArray& array()
    {return *m_uData.m_poJsonArray;}

  /**
   * @brief Get name of this object
   * @return Object name as string.
   */
  inline const CcString& getName() const
    {return m_sName;}

  /**
   * @brief Get stored Value.
   * @return Value as CcVariant
   */
  inline const CcVariant& getValue() const
    {return *m_uData.m_ovValue;}

  /**
   * @brief Get stored JSON Object.
   * @return Object as CcJsonObject
   */
  inline const CcJsonObject& getJsonObject() const
    {return *m_uData.m_poJsonObject;}

  /**
   * @brief Get stored JSON Array.
   * @return Object as CcJsonArray
   */
  inline const CcJsonArray& getJsonArray() const
    {return *m_uData.m_poJsonArray;}

  /**
   * @brief Get Type of stored data
   * @return Type as enum
   */
  inline EJsonDataType getType() const
    {return m_eType;}

  /**
   * @brief Change Name of this object.
   * @param sName: New name to set
   */
  inline void setName(const CcString& sName)
    { m_sName = sName; }

  /**
   * @brief Change current stored type to Value
   * @param vValue: New value to set.
   */
  void setValue(const CcVariant& vValue = EVariantType::NoType);

  /**
   * @brief Set Value by Move operation.
   * @param vValue: New value to set.
   */
  void setValue(CcVariant&& vValue);

  /**
   * @brief Change current stored type to an empty JSON Object
   */
  void setJsonObject();

  /**
   * @brief Change current stored type to an JSON Object and fill
   *        with content.
   * @param oObject: Object to copy data from
   * @param sName: Optionally new name for object
   */
  void setJsonObject(const CcJsonObject& oObject, const CcString& sName = "");

  /**
   * @brief Change current stored type to an empty JSON Array
   */
  void setJsonArray();

  /**
   * @brief Change current stored type to an JSON Array and fill
   *        with content.
   * @param oArray: Object to copy array data from
   * @param sName: Optionally new name for object
   */
  void setJsonArray(const CcJsonArray& oArray, const CcString& sName = "");

  /**
   * @brief Check if type is define for this object
   * @return true if no type is defined
   */
  inline bool isNull() const
    { return m_eType == EJsonDataType::Unknown; }

  /**
   * @brief Check if type is undefined for this object
   * @return true if type is defined
   */
  inline bool isNotNull() const
    { return m_eType != EJsonDataType::Unknown; }

  /**
   * @brief Check current type is set to JSON Value
   * @return true if type is Value
   */
  inline bool isValue() const
    { return m_eType == EJsonDataType::Value; }

  /**
   * @brief Check current type is set to JSON Object
   * @return true if type is Object
   */
  inline bool isObject() const
    { return m_eType == EJsonDataType::Object; }

  /**
   * @brief Check current type is set to JSON Array
   * @return true if type is Array
   */
  inline bool isArray() const
    { return m_eType == EJsonDataType::Array; }

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  CcJsonData& operator[](const CcString& sSearchName);

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonData& operator[](const CcString& sSearchName) const;
  
  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  CcJsonData& operator[](size_t uiIndex);

  /**
   * @brief Search an object by Index.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonData& operator[](size_t uiIndex) const;

  /**
   * @brief Move data from another object to this.
   * @param oToMove: Object to move from
   * @return reference to this
   */
  CcJsonData& operator=(CcJsonData&& oToMove);

  /**
   * @brief Copy data from another object to this.
   * @param oToCopy: Object to copy from
   * @return reference to this
   */
  CcJsonData& operator=(const CcJsonData& oToCopy);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcJsonData& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcJsonData& oToCompare) const
    {return !operator==(oToCompare);}

  /**
   * @brief Reset all stored data and init with unknown state
   */
  inline void reset()
    { deleteCurrent(); }
private:
  /**
   * @brief Remove all current stored data and change to inital state.
   */
  void deleteCurrent();

private:
  union CcDocumentsSHARED UJsonDataType
  {
    void*         m_pVoid = nullptr;
    CcVariant*    m_ovValue;
    CcJsonObject* m_poJsonObject;
    CcJsonArray*  m_poJsonArray;
  };
  UJsonDataType m_uData;
  EJsonDataType m_eType = EJsonDataType::Unknown; //!< Enum of current stored Object type.
  CcString m_sName;                               //!< Name of this Object
};

#ifdef WIN32
template class CcDocumentsSHARED CcList<CcJsonData>;
#endif

#endif /* _CcJsonData_H_ */
