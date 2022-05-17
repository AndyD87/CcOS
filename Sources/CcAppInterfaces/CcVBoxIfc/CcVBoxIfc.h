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
 * @brief     Class CcVBoxIfc
 */
#pragma once

#include "CcAppInterfaces.h"
#include "CcString.h"
#include "CcUuid.h"
#include "CcList.h"

class CcVersion;

/**
 * @brief Call systems vboxmanage interface and parse the response messages.
 */
class CcAppInterfacesSHARED CcVBoxIfc
{
public:
  /**
   * @brief Element of CVmInfoList.
   *        It will hold all informations from an Virtualbox Machine.
   */
  class CcAppInterfacesSHARED CVmInfoListItem
  {
  public:
    //! @brief default constructor
    CVmInfoListItem() = default;
    CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CVmInfoListItem)

    /**
     * @brief Compare contents of VM if they are same.
     * @param oToCompare: Object to compare to
     * @return True if informations from bot VMs are same
     */
    bool operator==(const CVmInfoListItem& oToCompare) const
    { return (sName == oToCompare.sName) && (oUuId == oToCompare.oUuId); }

    /**
     * @brief Compare contents of VM if they are not same.
     * @param oToCompare: Object to compare to
     * @return True if informations from bot VMs are not same
     */
    CVmInfoListItem& operator=(const CVmInfoListItem& oToCompare)
    { sName = oToCompare.sName; oUuId = oToCompare.oUuId; return *this; }

  public:
    CcString sName;  //!< Name of VM
    CcUuid   oUuId;  //!< Unique identifier of VM
  };

  //! List of Informations of VMs
  typedef CcList<CcVBoxIfc::CVmInfoListItem> CVmInfoList;

  /**
   * @brief Constructor
   */
  CcVBoxIfc(const CcString& sPath = "");

  /**
   * @brief Destructor
   */
  ~CcVBoxIfc();

  //! @return True if vboxmanage is available for execution.
  bool isValid()
  { return m_bExeFound; }

  /**
   * @brief Get Version of vboxmanage
   * @param pbOk: If not nullptr, boolean value will be set to true if
   *              Version was parsed successfully
   * @return Version of vboxmanage
   */
  CcVersion getVersion(bool* pbOk = nullptr);

  /**
   * @brief Get list of available VMs
   * @param pbOk: If not nullptr, boolean value will be set to true if
   *              All VMs were parsed successfully
   * @return List of VMs
   */
  CVmInfoList getVmList(bool* pbOk = nullptr);

  /**
   * @brief Overwrite the vboxmanage executable to use.
   *        A Version will be queried to get verified if api is working.
   * @param sPath: Path to vboxmanage to use.
   * @return True if executable is working as expected
   */
  bool setExecutable(const CcString& sPath);
private:
  /**
   * @brief Execute VBoxmanage and get response
   * @param sArgs: List of arguments to execute
   * @param pbOk: If not nullptr, boolean value will be set to true if
   *              vboxmanage returned with exitcode 0
   * @return
   */
  CcString exec(const CcStringList& sArgs, bool* pbOk = nullptr);

private:
  class CPrivate;
  CcString m_sExePath;      //!< Path to executable, to be set by setExecutable
  CcString m_sWorkingDir;   //!< Working directory where vboxmanage should be executed in
  bool m_bExeFound = false; //!< True if setExecutable succeeded otherwise false.
};

template class CcAppInterfacesSHARED CcList<CcVBoxIfc::CVmInfoListItem>;
