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
 * @brief     Class CcJsonDocument
 **/
#pragma once

#include "CcDocument.h"
#include "CcBase.h"
#include "Json/CcJsonNode.h"
#include "IIo.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcJsonDocument {
public:
  /**
   * @brief Constructor
   */
  CcJsonDocument();

  /**
   * @brief Constructor
   */
  CcJsonDocument( const CcString& sDocument );
  
  /**
   * @brief Constructor
   */
  CcJsonDocument(const CcJsonNode& oJsonData);

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
  ~CcJsonDocument();

  /**
   * @brief Compare stored Json Data, not Document, with other CcJsonDocument.
   * @param oToCompare: Reference to Document to compare with.
   * @return true if same
   */
  bool operator==(const CcJsonDocument& oToCompare) const;
  
  /**
   * @brief Compare stored Json Data, not Document, with other CcJsonDocument.
   *        It inverses result of operator==.
   * @param oToCompare: Reference to Document to compare with.
   * @return true if not same
   */
  inline bool operator!=(const CcJsonDocument& oToCompare) const
    { return !operator==(oToCompare);}

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  inline CcJsonNode& operator[](const CcString& sSearchName)
    { return m_oJsonData[sSearchName]; }

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonNode& operator[](const CcString& sSearchName) const
    { return m_oJsonData[sSearchName]; }
  
  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  inline CcJsonNode& operator[](size_t uiIndex)
    { return m_oJsonData[uiIndex]; }

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonNode& operator[](size_t uiIndex) const
    { return m_oJsonData[uiIndex]; }

  /**
   * @brief Parse string and create Json node
   * @param sDocument: String to parse
   * @return True if string was parsed successfully
   */
  bool parseDocument(const CcString& sDocument);

  /**
   * @brief Generate Json formated string from root node.
   * @param bCompact: If true no intendation will be used and
   *                  String will be as compact as possible
   * @return Generated string
   */
  CcString getDocument(bool bCompact = true);

  /**
   * @brief Generate json string from root node and write to stream
   * @param[out] rOutput: Target output stream
   * @param bCompact: If true no intendation will be used and
   *                  String will be as compact as possible
   */
  void writeDocument(IIo& rOutput, bool bCompact = true);

  //! @return Get root node from document
  CcJsonNode& getJsonData()
  { return m_oJsonData; }
  //! @copydoc CcJsonDocument::getJsonData()
  CcJsonNode& getJsonNode()
  { return m_oJsonData; }

  /**
   * @brief Verify if String is a valid formated json object or array
   * @param sData: String to verify
   * @return True if Json data was found and valid parsed
   */
  static bool isValidData(const CcString& sData);

private:
  size_t findBeginning(const CcString& sDocument);
  size_t findNextEnding(const char* sDocument, size_t uiLength);
  bool parseMap(CcJsonNode& oMap, const char*& sDocument, size_t& uiLength);
  bool parseArray(CcJsonNode& oArray, const char*& sDocument, size_t& uiLength);
  bool parseValue(CcJsonNode& oValue, const char*& sDocument, size_t& uiLength);
  void writeColon(IIo& rOutput);
  void writeMap(IIo& rOutput, const CcJsonNode& oItem);
  void writeArray(IIo& rOutput, const CcJsonNode& oItem);
  void writeValue(IIo& rOutput, const CcJsonNode& oItem);
  void writeIntends(IIo& rOutput) const;
  void writeNewLine(IIo& rOutput) const;

private:
  CcJsonNode m_oJsonData;
  bool m_bParseError = false;
  CcString m_sParseErrorMsg;
  bool m_bIntend = false;
  uint16 m_uiIntendLevel = 0;

  static const CcString c_sIndent;
};
