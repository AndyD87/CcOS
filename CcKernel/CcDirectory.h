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
 * @page      CcDirectory
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcDirectory
 */
#ifndef CcDirectory_H_
#define CcDirectory_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFile.h"
#include "CcFileInfoList.h"

class CcDateTime;

/**
 * @brief File Handler.
 */
class CcKernelSHARED CcDirectory {
public:
  /**
   * @brief Constructor
   */
  CcDirectory( const CcString& path = "");


  /**
   * @brief Destructor
   */
  virtual ~CcDirectory( void );

  bool exists();
  static bool exists(const CcString& sPathToDirectory);

  static bool create(const CcString& sPathToDirectory);
  static bool create(const CcString& sPathToDirectory, bool bRecursive);
  bool create(bool bRecursive = false);

  static bool remove(const CcString& sPathToDirectory);
  static bool remove(const CcString& sPathToDirectory, bool bRecursive);
  bool remove(bool bRecursive = false);

  bool setCreated(const CcDateTime& oCreated);
  bool setModified(const CcDateTime& oModified);
  bool setUserId(uint16 uiUserId);
  bool setGroupId(uint16 uiGroupId);
  static CcFileInfoList getFileList(const CcString& sPathToDirectory);

protected: //Variables
  CcString m_Path;      //!< Path to File as String
};

#endif /* CcDirectory_H_ */
