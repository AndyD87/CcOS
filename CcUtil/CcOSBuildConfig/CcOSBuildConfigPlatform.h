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
 * @page      CcUtil
 * @subpage   CcOSBuildConfigPlatform
 *
 * @page      CcOSBuildConfigPlatform
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcOSBuildConfigPlatform
 **/
#ifndef CcOSBuildConfigPlatform_H_
#define CcOSBuildConfigPlatform_H_

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
  CcOSBuildConfigPlatform( void )
    {}

  /**
   * @brief Destructor
   */
  virtual ~CcOSBuildConfigPlatform(void)
    {}

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


#ifdef WIN32
template class CcOSBuildConfigSHARED CcList<CcOSBuildConfigPlatform>;
#endif

#endif /* CcOSBuildConfigPlatform_H_ */
