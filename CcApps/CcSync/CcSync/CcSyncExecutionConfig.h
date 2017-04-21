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
 * @page      CcSync
 * @subpage   CcSyncExecutionConfig
 *
 * @page      CcSyncExecutionConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncExecutionConfig
 **/
#ifndef CcSyncExecutionConfig_H_
#define CcSyncExecutionConfig_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcString.h"

class CcXmlNode;

/**
* @brief Class impelmentation
*/
class CcSyncSHARED CcSyncExecutionConfig
{
public:
  /**
   * @brief Constructor
   */
  CcSyncExecutionConfig(void);

  /**
   * @brief CopyConstructor
   */
  CcSyncExecutionConfig(const CcSyncExecutionConfig& oToCopy);

  /**
   * @brief MoveConstructor
   */
  CcSyncExecutionConfig(CcSyncExecutionConfig&& oToMove);
  
  /**
   * @brief Constructor
   */
  CcSyncExecutionConfig(CcXmlNode& pXmlNode);

  /**
   * @brief Destructor
   */
  virtual ~CcSyncExecutionConfig(void);

  void parseXmlNode(CcXmlNode& pXmlNode);

  CcSyncExecutionConfig& operator=(const CcSyncExecutionConfig& oToCopy);
  CcSyncExecutionConfig& operator=(CcSyncExecutionConfig&& oToMove);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSyncExecutionConfig& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSyncExecutionConfig& oToCompare) const;

private:
  CcString m_sName;
  CcString m_sExecutable;
  CcString m_sParameters;
  CcString m_sWorkingDir;
  CcString m_sTimeMask;
};

#endif /* CcSyncExecutionConfig_H_ */
