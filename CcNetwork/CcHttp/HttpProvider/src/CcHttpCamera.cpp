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
 * @brief     Implementation of Class CcHttpCamera
 */
#include "HttpProvider/CcHttpCamera.h"
#include "CcHttpGlobals.h"
#include "CcImage.h"

CcHttpCamera::CcHttpCamera(CcHandle<CcCamera> Camera):
  m_Camera(Camera)
{
}

CcHttpCamera::~CcHttpCamera( void )
{
}

CcHttpResponse CcHttpCamera::execGet(CcHttpRequest &Data)
{
  // @todo parse data if for example image format or anything else is required
  CCUNUSED(Data);
  // Use an default Header by passing true to Header;
  CcHttpResponse caRet(true);
  CcImage oCamImage;
  if (m_Camera != nullptr)
  {
    // Get last Picture from Camera
    oCamImage.fillBuffer(m_Camera->getImageRaw(), m_Camera->getImageType());
    // Strore it to send-buffer
    caRet.Content = oCamImage.getImageBuffer();
    // Set correct Mime-Type
    switch (oCamImage.getType())
    {
      case EImageType::Jpeg:
        caRet.m_Header.ContentType = CcHttpGlobals::MIME_IMAGE_JPEG;
        break;
      case EImageType::Png:
        caRet.m_Header.ContentType = CcHttpGlobals::MIME_IMAGE_PNG;
        break;
      case EImageType::Gif:
        caRet.m_Header.ContentType = CcHttpGlobals::MIME_IMAGE_GIF;
        break;
      case EImageType::Bmp:
        caRet.m_Header.ContentType = CcHttpGlobals::MIME_IMAGE_BMP;
        break;
      case EImageType::Unknown:
      case EImageType::Raw:
      default:
        break;
    }
  }
  return caRet;
}

CcHttpResponse CcHttpCamera::execPost(CcHttpRequest &Data)
{
  return execGet(Data);
}
