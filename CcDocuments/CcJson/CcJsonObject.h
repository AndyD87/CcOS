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
 * @brief     Class CcJsonObject
 **/
#pragma once

#include "CcBase.h"
#include "CcDocument.h"
#include "CcList.h"
#include "CcJson/CcJsonNode.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcJsonObject : public CcList<CcJsonNode>
{
public:
  /**
   * @brief Constructor
   */
  CcJsonObject() = default;
  ~CcJsonObject() = default;

  /**
   * @brief Copy data from another object to this
   * @param oToCopy: Object to move from
   */
  CcJsonObject(const CcJsonObject& oToCopy) :
    CcList<CcJsonNode>()
  { operator=(oToCopy); }

  /**
   * @brief Move data from another object to this
   * @param oToMove: Object to move from
   */
  CcJsonObject(CcJsonObject&& oToMove)
  { operator=(CCMOVE(oToMove)); }

  /**
   * @brief Check if Node is part of array
   * @param oJsonData: Node to search
   * @return True if node was found
   */
  inline bool contains(const CcJsonNode &oJsonData) const
  { return CcList<CcJsonNode>::contains(oJsonData);}

  /**
   * @brief Check if json node with specific name is available
   * @param sName: Name to search for
   * @param eType: Set specific type to search for or EJsonDataType::Unknown for any type
   * @return True if name was found
   */
  bool contains(const CcString& sName, EJsonDataType eType = EJsonDataType::Unknown) const;
  
  /**
   * @brief Search an object by Name.
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  CcJsonNode& operator[](const CcString& sSearchName);

  /**
   * @brief Search an object by Name.
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonNode& operator[](const CcString& sSearchName) const;

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  CcJsonNode& operator[](size_t uiIndex)
  { return CcList<CcJsonNode>::operator[](uiIndex); }

  /**
   * @brief Search an object by Index.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonNode& operator[](size_t uiIndex) const
  { return CcList<CcJsonNode>::operator[](uiIndex); }

  /**
   * @brief Move data of another object to this
   * @param oToMove: Object to move from
   * @return Handle to this
   */
  CcJsonObject& operator=(CcJsonObject&& oToMove);

  /**
   * @brief Copy data of another object to this
   * @param oToCopy: Object to copy from
   * @return Handle to this
   */
  CcJsonObject& operator=(const CcJsonObject& oToCopy);

  /**
   * @brief Compare this object with another json object
   * @param oToCompare: Reference to object to compare with.
   * @return true if same
   */
  bool operator==(const CcJsonObject& oToCompare) const;

  /**
   * @brief Compare this object with another json object.
   *        It inverses result of operator==.
   * @param oToCompare: Reference to object to compare with.
   * @return true if not same
   */
  inline bool operator!=(const CcJsonObject& oToCompare) const
  { return !operator==(oToCompare); }
};
