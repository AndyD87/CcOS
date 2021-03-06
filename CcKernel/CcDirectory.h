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
 * @page      CcKernel
 * @subpage   CcDirectory
 *
 * @page      CcDirectory
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDirectory
 */
#ifndef H_CcDirectory_H_
#define H_CcDirectory_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcFile.h"
#include "CcFileInfoList.h"

class CcDateTime;

/**
 * @brief File Handler.
 */
class CcKernelSHARED CcDirectory
{
public:
  /**
   * @brief Constructor
   */
  CcDirectory( const CcString& sPath = "") : m_Path(sPath)
    {}

  /**
   * @brief Destructor
   */
  ~CcDirectory() = default;

  bool exists();
  static bool exists(const CcString& sPathToDirectory);

  static bool create(const CcString& sPathToDirectory, bool bRecursive = false, bool bFaileOnExists = false);
  static bool create(const char* sPathToDirectory, bool bRecursive = false, bool bFaileOnExists = false)
    { return create(CcString(sPathToDirectory), bRecursive, bFaileOnExists); }
  bool create(bool bRecursive = false, bool bFaileOnExists = false);

  static bool move(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo);
  bool move( const CcString& sPathToDirectoryTo);

  static bool copy(const CcString& sPathToDirectoryFrom, const CcString& sPathToDirectoryTo);
  bool copy(const CcString& sPathToDirectoryTo);

  static bool remove(const CcString& sPathToDirectory);
  static bool remove(const CcString& sPathToDirectory, bool bRecursive);
  static bool remove(const char* pcPath)
    { return remove(CcString(pcPath)); }
  bool remove(bool bRecursive = false);

  /**
   * @brief remove all files and, if Recursive is set, all folders within directory
   * @param bRecursive: remove directories too
   * @return true if succeeded
   */
  bool clear(bool bRecursive = false);
  
  /**
   * @brief remove all files and, if Recursive is set, all folders within directory
   * @param bRecursive: remove directories too
   * @return true if succeeded
   */
  static bool clear(const CcString& sPath, bool bRecursive = false);

  bool setCreated(const CcDateTime& oCreated);
  bool setModified(const CcDateTime& oModified);
  bool setUserId(uint32 uiUserId);
  bool setGroupId(uint32 uiGroupId);
  static CcFileInfoList getFileList(const CcString& sPathToDirectory);

protected: //Variables
  CcString m_Path;      //!< Path to File as String
};

#endif // H_CcDirectory_H_
