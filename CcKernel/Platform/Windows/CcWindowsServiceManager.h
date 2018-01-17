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
 * @subpage   CcWindowsServiceManager
 *
 * @page      CcWindowsServiceManager
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsServiceManager
 **/
#ifndef _CcWindowsServiceManager_H_
#define _CcWindowsServiceManager_H_

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include <winsvc.h>
#include "CcString.h"

class CcWindowsServiceManager {
public: //methods
  CcWindowsServiceManager(const CcString& sServiceName);
  virtual ~CcWindowsServiceManager();

  bool available();
  bool stop();
  bool start();
  bool disable();

private:
  bool connectService(DWORD AccessFlags);
  void disconnectService();

private:
  CcString m_sServiceName;
  SC_HANDLE m_schSCManager;
  SC_HANDLE m_schService;
};

#endif /* _CcWindowsServiceManager_H_ */