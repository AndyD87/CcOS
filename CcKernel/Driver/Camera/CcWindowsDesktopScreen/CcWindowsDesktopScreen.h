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
 * @page      Camera
 * @subpage   CcWindowsDesktopScreen
 *
 * @page      CcWindowsDesktopScreen
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsDesktopScreen
 */
#ifndef H_CcWindowsDesktopScreen_H_
#define H_CcWindowsDesktopScreen_H_

#include "CcBase.h"

#if defined(_MSC_VER) && !defined(CCKERNEL_MODE)
# ifndef CcWindowsDesktopScreenSHARED
#   ifdef screen_ccm_EXPORTS
 //    Cmake definition for shared build is set
#     define CcWindowsDesktopScreenSHARED __declspec(dllexport)
#   elif defined(CC_STATIC)
 //    CCOS will be build as static library no im-/export
#     define CcWindowsDesktopScreenSHARED
#   else
 //    if no definition found, we are on importing as dll
#     define CcWindowsDesktopScreenSHARED __declspec(dllimport)
#   endif
# endif
#else
# define CcWindowsDesktopScreenSHARED
#endif

#endif // H_CcWindowsDesktopScreen_H_
