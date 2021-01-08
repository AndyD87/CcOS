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
 * @subpage   CcJsonArray
 *
 * @page      CcJsonArray
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcJsonArray
 **/
#ifndef H_CcJsonArray_H_
#define H_CcJsonArray_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcList.h"
#include "CcJson/CcJsonNode.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcJsonArray : public CcList<CcJsonNode>
{
public:
  /**
   * @brief Constructor
   */
  CcJsonArray();

  /**
   * @brief Destructor
   */
  ~CcJsonArray();
  
  inline bool contains(const CcJsonNode &oJsonData) const
    { return CcList<CcJsonNode>::contains(oJsonData);}
  
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
   * @brief Compare this array with another json array
   * @param oToCompare: Reference to array to compare with.
   * @return true if same
   */
  bool operator==(const CcJsonArray& oToCompare) const;
  
  /**
   * @brief Compare this array with another json array.
   *        It inverses result of operator==.
   * @param oToCompare: Reference to array to compare with.
   * @return true if not same
   */
  inline bool operator!=(const CcJsonArray& oToCompare) const
    { return !operator==(oToCompare);}


  bool contains(const CcString& sName) const;

};

#endif // H_CcJsonArray_H_
