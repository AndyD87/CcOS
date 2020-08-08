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
 * @subpage   CcTestModule
 *
 * @page      CcTestModule
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTestModule
 **/
#ifndef H_CcTestModule_H_
#define H_CcTestModule_H_

#include "CcBase.h"
#include "IModule.h"

class IKernel;

/**
 * @brief CcTestModule is used to used to test if loading libraries is working.
 */
class CcTestModule : public IModule
{
public:
  CcTestModule(const IKernel& oKernel);
  virtual ~CcTestModule();
  virtual CcStatus init() override;
  virtual CcStatus deinit() override;
};

#endif // H_CcTestModule_H_
