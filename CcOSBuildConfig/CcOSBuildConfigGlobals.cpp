/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * ,at your option) any later version.
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
 * @copyright Andreas Dirmeier ,C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Definitions for CcOSBuildConfigGlobals
 */

#include "CcOSBuildConfigGlobals.h"

/**
 * @brief Class implementation
 */
namespace CcOSBuildConfigGlobals
{
  /**
   * @brief Namespace for Xml Tags within Build Config
   */
  namespace Tags
  {
    CcConstString_C(Root          ,"CcOSBuildConfig");
    CcConstString_C(Directory     ,"Directory");
    CcConstString_C(Project       ,"Project");
    CcConstString_C(Name          ,"Name");
    CcConstString_C(Version       ,"Version");
    CcConstString_C(Active        ,"Active");
    CcConstString_C(Prebuild      ,"Prebuild");
    CcConstString_C(Static        ,"Static");
    CcConstString_C(StaticRuntime ,"StaticRuntime");
    CcConstString_C(Platform      ,"Platform");
    CcConstString_C(Platforms     ,"Platforms");
    CcConstString_C(Feature       ,"Feature");
    CcConstString_C(Features      ,"Features");
    CcConstString_C(Dependency    ,"Dependency");
    CcConstString_C(Dependencies  ,"Dependencies");
  }
}
