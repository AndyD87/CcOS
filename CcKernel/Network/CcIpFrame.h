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
 * @page      Network
 * @subpage   CcIpFrame
 *
 * @page      CcIpFrame
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIpFrame
 */
#ifndef _CcIpFrame_H_
#define _CcIpFrame_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"

class CcKernelSHARED CcIpFrame
{
public:
  CcIpFrame();
  CcIpFrame(const CcIpFrame& oToCopy)
    { operator=(oToCopy);}
  CcIpFrame(CcIpFrame&& oToMove)
    { operator=(std::move(oToMove));}
  ~CcIpFrame();
  
  CcIpFrame& operator=(CcIpFrame&& oToMove);
  CcIpFrame& operator=(const CcIpFrame& oToCopy);
  bool operator==(const CcIpFrame& oToCompare) const;
  bool operator!=(const CcIpFrame& oToCompare) const
    { return !operator==(oToCompare);}
};

#endif //_CcIpFrame_H_
