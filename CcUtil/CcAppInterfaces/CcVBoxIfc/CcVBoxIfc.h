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
 * @page      CcVBoxIfc
 * @subpage   CcVBoxIfc
 *
 * @page      CcVBoxIfc
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVBoxIfc
 */
#ifndef _CcVBoxIfc_H_
#define _CcVBoxIfc_H_

#include "CcAppInterfaces.h"
#include "CcString.h"

/**
 * @brief CcVBoxIfc Class impelmentation
 */
class CcAppInterfacesSHARED CcVBoxIfc
{
public:
  /**
   * @brief Constructor
   */
  CcVBoxIfc(const CcString& sPath = "");

  /**
   * @brief Destructor
   */
  ~CcVBoxIfc();

  bool isValid()
    { return m_bExeFound; }

private:
  class CPrivate;
  CcString m_sExePath;
  CcString m_sWorkingDir;
  bool m_bExeFound = false;
};

#endif /* _CcVBoxIfc_H_ */
