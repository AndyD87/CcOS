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
 * @file
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxModule
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef H_CcLinuxModule_H_
#define H_CcLinuxModule_H_

#include "CcBase.h"
#include "IModuleBase.h"
#include "CcObject.h"

/**
 * @brief Implementation of Shared Memory within Linux Systems
 */
class CcLinuxModule : public CcObject
{
public:
  virtual ~CcLinuxModule();

  //! @return Get name of module
  const CcString& getName()
  { return m_sName; }

  /**
   * @brief Load module from path.
   * @param sName:    Path to file
   * @param oKernel:  Kernel to pass to module
   * @return Status of operation
   */
  CcStatus loadModule(const CcString& sName, const IKernel& oKernel);

  /**
   * @brief Unload an already loaded module from system
   * @param pModule: Hamndle of loaded module
   */
  void unloadModule(void* pModule);

  /**
   * @brief Cleanup all handles an pointers
   */
  void resetHandles();

  //! @return Get interface of loaded module
  inline IModuleBase* getModule()
  { return m_pModule; }

private: // Methos
  static void marker();

private: // Member
  CcString m_sName;
  IModuleBase* m_pModule = nullptr;
  IModule_CreateFunction m_pCreate;
  IModule_RemoveFunction m_pRemove;
  void*    m_pHandle = nullptr;

};

#endif // H_CcLinuxModule_H_
