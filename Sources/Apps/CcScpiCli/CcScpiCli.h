/*
 * This file is part of CcScpiCli.
 *
 * CcScpiCli is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcScpiCli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcScpiCli.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcScpiCli
 **/
#pragma once

#include "CcScpi.h"
#include "CcArguments.h"
#include "CcApp.h"

/**
 * @brief CcScpiCli implementation
 *        Main class wich is loaded to start Application.
 */
class CcScpiCli : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcScpiCli();

  /**
   * @brief Destructor
   */
  virtual ~CcScpiCli();

  /**
   * @brief Main method
   */
  virtual void run() override;

  CcStatus parseArguments(int iArgs, char** pArgs);

  void writeHelp();

private:
  CcArguments           m_oArguments;
};
