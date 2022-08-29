/*
 * This file is part of CcSshWorker.
 *
 * CcSshWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcSshWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcSshWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSshWorker
 **/
#pragma once

#include "CcShell.h"
#include "IIo.h"
#include "Ssh/Types.h"

class CcShellApp;

/**
 * @brief CcSshWorker implementation
 *        Main class wich is loaded to start Application.
 */
class CcSshWorker : public CcShell
{
public:
  enum class EState : uint16
  {
    Startup = 0,
    Handshake,
    Error = UINT16_MAX
  };
  /**
   * @brief Constructor
   */
  CcSshWorker(CcShellApp* pApplication, IIo* pIoStream);

  /**
   * @brief Destructor
   */
  virtual ~CcSshWorker();

  virtual void run() override;

  EState getState() const
  { return m_eState; }
  
  void setState(EState eState)
  { m_eState = eState; }

private:
  void execPackage();

private:
  CcShellApp* m_pApplication;
  IIo*        m_pIoStream;
  EState      m_eState = EState::Startup;
  CcByteArray m_oPackage;
};
