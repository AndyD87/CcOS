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
 *
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class CcGenericFilesystem
 */
#pragma once

#include "CcBase.h"
#include "IFileSystem.h"
#include "CcFile.h"

/**
 * @brief Button for GUI Applications
 */
class CcGenericFilesystem : public IFileSystem{
public:
  /**
   * @brief Constructor
   */
  CcGenericFilesystem();

  /**
   * @brief Destructor
   */
  virtual ~CcGenericFilesystem();

  CcFilePointer getFile(const CcString& path) const override;
  CcStatus mkdir(const CcString& Path) const override;
  CcStatus remove(const CcString& Path) const override;
};
