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
 * @brief     Class CcRestApiDeviceGpioPin
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "Devices/CcDeviceGpioPin.h"
#include "CcRestApiDevice.h"

class CcRestApiDevices;
class CcJsonNode;

/**
 * @brief CcRestApiDeviceGpioPin implementation
 */
class CcHttpSHARED CcRestApiDeviceGpioPin : public CcRestApiDevice
{
public:
  /**
   * @brief Create gpio pin device with existing device handle
   * @param pParent:        Parent RestApi node
   * @param oDeviceHandle:  Target GpioPin device to use
   */
  CcRestApiDeviceGpioPin(CcRestApiDevices* pParent, const CcDevice& oDeviceHandle);

  /**
   * @brief Create gpio pin device with port and pin number.
   *        Device will be taken from Kernel if exists.
   * @param pParent:  Parent RestApi node
   * @param uiPortNr: Target port on device
   * @param uiPinNr:  Target pin on @p uiPortNr
   */
  CcRestApiDeviceGpioPin(CcRestApiDevices* pParent, uint8 uiPortNr, uint8 uiPinNr);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiDeviceGpioPin();

  virtual bool get(CcHttpWorkData& oData) override;
  virtual bool post(CcHttpWorkData& oData) override;
  virtual CcJsonNode getDeviceNode() override;

  /**
   * @brief Write gpio info to get request
   * @param oData: Http work data
   * @return True if successfully set.
   */
  bool getGpioPinInfo(CcHttpWorkData& oData);

private:
  bool postGpioDeviceInfo(CcHttpWorkData& oData);
private:
  CcDeviceGpioPin m_oDevice;
};
