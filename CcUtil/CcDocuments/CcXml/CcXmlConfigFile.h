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
 * @subpage   CcXmlConfigFile
 *
 * @page      CcXmlConfigFile
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlConfigFile
 **/
#ifndef _CcXmlConfigFile_H_
#define _CcXmlConfigFile_H_

#include "CcBase.h"
#include "CcXml/CcXmlFile.h"

/**
 * @brief Class impelmentation
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

  bool openAppConfig(const CcString& sProjectName, const CcString& sFileName);
  bool parseXml(const CcString& sPath)
    { return m_oConfigFile.parseFile(sPath); }
  CcXmlNode& getRootNode()
    { return m_oConfigFile.rootNode(); }

private:
  CcXmlFile m_oConfigFile;
};

#endif /* _CcXmlConfigFile_H_ */
