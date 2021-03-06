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
 * @page      Windows
 * @subpage   CcWindowsGlobals
 *
 * @page      CcWindowsGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     To keep code working on different compiler and platforms,
 *            sometimes it is needed to define some datatypes like uchar.
 */
#ifndef H_WINDOWSGLOBALS_H_
#define H_WINDOWSGLOBALS_H_

#define WIN32_LEAN_AND_MEAN
#include <CcBase.h>
CCEXTERNC_BEGIN
#include <windows.h>
CCEXTERNC_END
#include <CcBase.h>

#endif // H_WINDOWSGLOBALS_H_
