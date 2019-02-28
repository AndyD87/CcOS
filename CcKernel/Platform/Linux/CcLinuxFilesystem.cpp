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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxFilesystem
 */
#include "CcLinuxFilesystem.h"
#include "CcLinuxFile.h"
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define DEFAULT_DIR_ACCESS_MASK S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH

CcLinuxFilesystem::CcLinuxFilesystem()
{
}

CcLinuxFilesystem::~CcLinuxFilesystem()
{
}

CcFilePointer CcLinuxFilesystem::getFile(const CcString& path) const
{
  CcFilePointer file = new CcLinuxFile(path);
  return file;
}

CcStatus CcLinuxFilesystem::mkdir(const CcString &Path) const
{
  CcStatus oResult;
  if( 0 != ::mkdir(  Path.getCharString(), DEFAULT_DIR_ACCESS_MASK ) )
  {
    switch(errno)
    {
      case EEXIST:
        oResult = EStatus::FSDirAlreadyExists;
        break;
      default:
        oResult.setSystemError(errno);
    }
    CCDEBUG("Creating sub dir failed: " + Path);
  }
  return oResult;
}

CcStatus CcLinuxFilesystem::remove(const CcString &Path) const
{
  if (CcLinuxFile(Path).isFile()){
    if (::remove(Path.getCharString()) == 0)
      return true;
  }
  else{
    if (::rmdir(Path.getCharString()) == 0)
      return true;
  }
  return false;
}
