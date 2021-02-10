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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpCamera
 */
#pragma once

#include <Devices/ICamera.h>
#include "CcBase.h"
#include "CcHttp.h"
#include "IHttpPathProvider.h"

/**
 * @brief Example Class implementation
 */
class CcHttpSHARED CcHttpCamera : public IHttpPathProvider
{
public:
  /**
   * @brief Constructor
   * @param oCamera: Camera device for working on
   */
  CcHttpCamera(CcHandle<ICamera> oCamera);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpCamera();

  /**
   * @brief Process GET request to capture image from camera
   * @param oData: Workset of http data
   * @return Status of operation.
   */
  virtual CcStatus execGet(CcHttpWorkData& oData) override;

  /**
   * @brief Process POST request to capture image from camera
   * @param oData: Workset of http data
   * @return Status of operation.
   */
  virtual CcStatus execPost(CcHttpWorkData& oData) override;

private:
  CcHandle<ICamera> m_Camera;
};
