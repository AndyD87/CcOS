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
 * @page      CcOSBuildConfig
 * @subpage   CcOSBuildConfigPlatform
 *
 * @page      CcOSBuildConfigPlatform
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcOSBuildConfigPlatform
 **/
#ifndef _CcOSBuildConfigPlatform_H_
#define _CcOSBuildConfigPlatform_H_

#include "CcBase.h"
#include "CcOSBuildConfigBase.h"
#include "CcOSBuildConfigProject.h"

/**
 * @brief Class impelmentation
 */
class CcOSBuildConfigPlatform {
public:
  /**
   * @brief Constructor
   */
  CcOSBuildConfigPlatform() = default;

  /**
   * @brief Destructor
   */
  ~CcOSBuildConfigPlatform() = default;

  bool operator==(const CcOSBuildConfigPlatform&)  const
  {
    return false;
  }

public:
  CcString m_sName;
  CcOSBuildConfigProjectList m_oAllProjects;
  CcOSBuildConfigProjectList m_oQuarantaine;
  CcOSBuildConfigProjectList m_oNoBuild;
};

#ifdef _MSC_VER
template class CcOSBuildConfigSHARED CcList<CcOSBuildConfigPlatform>;
#endif

#endif /* _CcOSBuildConfigPlatform_H_ */
