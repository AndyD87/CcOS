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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRestApiDeviceWlan
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "Devices/CcDeviceWlan.h"
#include "CcRestApiDevice.h"

class CcRestApiDevices;
class CcJsonNode;

/**
 * @brief CcRestApiDeviceWlan implementation
 */
class CcHttpSHARED CcRestApiDeviceWlan : public CcRestApiDevice
{
public:
  /**
   * @brief Constructor
   */
  CcRestApiDeviceWlan(CcRestApiDevices* pParent, const CcDevice& oDeviceHandle);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiDeviceWlan();

  virtual bool get(CcHttpWorkData& oData) override;
  virtual bool post(CcHttpWorkData& oData) override;
  virtual CcJsonNode getDeviceNode() override;

private:
  bool getWlanInfo(CcHttpWorkData& oData);
  bool postWlanInfo(CcHttpWorkData& oData);
private:
  CcDeviceWlan m_oDevice;
};
