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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsModule
 *            Abstract Class to define an interface to System own Shared Memory
 */
#pragma once

#include "CcWindowsGlobals.h"
#include "IModuleBase.h"
#include "CcObject.h"

/**
 * @brief Implementation of Shared Memory within Linux Systems
 */
class CcWindowsModule : public CcObject
{
public:
  virtual ~CcWindowsModule();

  /**
   * @brief Load module from filesystem with name as path
   * @param sName:    Module to load form FS
   * @param oKernel:  Kernel interface to pass to module
   * @return Status of opeation.
   */
  CcStatus loadModule(const CcString& sName, IKernel& oKernel);

  /**
   * @brief Unload module by it's handle
   * @param pModule: Target handle to unload
   */
  void unloadModule(void* pModule);

  /**
   * @brief Reset all handle values and cleanup
   */
  void resetHandles();

  //! @return Get name of module
  const CcString& getName()
  { return m_sName; }
  //! @return Get module handle
  inline IModuleBase* getModule()
  { return m_pModule; }

private: // Methos
  static void marker();
private: // Member
  CcString m_sName;
  IModuleBase* m_pModule            = nullptr;
  IModule_CreateFunction m_pCreate  = nullptr;
  IModule_RemoveFunction m_pRemove  = nullptr;
  void*    m_pInstance              = nullptr;

};
