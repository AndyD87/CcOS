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
    extern CcOSBuildConfigSHARED CcConstString_H(Root         ); //!< Root          as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Directory    ); //!< Directory     as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Project      ); //!< Project       as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Name         ); //!< Name          as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Version      ); //!< Version       as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Active       ); //!< Active        as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Prebuild     ); //!< Prebuild      as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Static       ); //!< Static        as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(StaticRuntime); //!< StaticRuntime as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Platform     ); //!< Platform      as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Platforms    ); //!< Platforms     as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Feature      ); //!< Feature       as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Features     ); //!< Features      as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Dependency   ); //!< Dependency    as tag fro config
    extern CcOSBuildConfigSHARED CcConstString_H(Dependencies ); //!< Dependencies  as tag fro config
  }
}
#endif // H_CcOSBuildConfigGlobals_H_
