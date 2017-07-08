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
 * @page      Types
 * @subpage   CcMap
 *
 * @page      CcMap
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Common Maps for CcMap<K,T>
 **/
#ifndef CcMapCommon_H_
#define CcMapCommon_H_

#include "CcBase.h"
#include "CcKernelBase.h"
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

#ifdef WIN32
template class CcKernelSHARED CcPair<CcString, CcString>;
template class CcKernelSHARED CcMap<CcString, CcString>;
#endif

/**
* @}
*/


#endif /* CcMapCommon_H_ */
