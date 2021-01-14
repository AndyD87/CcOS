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
 * @file
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
#include "CcVector.h"
#include "CcDevice.h"

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

  class CcRemoteDeviceSHARED CPortPin
  {
  public:
    CPortPin()=default;
    CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CPortPin);
    CPortPin(const char* pcName, uint8 uiPort, uint8 uiPin, IGpioPin::EDirection eDirection) :
      pcName(pcName),
      uiPort(uiPort),
      uiPin(uiPin),
      eDirection(eDirection)
    {}

    CPortPin& operator=(const CPortPin& oToCopy)
    {
      pcName=oToCopy.pcName;
      uiPort=oToCopy.uiPort;
      uiPin=oToCopy.uiPin;
      eDirection=oToCopy.eDirection;
      return *this;
    }

    bool operator==(const CPortPin& oToCompare) const
    { return uiPort == oToCompare.uiPort && uiPin == oToCompare.uiPin && eDirection == oToCompare.eDirection; }
    inline bool operator!=(const CPortPin& oToCompare) const
    { return operator==(oToCompare); }

    const char* pcName = nullptr;
    uint8 uiPort       = 0;
    uint8 uiPin        = 0;
    IGpioPin::EDirection eDirection = IGpioPin::EDirection::Input;
  };

  class CcRemoteDeviceSHARED CPortPinArray : public CcVector<CPortPin>
  {
  };

  void addDevice(const CcDevice& oHandle);

  bool hasGpio();
  bool hasLan();
  bool hasWlanAccessPoint();
  bool hasWlanClient();

  const CPortPinArray& getGpioPins() const;

  void initBoard();
public:
  CPortPinArray& getGpioPins();
private:
  class CPrivate;
  CPrivate* m_pPrivate;
};

#endif // H_CcRemoteDeviceBoardSupport_H_
