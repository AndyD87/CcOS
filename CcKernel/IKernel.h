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
 * @page      CcKernel
 * @subpage   IKernel
 *
 * @page      IKernel
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IKernel
 */
#ifndef H_IKernel_H_
#define H_IKernel_H_

#include "CcBase.h"
#include "IDevice.h"

class CcKernelPrivate;

/**
 * @brief This interface can share Kernel Resources with Modules and other
 *        parts which does not have direct access to it.
 *        An Instance of this module can be genareted from CcKernel::getInterface
 */
class CcKernelSHARED IKernel
{
public:
  IKernel() = default;
  IKernel(CcKernelPrivate* pContex) :
    pBaseObject(this),
    pContext(pContex)
  { }
  IKernel*          pBaseObject = nullptr;
  CcKernelPrivate*  pContext    = nullptr;
};

#endif // H_IKernel_H_
