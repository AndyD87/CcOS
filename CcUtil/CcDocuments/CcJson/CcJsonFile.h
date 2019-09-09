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
 * @subpage   CcJsonFile
 *
 * @page      CcJsonFile
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcJsonFile
 **/
#ifndef H_CcJsonFile_H_
#define H_CcJsonFile_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcFile.h"
#include "CcJson/CcJsonDocument.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcJsonFile {
public:
  /**
   * @brief Constructor
   */
  CcJsonFile();

  /**
   * @brief Constructor
   * @param sPathToFile: Path to file to parse instantly
   */
  CcJsonFile( const CcString& sPathToFile );

  /**
   * @brief Destructor
   */
  ~CcJsonFile();

  void setFile(const CcString& sPathToFile);
  bool parseFile(const CcString& sPathToFile);
  bool parseFile();

  const CcJsonDocument& getDocument()
    {return m_oDocument;}
  CcJsonDocument& document()
    { return m_oDocument;}

  bool write(bool bCompact = true);

private:
  CcJsonDocument m_oDocument;
  CcString m_sFilePath;
};

#endif // H_CcJsonFile_H_
