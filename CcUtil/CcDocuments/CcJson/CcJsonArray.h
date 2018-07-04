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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcJsonArray
 **/
#ifndef _CcJsonArray_H_
#define _CcJsonArray_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcList.h"
#include "CcJson/CcJsonData.h"

/**
 * @brief Class impelmentation
 */
class CcDocumentsSHARED CcJsonArray : public CcList<CcJsonData>
{
public:
  /**
   * @brief Constructor
   */
  CcJsonArray( void );

  /**
   * @brief Destructor
   */
  ~CcJsonArray( void );
  
  inline bool contains(const CcJsonData &oJsonData) const
    { return CcList<CcJsonData>::contains(oJsonData);}
  
  /**
   * @brief Search an object by Name.
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  CcJsonData& operator[](const CcString& sSearchName);

  /**
   * @brief Search an object by Name.
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
  CcJsonData& operator[](size_t uiIndex)
    { return CcList<CcJsonData>::operator[](uiIndex); }

  /**
   * @brief Search an object by Index.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonData& operator[](size_t uiIndex) const
    { return CcList<CcJsonData>::operator[](uiIndex); }
  bool contains(const CcString& sName) const;
};

#endif /* _CcJsonArray_H_ */
