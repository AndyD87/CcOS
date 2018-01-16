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
 * @subpage   CcListCommon
 *
 * @page      CcListCommon
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Common Maps for CcList<K,T>
 **/
#ifndef _CcListCommon_H_
#define _CcListCommon_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcList.h"
#include "CcGroup.h"
#include "CcVariant.h"

/**
 * @name Common Maps
 * @{
 */
#ifdef WIN32
template class CcKernelSHARED CcList<CcVariant>;
#endif
typedef CcList<CcVariant> CcVariantList;

/**
 * @}
 */


#endif /* _CcListCommon_H_ */
