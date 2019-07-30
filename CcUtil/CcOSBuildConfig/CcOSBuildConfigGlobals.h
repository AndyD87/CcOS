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
#ifndef H_CcOSBuildConfigGlobals_H_
#define H_CcOSBuildConfigGlobals_H_

#include "CcBase.h"
#include "CcOSBuildConfig.h"
#include "CcString.h"

/**
 * @brief Define all Variables wich are important for this project.
 */
namespace CcOSBuildConfigGlobals
{
  /**
   * @brief Namespace for Xml Tags withicn 
   */
  namespace Tags
  {
    extern const CcOSBuildConfigSHARED CcString Root         ; //!< Root          as tag fro config
    extern const CcOSBuildConfigSHARED CcString Directory    ; //!< Directory     as tag fro config
    extern const CcOSBuildConfigSHARED CcString Project      ; //!< Project       as tag fro config
    extern const CcOSBuildConfigSHARED CcString Name         ; //!< Name          as tag fro config
    extern const CcOSBuildConfigSHARED CcString Version      ; //!< Version       as tag fro config
    extern const CcOSBuildConfigSHARED CcString Active       ; //!< Active        as tag fro config
    extern const CcOSBuildConfigSHARED CcString Prebuild     ; //!< Prebuild      as tag fro config
    extern const CcOSBuildConfigSHARED CcString Static       ; //!< Static        as tag fro config
    extern const CcOSBuildConfigSHARED CcString StaticRuntime; //!< StaticRuntime as tag fro config
    extern const CcOSBuildConfigSHARED CcString Platform     ; //!< Platform      as tag fro config
    extern const CcOSBuildConfigSHARED CcString Platforms    ; //!< Platforms     as tag fro config
    extern const CcOSBuildConfigSHARED CcString Feature      ; //!< Feature       as tag fro config
    extern const CcOSBuildConfigSHARED CcString Features     ; //!< Features      as tag fro config
    extern const CcOSBuildConfigSHARED CcString Dependency   ; //!< Dependency    as tag fro config
    extern const CcOSBuildConfigSHARED CcString Dependencies ; //!< Dependencies  as tag fro config
  }
}
#endif /* H_CcOSBuildConfigGlobals_H_ */
