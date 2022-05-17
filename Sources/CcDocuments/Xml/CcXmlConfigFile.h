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
 * @brief     Class CcXmlConfigFile
 **/
#pragma once

#include "CcBase.h"
#include "Xml/CcXmlFile.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcXmlConfigFile 
{
public:
  /**
   * @brief Constructor
   */
  CcXmlConfigFile();

  /**
   * @brief Destructor
   */
  virtual ~CcXmlConfigFile();

  /**
   * @brief Open application config from system config directory
   * @param sProjectName: Name project and subdirectory in config dir
   * @param sFileName:    Name of config file in config dir
   * @return True if file was found and opened
   */
  bool openAppConfig(const CcString& sProjectName, const CcString& sFileName);

  /**
   * @brief Parse file at specific location
   * @param sPath: Path to file to open and parse
   * @return True if file was parsed successfully
   */
  bool parseXml(const CcString& sPath)
  { return m_oConfigFile.parseFile(sPath); }

  //! @return Get root node of parsed file
  CcXmlNode& getRootNode()
  { return m_oConfigFile.rootNode(); }

private:
  CcXmlFile m_oConfigFile;
};
