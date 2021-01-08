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
 * @page      Devices
 * @subpage   CcDeviceCamera
 *
 * @page      CcDeviceCamera
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceCamera
 */
#ifndef H_CcDeviceCamera_H_
#define H_CcDeviceCamera_H_

#include "CcBase.h"
#include "Devices/ICamera.h"
#include "CcDevice.h"

/**
 * @brief Camera class for controling them
 *        and getting pictures.
 */
class CcKernelSHARED CcDeviceCamera : public CcDevice
{
public:
  CcDeviceCamera(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceCamera() = default;

  ICamera* getDevice() const
  { return CcDevice::getDevice<ICamera>(); }

  /**
   * @brief Get Image as with CcImageData container
   * @return Data as CcImageData.
   */
  CcImageData getImage()
  { if(isValid()) return getDevice()->getImage(); return CcImageData(); }

  /**
   * @brief Capture image and write to file.
   *        File will be generated in defined path and name.
   *        The extension will be generated by Camera.
   * @return Type of Images as Enum
   */
  virtual CcString captureTo(const CcString& sPath, const CcString& sName, bool bAutoIncrement = false)
  { if (isValid()) return getDevice()->captureTo(sPath, sName, bAutoIncrement); return CcString(); }
};

#endif // _CcDeviceCamera_H_
