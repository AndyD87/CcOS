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
 * @subpage   CcUdpFrame
 *
 * @page      CcUdpFrame
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUdpFrame
 */
#ifndef _CcUdpFrame_H_
#define _CcUdpFrame_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"

class CcKernelSHARED CcUdpFrame
{
public:
  CcUdpFrame();
  CcUdpFrame(const CcUdpFrame& oToCopy)
    { operator=(oToCopy);}
  CcUdpFrame(CcUdpFrame&& oToMove)
    { operator=(std::move(oToMove));}
  ~CcUdpFrame();
  
  CcUdpFrame& operator=(CcUdpFrame&& oToMove);
  CcUdpFrame& operator=(const CcUdpFrame& oToCopy);
  bool operator==(const CcUdpFrame& oToCompare) const;
  bool operator!=(const CcUdpFrame& oToCompare) const
    { return !operator==(oToCompare);}
};

#endif //_CcUdpFrame_H_
