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
 * @page      CcWindowsDesktopScreen
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindowsDesktopScreen
 */
#ifndef CcWindowsDesktopScreen_H_
#define CcWindowsDesktopScreen_H_

#include "CcWindowsDesktopScreen.h"
#include "Platform/Windows/WindowsGlobals.h"
#include "windowsx.h"
#include "Devices/CcCamera.h"

class CcByteArray;

/**
 * @brief To get a view of windows desktop us it like a
 *        camera module, so for example it's possible to
 *        capture a screenshot
 */
class CcWindowsDesktopScreen : public CcCamera 
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsDesktopScreen( void );

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsDesktopScreen( void );

  /**
   * @brief Get a screenshot from Desktop.
   * @return Picture stored in CcImage
   */
  CcByteArray getImage(void) override;

  /**
   * @brief Get a screenshot from Desktop.
   * @return Picture stored in CcImage
   */
  EImageType getImageType(void) override
  {
    return EImageType::Bmp;
  };

private:
  void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

  /**
   * @brief Create Header Infromation of Bitmatp
   */
  PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);

};

#endif /* CcWindowsDesktopScreen_H_ */
