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
 * @page      CcUtil
 * @subpage   CcVDiskDriver
 *
 * @page      CcVDiskDriver
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVDiskDriver
 **/
#ifndef H_CcVDiskDriver_H_
#define H_CcVDiskDriver_H_

#include "CcBase.h"
#include "IDriver.h"

/**
 * @brief Class impelmentation
 */
class CcVDiskDriver : public NKernelModule::IDriver
{
public:
  /**
   * @brief Constructor
   */
  CcVDiskDriver(CcKernelModuleContext* pContext);

  /**
   * @brief Destructor
   */
  virtual ~CcVDiskDriver();

  virtual NKernelModule::IDevice* createDevice() override;
};

#endif // H_CcVDiskDriver_H_
