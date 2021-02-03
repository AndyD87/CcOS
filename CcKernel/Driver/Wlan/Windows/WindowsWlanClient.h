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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class WindowsWlanClient
 */
#pragma once

#include "CcBase.h"
#include "WindowsWlan.h"
#include "Devices/IWlanClient.h"

/**
 * @brief Wlan Client interface for Windows
 */
class WindowsWlanClient : public IWlanClient
{
public: //methods
  /**
   * @brief Create client for adapter with specific number.
   * @param uiNr: Adapter number
   */
  WindowsWlanClient(size_t uiNr);
  virtual ~WindowsWlanClient();

  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;
  virtual CcStatus login(const CcString& sSsid, const CcString& sPassord) override;

  /**
   * @brief Initialize Client with adapter handle
   * @param hWlan: Adapter handle to target client device
   * @return Status of operation
   */
  CcStatus init(void* hWlan);

  /**
   * @brief Get number of available adapters. A number lower than this is required
   *        for calling the constructor.
   * @param hWlan: Adapter interface handle from windows
   * @return Number of available adapter
   */
  static size_t getAdapters(void* hWlan);
private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
  size_t    m_uiNr;
};
