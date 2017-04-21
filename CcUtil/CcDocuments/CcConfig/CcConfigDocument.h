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
 * @page      CcConfig
 * @subpage   CcConfigDocument
 *
 * @page      CcConfigDocument
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConfigDocument
 */
#ifndef CcConfigDocument_H_
#define CcConfigDocument_H_

#include "CcBase.h"
#include "CcDocument.h"
#include "CcMapCommon.h"
#include "CcPair.h"
#include "CcString.h"

typedef CcMap<CcString, CcStringMap> CcSectionList;
template class CcDocumentsSHARED CcMap<CcString, CcStringMap>;

/**
* @brief Html Document Manager, it can parse or generate HTML-Documents.
*/
class CcDocumentsSHARED CcConfigDocument 
{
public:
  /**
  * @brief Construct Class with a Text as Base.
  * @param String: String containing a HTML-Document
  */
  CcConfigDocument(const CcString& String);

  /**
  * @brief Destructor
  */
  virtual ~CcConfigDocument(void);

  /**
   * @brief Parse a Document wich is represented as String.
   * @param String: Document as String.
   */
  void parseDocument(const CcString& String);

private: // Methods
private:
  CcStringMap   m_oGlobals;
  CcSectionList m_oSectionList;
};

#endif /* CcConfigDocument_H_ */
