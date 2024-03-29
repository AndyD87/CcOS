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
 * @par       Web:
 * @par       Language: C++11
 * @brief     Class GenericApp
 *
 *  Example GUI-Application with Menue Structure and Application Loader
 */
#pragma once

#include "CcBase.h"
#include "CcService.h"
#include "Devices/USB/Client/CcUsbCdc.h"
#include "Devices/USB/Client/CcUsbBulkTransfer.h"

class IShell;
class IDisplay;
class CcGenericConsole;

class GenericApp: public CcApp
{
public:
  GenericApp();
  virtual ~GenericApp();

  virtual void run() override;
private:
  void onShellStateChanged(void* pVoid);

private:
  CcUsbCdc* m_pCdcDevice              = nullptr;
  CcUsbBulkTransfer* m_pBulkTransfer  = nullptr;
  IShell*   m_pShell                  = nullptr;
  IDisplay*         m_pDisplay        = nullptr;
  CcGenericConsole* m_pConsole        = nullptr;
};
