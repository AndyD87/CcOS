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
 * @page      Types
 * @subpage   CcVersion
 *
 * @page      CcVersion
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVersion
 */
#ifndef _CcVersion_H_
#define _CcVersion_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcGlobalStrings.h"

/**
 * @brief Version Object for handling version numbers with up to 4 digits.
 *        Version Numbers can be parse from and exported to strings.
 *        
 *        Default Version format of this class is Major.Minor.Build.Revision
 *
 *        Valid values are 0-65535
 */
class CcKernelSHARED CcVersion
{
public:
  /**
   * @brief Default constructor, all version digits are set to 0
   */
  CcVersion() : CcVersion(0, 0, 0, 0)
    {}
  /**
   * @brief Constructor with a given major version number.
   * @param uiMajor: Major version number to set.
   */
  CcVersion(uint16 uiMajor) : CcVersion(uiMajor, 0, 0, 0)
    {}
  /**
   * @brief Constructor with a given major and minor version number.
   * @param uiMajor: Major version number to set.
   * @param uiMinor: Minor version number to set.
   */
  CcVersion(uint16 uiMajor, uint16 uiMinor) : CcVersion(uiMajor, uiMinor, 0, 0)
    {}
  /**
   * @brief Constructor with a given major, minor and build version number.
   * @param uiMajor: Major version number to set.
   * @param uiMinor: Minor version number to set.
   * @param uiBuild: Build version number to set.
   */
  CcVersion(uint16 uiMajor, uint16 uiMinor, uint16 uiBuild) : CcVersion(uiMajor, uiMinor, uiBuild, 0)
    {}
  /**
   * @brief Constructor with all version numbers, major, minor, build and revision..
   * @param uiMajor: Major version number to set.
   * @param uiMinor: Minor version number to set.
   * @param uiBuild: Build version number to set.
   * @param uiRevision: Revision version number to set.
   */
  CcVersion(uint16 uiMajor, uint16 uiMinor, uint16 uiBuild, uint16 uiRevision);

  /**
   * @brief Create Version Object by parsing a Version string.
   * @param sVersion: String to parse.
   * @param sSeperator: Seperator wich placed between the numbers, default is '.'.
   */
  CcVersion(const CcString& sVersion, const CcString& sSeperator = CcGlobalStrings::Seperators::Dot);

  /**
   * @brief Destructor, nothing special to do here.
   */
  ~CcVersion()
    {}

  /**
  * @brief Compare Operators
  * @{
  */
  bool operator>(const CcVersion& oToCompare) const;
  bool operator<(const CcVersion& oToCompare) const;
  bool operator==(const CcVersion& oToCompare) const;
  /**
  * @}
  */

  /**
   * @brief Parse Version from an string.
   * @param sVersion: String to parse.
   * @param sSeperator: Seperator wich placed between the numbers, default is '.'.
   * @return true if at least one the major number was successfully parsed.
   */
  bool setVersionString(const CcString& sVersion, const CcString& sSeperator = CcGlobalStrings::Seperators::Dot);

  /**
   * @brief Convert stored Version into a string.
   *        The depth can be varied, default is 2 to to get Major and Minor Version.
   * @param iDepth: Number of Depth, minimum 1 maximum 4, default is full length of 4.
   * @param sSeperator: Seperator to be placed between the numbers, default is '.'.
   * @return Version number as string. If error occured, string is empty.
   */
  CcString getVersionString(size_t iDepth = 4, const CcString& sSeperator = CcGlobalStrings::Seperators::Dot) const;

  /**
   * @brief Direct member access
   * @{
   */
  inline uint16 getMajor() { return m_uiMajor; }
  inline uint16 getMinor() { return m_uiMinor; }
  inline uint16 getBuild() { return m_uiBuild; }
  inline uint16 getRevision() { return m_uiRevision; }
  inline void setMajor(uint16 uiMajor) { m_uiMajor = uiMajor; }
  inline void setMinor(uint16 uiMinor) { m_uiMinor = uiMinor; }
  inline void setBuild(uint16 uiBuild) { m_uiBuild = uiBuild; }
  inline void setRevision(uint16 uiRevision) { m_uiRevision = uiRevision; }
  /**
   * @}
   */

  /**
   * @brief Set version number on a specific postion like 1 = Minor.
   * @param uiPos: Position for new number.
   * @param uiVersionNr: Version number to set on target position.
   */
  void setVersionByPosition(uint16 uiPos, uint16 uiVersionNr);

private:
  uint16 m_uiMajor;
  uint16 m_uiMinor;
  uint16 m_uiBuild;
  uint16 m_uiRevision;
};

#endif //_CcVersion_H_
