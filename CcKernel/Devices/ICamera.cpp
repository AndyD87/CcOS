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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class ICamera
 */
#include <Devices/ICamera.h>
#include "CcByteArray.h"
#include "CcString.h"
#include "CcDirectory.h"
#include "CcFile.h"
#include "CcFileSystem.h"

ICamera::ICamera()
{
}

ICamera::~ICamera()
{
}

CcString ICamera::captureTo(const CcString& sPath, const CcString& sName, bool bAutoIncrement)
{
  CcString sNewBaseName = sPath;
  if(CcDirectory::exists(sPath))
  {
    CcImageData oData = getImage();
    if(oData.getBuffer().size() > 0)
    {
      CcString sFilename = sName;
      if(bAutoIncrement)
      {
        sFilename = CcFileSystem::getNextFreeFilename(sPath, sFilename, "." + oData.getFileExtension());
      }
      sNewBaseName.appendPath(sFilename);
      sNewBaseName += "." + oData.getFileExtension();
      CcFile oFile(sNewBaseName);
      if(oFile.open(EOpenFlags::Write))
      {
        oFile.writeArray(oData.getBuffer());
        oFile.close();
      }
      else
      {
        sNewBaseName = "";
      }
    }
    else
    {
      sNewBaseName = "";
    }
  }
  else
  {
    sNewBaseName = "";
  }
  return sNewBaseName;
}
