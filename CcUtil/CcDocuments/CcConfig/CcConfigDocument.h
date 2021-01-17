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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcConfigDocument
 */
#ifndef H_CcConfigDocument_H_
#define H_CcConfigDocument_H_

#include "CcBase.h"
#include "CcDocument.h"
#include "CcStringList.h"
#include "CcMapCommon.h"
#include "CcPair.h"
#include "CcString.h"

/**
 * @brief Storage class for Sections
 */
class CcSectionList
{
public:
  CcStringList        oNames;   //!< Name of section within []
  CcList<CcStringMap> oSections;//!< Values of section
};

#ifdef _MSC_VER
template class CcDocumentsSHARED CcList<CcStringMap>;
#endif

/**
 * @brief Config Document Manager, it can parse or generate config documents, like ini-files.
 */
class CcDocumentsSHARED CcConfigDocument 
{
public:
  /**
   * @brief Construct Class with a Text as Base.
   * @param String: String containing a config document
   */
  CcConfigDocument(const CcString& String);

  /**
   * @brief Destructor
   */
  ~CcConfigDocument();

  /**
   * @brief Parse a Document wich is represented as String.
   * @param String: Document as String.
   */
  void parseDocument(const CcString& String);

private: // Methods
  CcStringMap   m_oGlobals;       //!< Global values without sections
  CcSectionList m_oSectionList;   //!< Section base variables
};

#endif // H_CcConfigDocument_H_
