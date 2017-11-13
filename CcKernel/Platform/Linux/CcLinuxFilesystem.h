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
 * @page      Linux
 * @subpage   CcLinuxFileSystem
 *
 * @page      CcLinuxFileSystem
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLinuxFilesystem
 */
#ifndef CcLinuxFileSYSTEM_H_
#define CcLinuxFileSYSTEM_H_

#include "CcBase.h"
#include "CcFileSystemAbstract.h"
#include "CcFile.h"

/**
 * @brief Button for GUI Applications
 */
class CcLinuxFilesystem : public CcFileSystemAbstract{
public:
  /**
   * @brief Constructor
   */
  CcLinuxFilesystem( void );

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxFilesystem( void );

  CcFilePointer getFile(const CcString &path) const override;
  CcStatus mkdir(const CcString& Path) const override;
  CcStatus remove(const CcString &Path) const override;
};

#endif /* CcLinuxFileSYSTEM_H_ */
