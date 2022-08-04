/*
 * This file is part of CcShellApp.
 *
 * CcShellApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcShellApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcShellApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcShellApp
 **/
#pragma once

#include "CcShell.h"
#include "CcArguments.h"
#include "CcVector.h"
#include "Network/CcSocket.h"

class CcShellWorker;

/**
 * @brief CcShellApp implementation
 *        Main class wich is loaded to start Application.
 */
class CcShellApp : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcShellApp();

  /**
   * @brief Destructor
   */
  virtual ~CcShellApp();

  /**
   * @brief Main method
   */
  virtual void run() override;

  CcStatus parseArguments(int iArgs, char** pArgs);

private:
  void runServer();
  void runSsh();
  void runConnect();
  void runSconnect();
  void runLocal();

private:
  CcArguments                 m_oArguments;
  CcVector<CcShellWorker*>    m_oWorker;
  CcSocket                    m_Socket;
};
