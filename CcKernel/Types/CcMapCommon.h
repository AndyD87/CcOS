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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Common Maps for CcMap<K,T>
 **/
#ifndef H_CcMapCommon_H_
#define H_CcMapCommon_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcPair.h"
#include "CcMap.h"
#include "CcString.h"

class CcString;

/**
 * @name Common Maps
 * @{
 */
typedef CcPair<CcString, CcString> CcStringPair;
typedef CcMap<CcString, CcString> CcStringMap;

#ifdef _MSC_VER
template class CcKernelSHARED CcPair<CcString, CcString>;
template class CcKernelSHARED CcMap<CcString, CcString>;
#endif

/**
 * @}
 */


#endif // H_CcMapCommon_H_
