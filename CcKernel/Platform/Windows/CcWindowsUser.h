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
 * @page      Windows
 * @subpage   CcWindowsUser
 *
 * @page      CcWindowsUser
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsUser
 */
#ifndef H_CcWindowsUser_H_
#define H_CcWindowsUser_H_

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

  bool login(const CcPassword& Password) override;

  void setWindowsHomeDir(const CcString& sHomeDir);
  void setWindowsPassword(const CcString& sPassword);
private:
  CcString m_sHomeDir;
  CcString m_sPassword;
};

#endif // H_CcWindowsUser_H_
