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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ICamera
 */
#ifndef H_ICamera_H_
#define H_ICamera_H_

#include "CcBase.h"
#include "CcBase.h"
#include "IDevice.h"
#include "CcImageData.h"

class CcString;
class CcByteArray;
class ICamera;

#ifdef _MSC_VER
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
   * @brief Get Image as with CcImageData container
   * @return Data as CcImageData.
   */
  virtual CcImageData getImage() = 0;

  /**
   * @brief Capture image and write to file.
   *        File will be generated in defined path and name.
   *        The extension will be generated by Camera.
   * @return Type of Images as Enum
   */
  virtual CcString captureTo(const CcString& sPath, const CcString& sName, bool bAutoIncrement = false);
};

#endif // _ICamera_H_
