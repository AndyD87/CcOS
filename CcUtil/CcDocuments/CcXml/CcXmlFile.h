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
 * @page      CcXml
 * @subpage   CcXmlFile
 *
 * @page      CcXmlFile
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlFile
 **/
#ifndef CcXmlFile_H_
#define CcXmlFile_H_

#include "CcBase.h"
#include "CcXml/CcXml.h"
#include "CcXml/CcXmlDocument.h"


/**
 * @brief Class impelmentation
 */
class CcDocumentsSHARED CcXmlFile 
{
public:
  /**
   * @brief Constructor
   */
  CcXmlFile( void );

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
  ~CcXmlFile( void );

  CcXmlFile& operator=(const CcXmlFile& oToCopy);
  CcXmlFile& operator=(CcXmlFile&& oToMove);
  bool operator==(const CcXmlFile& oToCompare) const;
  bool operator!=(const CcXmlFile& oToCompare) const;

  inline const CcString& getFile() const
    { return m_sFilePath; }
  void setFile(const CcString& sFilePath)
    { m_sFilePath = sFilePath; }
  bool writeData(bool bIntend = false);
  bool readData();

  inline CcXmlNode& rootNode()
    { return m_oDocument.rootNode(); }
  inline CcString getXmlString()
    { return m_oDocument.getXmlDocument(); }

private:
  CcString      m_sFilePath;
  CcXmlDocument m_oDocument;
};

#endif /* CcXmlFile_H_ */
