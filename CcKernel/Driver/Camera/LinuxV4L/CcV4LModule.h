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
 * @page      CcNetwork
 * @subpage   CcV4LModule
 *
 * @page      CcV4LModule
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcV4LModule
 */

#ifndef H_CcV4LModule_H_
#define H_CcV4LModule_H_

#include "CcBase.h"
#include "LinuxV4L.h"
#include "CcV4LDriver.h"
#include "IModule.h"

class CcV4LModule : public IModule
{
public:
  CcV4LModule(const IKernel& oKernel);
  virtual ~CcV4LModule();

  virtual CcStatus init();
  virtual CcStatus deinit();

private:
  CcV4LDriver m_oDriver;
};

#endif // H_CcV4LModule_H_
