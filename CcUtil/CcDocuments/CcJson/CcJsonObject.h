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
 * @subpage   CcJsonObject
 *
 * @page      CcJsonObject
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcJsonObject
 **/
#ifndef H_CcJsonObject_H_
#define H_CcJsonObject_H_

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
  CcJsonObject();
  
  /**
   * @brief CopyConstructor
   */
  CcJsonObject(const CcJsonObject& oToCopy) = default;

  /**
   * @brief CopyConstructor
   */
  CcJsonObject(CcJsonObject&& oToMove)
    {operator=(std::move(oToMove));}

  /**
   * @brief Destructor
   */
  ~CcJsonObject();

  inline bool contains(const CcJsonNode &oJsonData) const
    { return CcList<CcJsonNode>::contains(oJsonData);}

  bool contains(const CcString& sName) const;
  
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

  CcJsonObject& operator=(CcJsonObject&& oToMove);
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

#endif // H_CcJsonObject_H_
