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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlFile
 **/
#pragma once

#include "CcBase.h"
#include "CcXml/CcXml.h"
#include "CcXml/CcXmlDocument.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcXmlFile 
{
public:
  /**
   * @brief Constructor
   */
  CcXmlFile();

  /**
   * @brief CopyConstructor
   */
  CcXmlFile( const CcXmlFile& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcXmlFile( CcXmlFile&& oToMove );
  
  /**
   * @brief Constructor
   */
  CcXmlFile( const CcString& sFilePath );

  /**
   * @brief Destructor
   */
  ~CcXmlFile();

  /**
   * @brief Move content from other file to this
   * @param oToMove: XML-File to move data from
   * @return Handle to this
   */
  CcXmlFile& operator=(CcXmlFile&& oToMove);

  /**
   * @brief Copy content from other file to this
   * @param oToCopy: XML-File to copy data from
   * @return Handle to this
   */
  CcXmlFile& operator=(const CcXmlFile& oToCopy);

  /**
   * @brief Compare this file content with another file
   * @param oToCompare: File to compare to
   * @return True if bot contents are same
   */
  bool operator==(const CcXmlFile& oToCompare) const;

  /**
   * @brief Compare this file content with another file
   * @param oToCompare: File to compare to
   * @return True if bot contents are same
   */
  bool operator!=(const CcXmlFile& oToCompare) const;

  //! @return Get path to file
  inline const CcString& getFile() const
  { return m_sFilePath; }
  //! @return Get xml document from file
  inline CcXmlDocument& document()
  { return m_oDocument;}
  //! @return Xml root node
  inline CcXmlNode& rootNode()
  { return m_oDocument.rootNode(); }
  //! @return Get generated string from xml node
  inline CcString getXmlString()
  { return m_oDocument.getDocument(); }

  //! @param sFilePath: Set path to file
  void setFile(const CcString& sFilePath)
  { m_sFilePath = sFilePath; }

  /**
   * @brief Parse file at specified location
   * @param sFilePath: Path to file to parse
   * @return True if file was successfully parsed
   */
  bool parseFile(const CcString& sFilePath);

  /**
   * @brief Write xml content to file.
   * @param bIntend: True if format shoud become a readable formate
   * @return True if filew as successfully written
   */
  bool writeData(bool bIntend = false);

  /**
   * @brief Read data from already placed filepath
   * @return True if data was successfully read.
   */
  bool readData();

private:
  CcString      m_sFilePath;
  CcXmlDocument m_oDocument;
};
