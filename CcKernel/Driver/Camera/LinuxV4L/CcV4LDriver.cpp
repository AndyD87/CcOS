/*
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
 * @brief     Implementation of Class CcV4LDriver
 */
#include "CcV4LDriver.h"
#include "CcString.h"
#include "CcStatic.h"
#include "CcFile.h"
#include "CcV4LCamera.h"
#include "IKernel.h"
#include "CcKernel.h"
#include "CcDevice.h"

CcV4LDriver::CcV4LDriver()
{
}

CcV4LDriver::~CcV4LDriver()
{
}

CcStatus CcV4LDriver::entry()
{
  CcStatus oStatus(false);
  ICamera* pCamera = nullptr;
  size_t uiIndex = 0;
  do
  {
    CcString sPath("/dev/video");
    sPath.appendSize(uiIndex);
    uiIndex++;
    if(CcFile::exists(sPath))
    {
      CCNEWTYPE(pCamera, CcV4LCamera, sPath);
      CcKernel::addDevice(CcDevice(pCamera, EDeviceType::Camera));
      m_pCameras.append(pCamera);
    }
  } while(pCamera);
  return oStatus;
}

CcStatus CcV4LDriver::unload()
{
  for(ICamera* pCamera : m_pCameras)
  {
    CCDELETE(pCamera);
  }
  m_pCameras.clear();
  return true;
}
