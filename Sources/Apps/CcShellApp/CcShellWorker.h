/*
 * This file is part of CcShellWorker.
 *
 * CcShellWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcShellWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcShellWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcShellWorker
 **/
#pragma once

#include "CcShell.h"
#include "IIo.h"

class CcShellApp;

/**
 * @brief CcShellWorker implementation
 *        Main class wich is loaded to start Application.
 */
class CcShellWorker : public CcShell
{
public:
  /**
   * @brief Constructor
   */
  CcShellWorker(CcShellApp* pApplication, IIo* pIoStream, bool bRaw = false);

  /**
   * @brief Destructor
   */
  virtual ~CcShellWorker();

  virtual void run() override;

private:
  CcShellApp* m_pApplication;
  IIo*        m_pIoStream;
  bool        m_bRaw;
};
