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
 * @page      CcVBoxIfc
 * @subpage   CcVBoxIfc
 *
 * @page      CcVBoxIfc
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVBoxIfc
 */
#ifndef _CcVBoxIfc_H_
#define _CcVBoxIfc_H_

#include "CcAppInterfaces.h"
#include "CcString.h"
#include "CcUuid.h"
#include "CcList.h"

class CcVersion;

/**
 * @brief CcVBoxIfc Class impelmentation
 */
class CcAppInterfacesSHARED CcVBoxIfc
{
public:
  class CcAppInterfacesSHARED CVmInfoListItem
  {
  public:
    bool operator==(const CVmInfoListItem& oToCompare) const
      { return (sName == oToCompare.sName) && (oUuId == oToCompare.oUuId); }
    CVmInfoListItem& operator=(const CVmInfoListItem& oToCompare)
      { sName = oToCompare.sName; oUuId = oToCompare.oUuId; return *this; } 
  public:
    CcString sName;
    CcUuid   oUuId;
  };

  typedef CcList<CcVBoxIfc::CVmInfoListItem> CVmInfoList;

  /**
   * @brief Constructor
   */
  CcVBoxIfc(const CcString& sPath = "");

  /**
   * @brief Destructor
   */
  ~CcVBoxIfc();

  bool isValid()
    { return m_bExeFound; }
  CcVersion getVersion(bool* bOk = nullptr);
  CVmInfoList getVmList(bool* bOk = nullptr);

  bool setExecutable(const CcString& sPath);
private:
  CcString exec(const CcStringList& sArgs, bool* bOk = nullptr);

private:
  class CPrivate;
  CcString m_sExePath;
  CcString m_sWorkingDir;
  bool m_bExeFound = false;
};

#ifdef _MSC_VER
template class CcAppInterfacesSHARED CcList<CcVBoxIfc::CVmInfoListItem>;
#endif

#endif /* _CcVBoxIfc_H_ */
