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
 * @brief     Class IModule
 */
#pragma once

#include "IModuleBase.h"

#ifdef WINDOWS
  extern BOOL WINAPI DllMain
  (
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved    // reserved
  );
#else
  /**
   * @brief Use this static class for handling library load and
   *        and unload process.
   */
  class StartupAndCleanup
  {
  public:
    /**
     * @brief Lib entry
     */
    StartupAndCleanup();

    /**
     * @brief Lib exit
     */
    ~StartupAndCleanup();
  };
  //! Initialize StartAndCleanup static variable on object load
  extern StartupAndCleanup g_oStartupAndCleanup;
#endif

/**
 * @brief Default Class to create a Application
 */
class IModule : public IModuleBase
{
public:
  IModule() = default;
  virtual ~IModule();

  /**
   * @brief Initialize static parts of module.
   *        This will be done from Kernel after load
   */
  static void initStatic();

  /**
   * @brief Cleanup static parts of module.
   *        This will be done from Kernel before unload
   */
  static void deinitStatic();

  /**
   * @brief The KernelModule will be overwritten.
   *        The Kernel is now on all modules the same.
   * @param oKernel: Target Kernel to set
   */
  virtual void setKernel(const IKernel& oKernel) override final;

private:
  static CcVector<IModule*>* s_pInstances;  //!< All created instances
};
