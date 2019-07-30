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
 * @page      Camera
 * @subpage   CcWindowsDesktopScreen
 *
 * @page      CcWindowsDesktopScreen
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsDesktopScreen
 */
#ifndef H_CcWindowsDesktopScreen_H_
#define H_CcWindowsDesktopScreen_H_

#include <Devices/ICamera.h>
#include "CcWindowsDesktopScreen.h"
#include "Platform/Windows/CcWindowsGlobals.h"
#include "windowsx.h"

class CcByteArray;

/**
 * @brief To get a view of windows desktop us it like a
 *        camera module, so for example it's possible to
 *        capture a screenshot
 */
class CcWindowsDesktopScreen : public ICamera 
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsDesktopScreen();

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsDesktopScreen();

  /**
   * @brief Get a screenshot from Desktop.
   * @return Picture stored in CcImage
   */
  CcByteArray getImageRaw() override;

  /**
   * @brief Get a screenshot from Desktop.
   * @return Picture stored in CcImage
   */
  EImageType getImageType() override
    { return EImageType::Bmp; }

private:
  void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

  /**
   * @brief Create Header Infromation of Bitmatp
   */
  PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);

};

#endif /* H_CcWindowsDesktopScreen_H_ */
