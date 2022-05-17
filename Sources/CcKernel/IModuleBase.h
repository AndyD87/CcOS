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
 * @brief     Class IModule
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "IKernel.h"
#include "CcEventHandler.h"
#include "CcVector.h"

class IModuleBase;

//! @brief Function declaration of an IModule_Create method
typedef IModuleBase* (*IModule_CreateFunction)(void);

//! @brief Function declaration of an IModule_Remove method
typedef void (*IModule_RemoveFunction)(IModuleBase*);

//! @brief String definition of IModule_Create
#define IModule_CreateFunctionName "IModule_Create"

//! @brief String definition of IModule_Remove
#define IModule_RemoveFunctionName "IModule_Remove"

#ifdef CC_DOXYGEN_ACTIVE
/**
 * @brief Create and initialize module
 * @return Created module
 */
IModuleBase* IModule_Create();

/**
 * @brief Deinitialize and remove module
 * @param pModule: Module to unload
 */
void IModule_Remove(IModuleBase* pModule);
#endif

/**
 * @brief Interface to create a module for dynamic loading.
 *        Every module has to implement the following modules:
 *
 *        IModuleBase* IModule_Create(void);
 *        This is used to create a module inheriting IModuleBase and passthrough registered to kernel.
 *
 *        void IModule_Remove(IModuleBase*);
 *        This method should cleanup and delete the module.
 */
class CcKernelSHARED IModuleBase : public CcObject
{
public:
  IModuleBase() = default;
  virtual ~IModuleBase() = default;

  /**
   * @brief This method will be called from kernel if registration is done.
   * @return Sucess if initializing succeeded.
   */
  virtual CcStatus init() = 0;

  /**
   * @brief This method will be called from kernel if registration is removed.
   * @return Sucess if deinitializing succeeded.
   */
  virtual CcStatus deinit() = 0;

  /**
   * @brief For signaling unload event for other objects, they can be
   *        registered here.
   * @param oUnloadEvent: oUnload event calling object.
   */
  void registerOnUnload(const CcEvent& oUnloadEvent);

  /**
   * @brief Remove all the given objects from unload event list.
   * @param pUnregister: Object to unregister.
   */
  void deregisterOnUnload(CcObject* pUnregister);

  /**
   * @brief Set kernel interface on Module load.
   *        After that, init will be executed
   * @param oKernel: Kernel to import.
   */
  virtual void setKernel(IKernel& oKernel) = 0;

protected:
  CcEventHandler  m_oUnloadEvent; //!< Event list if unload is requested.
  IKernel         m_oKernel;      //!< Kernel object with new/delete and all drivers and devices.
};
