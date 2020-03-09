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
 * @page      CcWindowsDesktopScreenDevice
 * @subpage   CcWindowsDesktopScreenDriver
 *
 * @page      CcWindowsDesktopScreenDriver
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsDesktopScreenDriver
 */
#ifndef H_CcWindowsDesktopScreenDriver_H_
#define H_CcWindowsDesktopScreenDriver_H_

#include "CcWindowsDesktopScreen.h"
#include "IDriver.h"

class CcByteArray;
class ICamera;
class IKernel;

/**
 * @brief To get a view of windows desktop us it like a
 *        camera module, so for example it's possible to
 *        capture a screenshot
 */
class CcWindowsDesktopScreenDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsDesktopScreenDriver(IKernel* pKernel);

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsDesktopScreenDriver();

  CcStatus entry() override;
  CcStatus unload() override;

private:
  ICamera* m_DesktopScreen;
  IKernel* m_pKernel;
};

#endif // H_CcWindowsDesktopScreenDriver_H_
