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
 * @subpage   IIo
 *
 * @page      IIo
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxModule
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef H_CcLinuxModule_H_
#define H_CcLinuxModule_H_

#include "CcBase.h"
#include "IModule.h"

/**
 * @brief Implementation of Shared Memory within Linux Systems
 */
class CcLinuxModule : public IModule
{
public:
  /**
   * @brief Create a Shared Memory by name and with specific size
   * @param sName: Name of new Memory
   */
  CcLinuxModule(const CcString& sName);

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxModule();

  virtual CcStatus init() override;
  virtual CcStatus deinit() override;
private:
  class CPrivate;
  CPrivate* getContext()
    { return static_cast<CPrivate*>(m_pContext); }
};

#endif // H_CcLinuxModule_H_
