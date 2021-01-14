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
#ifndef H_CcRestApiDeviceGpioPin_H_
#define H_CcRestApiDeviceGpioPin_H_

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
   * @brief Constructor
   */
  CcRestApiDeviceGpioPin(CcRestApiDevices* pParent, const CcDevice& oDeviceHandle);
  CcRestApiDeviceGpioPin(CcRestApiDevices* pParent, uint8 uiPortNr, uint8 uiPinNr);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiDeviceGpioPin();

  virtual bool get(CcHttpWorkData& oData) override;
  virtual bool post(CcHttpWorkData& oData) override;
  bool getGpioPinInfo(CcHttpWorkData& oData);
  virtual CcJsonNode getDeviceNode() override;

private:
  bool postGpioDeviceInfo(CcHttpWorkData& oData);
private:
  CcDeviceGpioPin m_oDevice;
};

#endif // H_CcRestApiDeviceGpioPin_H_
