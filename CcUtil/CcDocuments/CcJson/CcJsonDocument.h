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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcJsonDocument
 **/
#ifndef _CcJsonDocument_H_
#define _CcJsonDocument_H_

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
  CcJsonDocument();

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
  inline CcJsonData& operator[](const CcString& sSearchName)
    { return m_oJsonData[sSearchName]; }

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param sSearchName: Name of object to search for
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonData& operator[](const CcString& sSearchName) const
    { return m_oJsonData[sSearchName]; }
  
  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  inline CcJsonData& operator[](size_t uiIndex)
    { return m_oJsonData[uiIndex]; }

  /**
   * @brief Search an object by Name.
   *        This will only work if Object is an Array or Object
   * @param uiIndex: Number of Object in List
   * @return Found Object, or a null valued JsonObject
   */
  const CcJsonData& operator[](size_t uiIndex) const
    { return m_oJsonData[uiIndex]; }


  bool parseDocument(const CcString& sDocument);
  CcString& getDocument(bool bCompact = true);
  CcJsonData& getJsonData()
    { return m_oJsonData; }
  static bool isValidData(const CcString& sData);

private:
  size_t findBeginning(const CcString& sDocument);
  size_t findNextEnding(const char* sDocument, size_t uiLength);
  bool parseMap(CcJsonData& oMap, const char*& sDocument, size_t& uiLength);
  bool parseArray(CcJsonData& oArray, const char*& sDocument, size_t& uiLength);
  bool parseValue(CcJsonData& oValue, const char*& sDocument, size_t& uiLength);
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

#endif /* _CcJsonDocument_H_ */
