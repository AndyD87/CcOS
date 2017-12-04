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
 * @subpage   CcOSBuildConfigGlobals
 *
 * @page      CcOSBuildConfigGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Global Variables for CcOSBuildConfig
 **/
#ifndef CcOSBuildConfigGlobals_H_
#define CcOSBuildConfigGlobals_H_

#include "CcBase.h"
#include "CcOSBuildConfig.h"
#include "CcString.h"

/**
 * @brief Class impelmentation
 */
namespace CcOSBuildConfigGlobals
{
  /**
   * @brief Namespace for Xml Tags withicn 
   */
  namespace Tags
  {
    extern const CcOSBuildConfigSHARED CcString Root         ;
    extern const CcOSBuildConfigSHARED CcString Directory    ;
    extern const CcOSBuildConfigSHARED CcString Project      ;
    extern const CcOSBuildConfigSHARED CcString Name         ;
    extern const CcOSBuildConfigSHARED CcString Version      ;
    extern const CcOSBuildConfigSHARED CcString Active       ;
    extern const CcOSBuildConfigSHARED CcString Prebuild     ;
    extern const CcOSBuildConfigSHARED CcString Static       ;
    extern const CcOSBuildConfigSHARED CcString StaticRuntime;
    extern const CcOSBuildConfigSHARED CcString Platform     ;
    extern const CcOSBuildConfigSHARED CcString Platforms    ;
    extern const CcOSBuildConfigSHARED CcString Feature      ;
    extern const CcOSBuildConfigSHARED CcString Features     ;
    extern const CcOSBuildConfigSHARED CcString Dependency   ;
    extern const CcOSBuildConfigSHARED CcString Dependencies ;
  }
}
#endif /* CcOSBuildConfigGlobals_H_ */
