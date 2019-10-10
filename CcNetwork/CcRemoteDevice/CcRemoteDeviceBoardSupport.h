/*
 * This file is part of CcRemoteDeviceBoardSupport.
 *
 * CcRemoteDeviceBoardSupport is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceBoardSupport is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceBoardSupport.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcRemoteDeviceBoardSupport
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceBoardSupport
 **/
#ifndef H_CcRemoteDeviceBoardSupport_H_
#define H_CcRemoteDeviceBoardSupport_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "Devices/IGpioPin.h"

/**
 * @brief CcRemoteDeviceBoardSupport implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceBoardSupport
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceBoardSupport();

  /**
   * @brief Destructor
   */
  ~CcRemoteDeviceBoardSupport();

  typedef struct CcRemoteDeviceSHARED
  {
  public:
    const char* pcName;
    uint8 uiPort;
    uint8 uiPin;
    IGpioPin::EDirection eDirection;
  } SPortPin;

  typedef struct CcRemoteDeviceSHARED
  {
    size_t    uiPinCount;
    SPortPin* pPins;
  } SPortPinArray;

  bool hasGpio();
  bool hasLan();
  bool hasWlanAccessPoint();
  bool hasWlanClient();

  const SPortPinArray& getGpioPins() const;

private:
  class CPrivate;
};

#endif // H_CcRemoteDeviceBoardSupport_H_
