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
 * @subpage   CcJsonDocument
 *
 * @page      CcJsonDocument
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcJsonDocument
 **/
#ifndef CcJsonDocument_H_
#define CcJsonDocument_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcJson/CcJsonData.h"

/**
 * @brief Class impelmentation
 */
class CcDocumentsSHARED CcJsonDocument {
public:
  /**
   * @brief Constructor
   */
  CcJsonDocument( void );

  /**
   * @brief Constructor
   */
  CcJsonDocument( const CcString& sDocument );
  
  /**
   * @brief Constructor
   */
  CcJsonDocument(const CcJsonData& oJsonData);

  /**
   * @brief Constructor
   */
  CcJsonDocument(const CcJsonObject& oJsonObject);
  
  /**
   * @brief Constructor
   */
  CcJsonDocument(const CcJsonArray& oJsonArray);

  /**
   * @brief Destructor
   */
  virtual ~CcJsonDocument( void );

  bool parseDocument(const CcString& sDocument);
  CcString& getJsonDocument(bool bCompact = true);

  CcJsonData& getJsonData()
    { return m_oJsonData; }
private:
  size_t findBeginning(const CcString& sDocument);
  size_t findNextEnding(const CcString& sDocument, size_t uiOffset);
  size_t parseMap(CcJsonData& oMap, const CcString& sDocument, size_t uiOffset);
  size_t parseArray(CcJsonData& oArray, const CcString& sDocument, size_t uiOffset);
  size_t parseValue(CcJsonData& oValue, const CcString& sDocument, size_t uiOffset);
  void writeMap(CcString &sOut, const CcJsonData& oItem);
  void writeArray(CcString &sOut, const CcJsonData& oItem);
  void writeValue(CcString &sOut, const CcJsonData& oItem);
  void writeIntends(CcString &sOut) const;
  void writeNewLine(CcString &sOut) const;

private:
  CcString m_sDocument;
  CcJsonData m_oJsonData;
  bool m_bParseError = false;
  CcString m_sParseErrorMsg;
  bool m_bIntend = false;
  uint16 m_uiIntendLevel = 0;

  static const CcString c_sIndent;
};

#endif /* CcJsonDocument_H_ */
