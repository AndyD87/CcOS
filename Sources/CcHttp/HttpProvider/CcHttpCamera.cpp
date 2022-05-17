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
 * @brief     Implementation of Class CcHttpCamera
 */
#include "HttpProvider/CcHttpCamera.h"
#include "CcHttpGlobals.h"

CcHttpCamera::CcHttpCamera(CcHandle<ICamera> Camera):
  m_Camera(Camera)
{
}

CcHttpCamera::~CcHttpCamera()
{
}

CcStatus CcHttpCamera::execGet(CcHttpWorkData& oData)
{
  // @todo parse data if for example image format or anything else is required
  CCUNUSED(oData);
  // Use an default Header by passing true to Header;
  CcStatus oSuccess = false;
  if (m_Camera != nullptr)
  {
    oSuccess = false;
    CcImageData oImage = m_Camera->getImage();
    // Strore it to send-buffer
    oData.write(oImage.getBuffer());
    // Set correct Mime-Type
    switch (oImage.getType())
    {
      case EImageType::Jpeg:
        oData.getResponse().setContentType(CcHttpGlobals::MIME_IMAGE_JPEG);
        oSuccess = true;
        break;
      case EImageType::Png:
        oData.getResponse().setContentType(CcHttpGlobals::MIME_IMAGE_PNG);
        oSuccess = true;
        break;
      case EImageType::Gif:
        oData.getResponse().setContentType(CcHttpGlobals::MIME_IMAGE_GIF);
        oSuccess = true;
        break;
      case EImageType::Bmp:
        oData.getResponse().setContentType(CcHttpGlobals::MIME_IMAGE_BMP);
        oSuccess = true;
        break;
      case EImageType::Unknown:
      case EImageType::Raw:
        break;
      default:
        // Unhandled/Not required
        break;
    }
  }
  return oSuccess;
}

CcStatus CcHttpCamera::execPost(CcHttpWorkData& oData)
{
  return execGet(oData);
}
