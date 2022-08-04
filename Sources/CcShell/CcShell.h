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
 * @brief     Class CcShell
 */
#pragma once

#include "CcShellBase.h"
#include "CcString.h"
#include "IShell.h"
#include "CcFile.h"
#include "CcIoSwitch.h"

class CcShellWorker;

/**
 * @brief Shell application to be started with own script interpreter.
 */
class CcShellSHARED CcShell : public IShell
{
public:
  /**
   * @brief Default shell instance with reading and writin from stdin.
   */
  CcShell(IIo* pIoStream = nullptr);

  /**
   * @brief Destructor
   */
  virtual ~CcShell();

private:
  IIo*                      m_pStreamControl = nullptr;
  CcIoSwitch                m_oIoStream;
};
