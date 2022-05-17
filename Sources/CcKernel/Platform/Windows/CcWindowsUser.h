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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsUser
 */
#pragma once

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "CcUser.h"
#include "CcString.h"

/**
 * @brief Example Class implementation
 */
class CcWindowsUser :public CcUser{
public:
  /**
   * @brief Constructor
   */
  CcWindowsUser(const CcString& Username);

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsUser();

  /**
   * @brief Login to system user on windows
   * @param Password: Password to login
   */
  virtual bool login(const CcPassword& Password) override;

  /**
   * @brief Change home directory.
   * @param sHomeDir: New home directory
   */
  void setWindowsHomeDir(const CcString& sHomeDir);

  /**
   * @brief Change password for user.
   * @param sPassword: new passowrd to set.
   * @todo Channge parameter to CcPassword for more safety
   */
  void setWindowsPassword(const CcString& sPassword);
private:
  CcString m_sHomeDir;
  CcString m_sPassword;
};
