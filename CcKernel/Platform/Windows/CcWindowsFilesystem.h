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
 * @subpage   CcWindowsFilesystem
 *
 * @page      CcWindowsFilesystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsFilesystem
 */
#ifndef H_CcWindowsFilesystem_H_
#define H_CcWindowsFilesystem_H_

#include "CcBase.h"
#include "IFileSystem.h"
#include "CcFile.h"

/**
 * @brief Button for GUI Applications
 */
class CcWindowsFilesystem : public IFileSystem
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsFilesystem();

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsFilesystem();

  CcFilePointer getFile(const CcString& path) const override;
  CcStatus mkdir(const CcString& Path) const override;
  CcStatus remove(const CcString& Path) const override;
};

#endif // H_CcWindowsFilesystem_H_
