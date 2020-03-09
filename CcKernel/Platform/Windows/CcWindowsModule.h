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
 * @page      Windows
 * @subpage   CcWindowsModule
 *
 * @page      CcWindowsModule
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsModule
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef H_CcWindowsModule_H_
#define H_CcWindowsModule_H_

#include "CcBase.h"
#include "IModule.h"

/**
 * @brief Implementation of Shared Memory within Linux Systems
 */
class CcWindowsModule
{
public:
  CcStatus loadModule(const CcString& sName);
  CcStatus unloadModule();
  void resetHandles();

  inline IModule* getModule()
  { return m_pModule; }

private: // Methos
  static void marker();
private: // Member
  IModule* m_pModule = nullptr;
  IModule_CreateFunction m_pCreate;
  IModule_RemoveFunction m_pRemove;
  void*    m_pInstance = nullptr;

};

#endif // H_CcWindowsModule_H_
