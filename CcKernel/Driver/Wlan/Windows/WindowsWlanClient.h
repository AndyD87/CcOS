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
 * @page      Windows
 * @subpage   WindowsWlanClient
 *
 * @page      WindowsWlanClient
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class WindowsWlanClient
 */
#ifndef H_WindowsWlanClient_H_
#define H_WindowsWlanClient_H_

#include "CcBase.h"
#include "WindowsWlan.h"
#include "Devices/IWlanClient.h"

class WindowsWlanClient : public IWlanClient
{
public: //methods
  WindowsWlanClient(size_t uiNr);
  virtual ~WindowsWlanClient();

  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;
  virtual CcStatus login(const CcString& sSsid, const CcString& sPassord) override;

  CcStatus init(void* hWlan);

  static size_t getAdapters(void* hWlan);
private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
  size_t    m_uiNr;
};

#endif // H_WindowsWlanClient_H_
