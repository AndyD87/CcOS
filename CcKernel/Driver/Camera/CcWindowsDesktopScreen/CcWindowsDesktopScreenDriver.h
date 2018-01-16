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
 * @subpage   CcWindowsDesktopScreenDriver
 *
 * @page      CcWindowsDesktopScreenDriver
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsDesktopScreenDriver
 */
#ifndef _CcWindowsDesktopScreenDriver_H_
#define _CcWindowsDesktopScreenDriver_H_

#include "CcDriver.h"
#include "CcWindowsDesktopScreen.h"

class CcByteArray;

/**
 * @brief To get a view of windows desktop us it like a
 *        camera module, so for example it's possible to
 *        capture a screenshot
 */
class CcWindowsDesktopScreenDriver : public CcDriver 
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsDesktopScreenDriver( void );

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsDesktopScreenDriver( void );

  CcStatus entry() override;

private:
  CcHandle<CcCamera> m_DesktopScreen;
};

#endif /* _CcWindowsDesktopScreenDriver_H_ */
