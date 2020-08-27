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

CcFilePointer CcLinuxFilesystem::getFile(const CcString& sPath) const
{
  CCNEWTYPE(pFile, CcLinuxFile, sPath);
  return pFile;
}

CcStatus CcLinuxFilesystem::mkdir(const CcString& sPath) const
{
  CcStatus oResult;
  if( 0 != ::mkdir(sPath.getCharString(), DEFAULT_DIR_ACCESS_MASK ) )
  {
    switch(errno)
    {
      case EEXIST:
        oResult = EStatus::FSDirAlreadyExists;
        break;
      default:
        oResult.setSystemError(errno);
        CCDEBUG("Creating sub dir failed: " + sPath);
    }
  }
  return oResult;
}

CcStatus CcLinuxFilesystem::remove(const CcString& sPath) const
{
  if (CcLinuxFile(sPath).isFile()){
    if (::remove(sPath.getCharString()) == 0)
      return true;
  }
  else{
    if (::rmdir(sPath.getCharString()) == 0)
      return true;
  }
  return false;
}
