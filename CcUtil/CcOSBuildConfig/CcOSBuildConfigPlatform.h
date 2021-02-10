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
 * @brief     Class CcOSBuildConfigPlatform
 **/
#pragma once

#include "CcBase.h"
#include "CcOSBuildConfigBase.h"
#include "CcOSBuildConfigProject.h"

/**
 * @brief Class implementation
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

  //! @brief Fake implementation for lists
  bool operator==(const CcOSBuildConfigPlatform&)  const
  { return false; }

public:
  CcString m_sName;                           //!< Name of Platform
  CcOSBuildConfigProjectList m_oAllProjects;  //!< List of all projects required for this platform
  CcOSBuildConfigProjectList m_oQuarantaine;  //!< List of all projects marked as nice to have
  CcOSBuildConfigProjectList m_oNoBuild;      //!< List of all projects marked as not part of default build
};

#ifdef _MSC_VER
template class CcOSBuildConfigSHARED CcList<CcOSBuildConfigPlatform>;
#endif
