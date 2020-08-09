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
 * @subpage   IModule
 *
 * @page      IModule
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IModule
 */
#ifndef H_IModule_H_
#define H_IModule_H_

#include "CcBase.h"
#include "CcString.h"
#include "IKernel.h"
#include "CcEventHandler.h"
#include "CcVector.h"

class IModule;

typedef IModule* (*IModule_CreateFunction)(const IKernel& oKernel);
typedef void (*IModule_RemoveFunction)(IModule*);

#define IModule_CreateFunctionName "IModule_Create"
#define IModule_RemoveFunctionName "IModule_Remove"

/**
 * @brief Default Class to create a Application
 */
class CcKernelSHARED IModule
{
public:
  IModule(const IKernel& oKernel);
  virtual ~IModule();
  virtual CcStatus init() = 0;
  virtual CcStatus deinit() = 0;
  void registerOnUnload(const CcEvent& oUnloadEvent);
  void unregisterOnUnload(CcObject* pUnregister);

  static void initStatic();
  static void deinitStatic();
protected:
  IKernel         m_oKernel;    //!< Kernel object with new/delete and all drivers and devices
  CcEventHandler  m_oUnloadEvent;
private:
  static void unload();
  static CcVector<IModule*>* s_pInstances;  //!< All created instances
};

#endif // H_IModule_H_
