/**
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @brief     Implementation of Class CcGenericFilesystem
 */
#include "CcGenericFilesystem.h"
#include "CcGenericFile.h"
#include "CcGenericDirectory.h"

CcGenericFilesystem::CcGenericFilesystem()
{
}

CcGenericFilesystem::~CcGenericFilesystem()
{
}

CcFilePointer CcGenericFilesystem::getFile(const CcString& ) const
{
  CcFilePointer file;
  CCNEW(file, CcGenericDirectory);
  return file;
}

CcStatus CcGenericFilesystem::mkdir(const CcString &Path) const
{
  CcStatus oResult(false);
  CCUNUSED(Path);
  return oResult;
}

CcStatus CcGenericFilesystem::remove(const CcString &Path) const
{
  CcStatus oResult(false);
  CCUNUSED(Path);
  return oResult;
}
