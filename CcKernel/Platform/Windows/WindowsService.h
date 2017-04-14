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
 * @page      WindowsService
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsService
 **/
#ifndef WINDOWSSERVICE_H_
#define WINDOWSSERVICE_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include <winsvc.h>
#include "CcString.h"

class WindowsService {
public: //methods
  WindowsService();
  virtual ~WindowsService();

  void init(const CcString& Name);
  static void serviceMain(DWORD argc, LPTSTR *argv);

  SERVICE_STATUS m_State;
};

#endif /* WINDOWSSERVICE_H_ */
