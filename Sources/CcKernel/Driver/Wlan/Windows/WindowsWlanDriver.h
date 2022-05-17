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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */
#pragma once

#include "IDriver.h"
#include "CcBase.h"
#include "IKernel.h"
#include "WindowsWlan.h"

/**
 * @brief Generate SM32F407V CPU Device
 */
class WindowsWlanDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  WindowsWlanDriver();

  /**
   * @brief Destructor
   */
  virtual ~WindowsWlanDriver();
  
  virtual CcStatus entry() override;
  virtual CcStatus unload() override;

  /**
   * @brief Setup client by index of device
   * @param uiNr: Index  of device in system
   * @return Status of operation
   */
  CcStatus setupClient(size_t uiNr);

  //! @return Get handle of wlan devie
  void* getWlanHandle();

  //! @return Get instance of driver.
  static WindowsWlanDriver* getInstance()
    { return g_pInstance; }
 
private: // Types
  class CPrivate;
private:
  CPrivate* m_pPrivate;
  static WindowsWlanDriver* g_pInstance;
};
