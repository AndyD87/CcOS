/*
 * This file is part of CcOSModuleLoaderApp.
 *
 * CcOSModuleLoaderApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOSModuleLoaderApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOSModuleLoaderApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcOSModuleLoaderApp
 **/
#ifndef H_CcOSModuleLoaderApp_H_
#define H_CcOSModuleLoaderApp_H_

#include "CcApp.h"
#include "CcArguments.h"

/**
 * @brief CcOSModuleLoaderApp implementation
 *        Main class wich is loaded to start Application.
 */
class CcOSModuleLoaderApp : public CcApp
{
public:
  CcOSModuleLoaderApp(int argc, char** argv);

  /**
   * @brief Destructor
   */
  virtual ~CcOSModuleLoaderApp();

  virtual void run() override;
  
private:
  class CPrivate;

  void parseArgs();
  void printHelp();
private:
  CPrivate*             m_pPrivate = nullptr;  //!< Private data for applicaton.
  CcArguments           m_oArguments;
  CcString              m_sFileToLoad;
};

#endif // H_CcOSModuleLoaderApp_H_
