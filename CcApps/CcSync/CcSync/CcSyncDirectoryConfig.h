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
 * @page      CcSyncDirectoryConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncDirectoryConfig
 **/
#ifndef CcSyncDirectoryConfig_H_
#define CcSyncDirectoryConfig_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcString.h"

class CcXmlNode;
class CcJsonObject;

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncDirectoryConfig
{
public:
  /**
   * @brief Constructor
   */
  CcSyncDirectoryConfig(void);

  /**
   * @brief Constructor
   */
  CcSyncDirectoryConfig(CcXmlNode& pXmlNode);
  
  /**
   * @brief Constructor
   */
  CcSyncDirectoryConfig(const CcJsonObject& pJsonNode);

  /**
   * @brief CopyConstructor
   */
  CcSyncDirectoryConfig(const CcSyncDirectoryConfig& oToCopy);

  /**
   * @brief MoveConstructor
   */
  CcSyncDirectoryConfig(CcSyncDirectoryConfig&& oToMove);

  /**
   * @brief Destructor
   */
  virtual ~CcSyncDirectoryConfig(void);

  CcSyncDirectoryConfig& operator=(const CcSyncDirectoryConfig& oToCopy);
  CcSyncDirectoryConfig& operator=(CcSyncDirectoryConfig&& oToMove);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSyncDirectoryConfig& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSyncDirectoryConfig& oToCompare) const;

  void parseXmlNode(CcXmlNode& pXmlNode);
  void parseJsonNode(const CcJsonObject& rJsonNode);

  void overrideLocation(const CcString& sPath)
    { m_sLocation = sPath; }

  const CcString& getName() const
    { return m_sName; }
  const CcString& getLocation() const
    { return m_sLocation; }

  CcXmlNode getXmlNode();

private:
  CcString m_sName;
  CcString m_sLocation;
};

#endif /* CcSyncDirectoryConfig_H_ */
