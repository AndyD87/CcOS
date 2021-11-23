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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsDesktopScreenDevice
 */
#ifndef H_CcWindowsDesktopScreenDevice_H_
#define H_CcWindowsDesktopScreenDevice_H_

#include "CcWindowsDesktopScreen.h"
#include "Devices/ICamera.h"
#include "Platform/Windows/CcWindowsGlobals.h"
#include "windowsx.h"

class CcByteArray;

/**
 * @brief To get a view of windows desktop us it like a
 *        camera module, so for example it's possible to
 *        capture a screenshot
 */
class CcWindowsDesktopScreenDevice : public ICamera
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsDesktopScreenDevice();

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsDesktopScreenDevice();

  /**
   * @brief Get a screenshot from Desktop.
   * @return Picture stored in CcImage
   */
  CcImageData getImage() override;

private:
  void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

  /**
   * @brief Create Header Infromation of Bitmatp
   */
  PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);

};

#endif // H_CcWindowsDesktopScreenDevice_H_
