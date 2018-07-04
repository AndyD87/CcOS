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
#ifndef _CcJsonObject_H_
#define _CcJsonObject_H_

#include "CcBase.h"
#include "CcDocument.h"
#include "CcList.h"
#include "CcJson/CcJsonData.h"

/**
 * @brief Class impelmentation
 */
class CcDocumentsSHARED CcJsonObject : public CcList<CcJsonData>
{
public:
  /**
   * @brief Constructor
   */
  CcJsonObject( void );
  
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
  ~CcJsonObject( void );

  inline bool contains(const CcJsonData &oJsonData) const
    { return CcList<CcJsonData>::contains(oJsonData);}

  bool contains(const CcString& sName) const;
  
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

  CcJsonObject& operator=(CcJsonObject&& oToMove);
  CcJsonObject& operator=(const CcJsonObject& oToCopy);
};

#endif /* _CcJsonObject_H_ */
