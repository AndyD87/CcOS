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
 * @page      CcJsonObject
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcJsonObject
 **/
#ifndef CcJsonObject_H_
#define CcJsonObject_H_

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
  CcJsonObject(const CcJsonObject&& oToMove)
    {operator=(std::move(oToMove));}

  /**
   * @brief Destructor
   */
  virtual ~CcJsonObject( void );

  inline bool contains(const CcJsonData &oJsonData) const
    { return CcList<CcJsonData>::contains(oJsonData);}

  bool contains(const CcString& sName) const;

  CcJsonData& operator[](const CcString& sName);
  const CcJsonData& operator[](const CcString& sName) const;

  CcJsonObject& operator=(CcJsonObject&& oToMove);
  CcJsonObject& operator=(const CcJsonObject& oToCopy);
};

#endif /* CcJsonObject_H_ */
