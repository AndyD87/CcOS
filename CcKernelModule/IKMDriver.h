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
 * @page      CcKernelModule
 * @subpage   IKMDriver
 *
 * @page      IKMDriver
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IKMDriver
 */

#ifndef H_IKMDriver_H_
#define H_IKMDriver_H_

#include "CcBase.h"
#include "CcKernelModule.h"

class IKMDevice;
class CcKernelModuleContext;

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelModuleSHARED IKMDriver
{
public:
  IKMDriver(CcKernelModuleContext* pContext);
  virtual ~IKMDriver();

private:
  class CContext;
  CContext* m_pContext = nullptr;
};

#endif // _IKMDriver_H_
