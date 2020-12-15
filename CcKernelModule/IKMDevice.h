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
 * @subpage   IKMDevice
 *
 * @page      IKMDevice
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IKMDevice
 */

#ifndef H_IKMDevice_H_
#define H_IKMDevice_H_

#include "CcBase.h"
#include "CcKernelModule.h"

class IKMDevice;

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelModuleSHARED IKMDevice
{
public:
  IKMDevice();
  virtual ~IKMDevice();

private:
  class CContext;
  CContext*   m_pContext = nullptr;
  IKMDevice*  m_pNextDevice = nullptr;
};

#endif // _IKMDevice_H_
