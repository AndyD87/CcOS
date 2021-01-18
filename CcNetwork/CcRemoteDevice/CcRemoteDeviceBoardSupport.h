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
#pragma once

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

  /**
   * @brief Create GPIO pin for remote access
   */
  class CcRemoteDeviceSHARED CPortPin
  {
  public:
    CPortPin()=default;
    CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CPortPin)

    /**
     * @brief Create Pin and create a name for remote access.
     * @param pcName:     Name for external access.
     * @param uiPort:     Internal port.
     * @param uiPin:      Pin of @ref uiPort.
     * @param eDirection: Default direction of port.
     */
    CPortPin(const char* pcName, uint8 uiPort, uint8 uiPin, IGpioPin::EDirection eDirection) :
      m_pcName(pcName),
      m_uiPort(uiPort),
      m_uiPin(uiPin),
      m_eDirection(eDirection)
    {}

    /**
     * @brief Copy pin informations from another PortPin.
     * @param oToCopy: Object to copy from.
     * @return Reference to this.
     */
    CPortPin& operator=(const CPortPin& oToCopy)
    {
      m_pcName=oToCopy.m_pcName;
      m_uiPort=oToCopy.m_uiPort;
      m_uiPin=oToCopy.m_uiPin;
      m_eDirection=oToCopy.m_eDirection;
      return *this;
    }

    /**
     * @brief Compare this port pin with another one.
     * @param oToCompare: Object to compare to.
     * @return True if both port, pin and direction are same.
     */
    bool operator==(const CPortPin& oToCompare) const
    {
      return  m_uiPort == oToCompare.m_uiPort &&
              m_uiPin == oToCompare.m_uiPin &&
              m_eDirection == oToCompare.m_eDirection;
    }

    //! @return Get name of port pin
    const char* getName() const
    { return m_pcName; }
    //! @return Get internal port
    uint8 getPort() const
    { return m_uiPort; }
    //! @return Get pin of port
    uint8 getPin() const
    { return m_uiPin; }
    //! @return Get direction of this pin
    IGpioPin::EDirection getDirection() const
    { return m_eDirection; }
  private:
    const char* m_pcName = nullptr;
    uint8 m_uiPort       = 0;
    uint8 m_uiPin        = 0;
    IGpioPin::EDirection m_eDirection = IGpioPin::EDirection::Input;
  };

  /**
   * @brief List of port pins
   */
  class CcRemoteDeviceSHARED CPortPinArray : public CcVector<CPortPin>
  {
  };

  /**
   * @brief Add device to supported remote devices
   * @param oHandle: Handle of device
   */
  void addDevice(const CcDevice& oHandle);

  //! @return True if port pins are available
  bool hasGpio();
  //! @return True if lan device for remote device is available
  bool hasLan();
  //! @return True if wlan accesspoint is in supported device list
  bool hasWlanAccessPoint();
  //! @return True if wlan client is in supported device list
  bool hasWlanClient();
  //! @return Get available list of por pins
  const CPortPinArray& getGpioPins() const;

  /**
   * @brief Initialize Board
   */
  void initBoard();
public:
  //! @return Get available list of por pins
  CPortPinArray& getGpioPins();
private:
  class CPrivate;
  CPrivate* m_pPrivate;
};
