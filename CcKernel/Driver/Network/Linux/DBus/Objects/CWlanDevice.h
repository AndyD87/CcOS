/*
 *
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
 * @page      Linux
 * @subpage   CWlanDevice
 *
 * @page      CWlanDevice
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CWlanDevice
 */

#ifndef H_CWlanDevice_H_
#define H_CWlanDevice_H_

#include "../ILinuxDbus.h"
#include "Devices/IWlan.h"

class CcStringList;

namespace NLinuxDbus
{
class CNetworkManager;

class CWlanDevice : public IWlan
{
public:
  CWlanDevice(CNetworkManager* pNetworkManager, const CcString& sPath);
  virtual ~CWlanDevice();

  virtual IWlanAccessPoint* getAccessPoint() override;
  virtual IWlanClient* getClient() override;
  virtual CCapabilities getCapabilities() override;
  CcStringList getAccessPoints();

private:
  class CPrivate;
  CPrivate* m_pPrivate;
};
}
#endif // H_CWlanDevice_H_