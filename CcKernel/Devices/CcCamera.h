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
 * @page      CcCamera
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcCamera
 */
#ifndef CCCAMERA_H_
#define CCCAMERA_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcDevice.h"
#include "Types/CcImageData.h"
#include "Types/CcByteArray.h"

class CcCamera;

#ifdef WIN32
template class CcKernelSHARED CcHandle<CcCamera>;
#endif

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcCamera : public CcDevice
{
public:
  /**
   * @brief Constructor
   */
  CcCamera(void);

  /**
   * @brief Destructor
   */
  virtual ~CcCamera(void);

  virtual CcByteArray getImage(void) = 0;
  virtual EImageType getImageType(void) = 0;

  virtual CcImageData getImageData(void);
};

#endif /* CCCAMERA_H_ */
