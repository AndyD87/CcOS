/*
 * This file is part of CcConvert.
 *
 * CcConvert is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConvert is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConvert.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConvert
 **/
#pragma once

#include "CcArguments.h"
#include "CcApp.h"

/**
 * @brief CcConvert implementation
 *        Main class wich is loaded to start Application.
 */
class CcConvert : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcConvert();

  /**
   * @brief Destructor
   */
  virtual ~CcConvert();

  /**
   * @brief Main method
   */
  virtual void run() override;

  CcStatus parseArguments(int iArgs, char** pArgs);

  void writeHelp();

private:
  CcArguments           m_oArguments;
};
