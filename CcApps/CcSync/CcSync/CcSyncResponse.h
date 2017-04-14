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
 * @page      CcSyncResponse
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncResponse
 **/
#ifndef CcSyncResponse_H_
#define CcSyncResponse_H_

#include "CcSync.h"
#include "CcError.h"
#include "ESyncCommandType.h"
#include "CcJson/CcJsonObject.h"
#include "CcSyncFileInfoList.h"
#include "CcSyncFileInfoList.h"

class CcByteArray;
class CcSyncAccountConfig;

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncResponse
{
public:
  /**
   * @brief Constructor
   */
  CcSyncResponse( void );

  /**
   * @brief CopyConstructor
   */
  CcSyncResponse( const CcSyncResponse& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcSyncResponse( CcSyncResponse&& oToMove );

  /**
   * @brief Destructor
   */
  virtual ~CcSyncResponse( void );


  CcSyncResponse& operator=(const CcSyncResponse& oToCopy);
  CcSyncResponse& operator=(CcSyncResponse&& oToMove);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSyncResponse& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSyncResponse& oToCompare) const;

  bool parseData(const CcString& oData);

  void init(ESyncCommandType eCommandType);

  CcByteArray getBinary();
  inline ESyncCommandType getCommandType() const
    { return m_eType; }
  inline CcJsonObject& data()
    { return m_oData; }
  inline const CcJsonObject& getData()
    { return m_oData; }

  bool hasError();
  CcError getError();
  CcString getErrorMsg();

  void setLogin(const CcString& sUserToken);
  void setResult(bool uiResult);
  void setError(CcError uiErrorCode, const CcString& sErrorMsg);
  void addAccountInfo(const CcSyncAccountConfig& oAccountConfig);
  CcSyncAccountConfig getAccountInfo();

  CcString getSession();

  void addDirectoryDirectoryInfoList(const CcSyncFileInfoList& oDirectoryInfoList, const CcSyncFileInfoList& oFileInfoList);

  bool hasFileInfo();
  void addFileInfo(const CcSyncFileInfo& oDirectoryInfo);
  CcSyncFileInfo getFileInfo();

  bool getDirectoryDirectoryInfoList(CcSyncFileInfoList& oDirectoryInfoList, CcSyncFileInfoList& oFileInfoList);

private: // Methods
  bool getTypeFromData();

private:
  ESyncCommandType m_eType;
  CcJsonObject m_oData;
  bool m_bHasAdditionalData = false;
};

#endif /* CcSyncResponse_H_ */
