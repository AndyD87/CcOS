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
 * @page      Communication
 * @subpage   CcThreadContext
 *
 * @page      CcThreadContext
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcThreadContext
 */
#ifndef _CcThreadContext_H_
#define _CcThreadContext_H_

#include "CcBase.h"

class IThread;
class CcThreadData;

class CcThreadContext
{
public:
  CcThreadContext() = default;
  CcThreadContext(IThread* pThreadObject, CcThreadData* pData):
    pThreadObject(pThreadObject),
    pData(pData)
  {}
  void setData(CcThreadData* pNewData)
    { pData = pNewData; }
  IThread*      pThreadObject  = nullptr;
  CcThreadData* pData          = nullptr;
  bool          bClosed        = false;
};

#endif /* _CcThreadContext_H_ */
