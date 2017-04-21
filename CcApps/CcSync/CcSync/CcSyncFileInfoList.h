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
 * @subpage   CcSyncFileInfoList
 *
 * @page      CcSyncFileInfoList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncFileInfoList
 **/
#ifndef CcSyncFileInfoList_H_
#define CcSyncFileInfoList_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcSyncFileInfo.h"
#include "CcList.h"

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncFileInfoList : public CcList<CcSyncFileInfo>
{
public:
  /**
   * @brief Constructor
   */
  CcSyncFileInfoList()
  {}

  /**
   * @brief Destructor
   */
  ~CcSyncFileInfoList( void )
  {}

  bool containsFile(const CcString& sFilename) const;
  bool containsFile(uint64 uiFileId) const;
  const CcSyncFileInfo& getFile(const CcString& sFilename) const;
  const CcSyncFileInfo& getFile(uint64 uiFileId) const;
  bool removeFile(const CcString& sFilename);
  bool removeFile(uint64 uiFileId);

  inline bool containsDirectory(const CcString& sDirectoryName) const
    { return containsFile(sDirectoryName);}
  inline const CcSyncFileInfo& getDirectory(const CcString& sDirectoryName) const
    { return getFile(sDirectoryName); }

};

#endif /* CcSyncFileInfoList_H_ */
