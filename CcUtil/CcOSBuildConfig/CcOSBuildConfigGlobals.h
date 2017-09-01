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
 * @subpage   CcOSBuildConfigGlobals
 *
 * @page      CcOSBuildConfigGlobals
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Global Variables for CcOSBuildConfig
 **/
#ifndef CcOSBuildConfigGlobals_H_
#define CcOSBuildConfigGlobals_H_

#include "CcBase.h"
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
    static const CcString Root          ("CcOSBuildConfig");
    static const CcString Directory     ("Directory");
    static const CcString Project       ("Project");
    static const CcString Name          ("Name");
    static const CcString Version       ("Version");
    static const CcString Active        ("Active");
    static const CcString Prebuild      ("Prebuild");
    static const CcString Static        ("Static");
    static const CcString StaticRuntime ("StaticRuntime");
    static const CcString Platform      ("Platform");
    static const CcString Platforms     ("Platforms");
    static const CcString Feature       ("Feature");
    static const CcString Features      ("Features");
    static const CcString Dependency    ("Dependency");
    static const CcString Dependencies  ("Dependencies");
  }
}
#endif /* CcOSBuildConfigGlobals_H_ */
