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
 * @brief     Class CcJsonFile
 **/
#pragma once

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

  /**
   * @brief Set file to parse for json data
   * @param sPathToFile:  Path to file
   */
  void setFile(const CcString& sPathToFile);

  /**
   * @brief Read and parse previosly set file from path
   * @return True if parsing was successfull
   */
  bool parseFile();

  /**
   * @brief Set file and directly parse for json data
   * @param sPathToFile: Path to file
   * @return True if file was read and parsed successfully
   */
  bool parseFile(const CcString& sPathToFile);

  //! @return Json root node
  const CcJsonDocument& getDocument()
  {return m_oDocument;}
  //! @return Json string data
  CcJsonDocument& document()
  { return m_oDocument;}

  /**
   * @brief Generate Json string and write to file
   * @param bCompact: If true no intendation will be used and
   *                  String will be as compact as possible
   * @return True if write was successfully
   */
  bool write(bool bCompact = true);

private:
  CcJsonDocument m_oDocument;
  CcString m_sFilePath;
};
