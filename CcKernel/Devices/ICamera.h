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
 * @subpage   ICamera
 *
 * @page      ICamera
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ICamera
 */
#ifndef _ICamera_H_
#define _ICamera_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IDevice.h"
#include "Types/CcImageData.h"
#include "Types/CcByteArray.h"

class ICamera;

#ifdef WIN32
template class CcKernelSHARED CcHandle<ICamera>;
#endif

/**
 * @brief Camera class for controling them
 *        and getting pictures.
 */
class CcKernelSHARED ICamera : public IDevice
{
public:
  /**
   * @brief Constructor
   */
  ICamera();

  /**
   * @brief Destructor
   */
  virtual ~ICamera();

  /**
   * @brief Get a raw Image from camera.
   * @return Raw Image as ByteArray
   */
  virtual CcByteArray getImageRaw() = 0;

  /**
   * @brief Get format of Images the camera will make.
   * @return Type of Images as Enum
   */
  virtual EImageType getImageType() = 0;

  /**
   * @brief Get Image as with CcImageData container
   * @return Data as CcImageData.
   */
  virtual CcImageData getImage();
};

#endif /* _ICamera_H_ */
