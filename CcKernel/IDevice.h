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
 * @brief     Class IDevice
 */
#pragma once

#include "CcBase.h"
#include "Types/CcHandle.h"
#include "CcObject.h"
#include "CcString.h"

/**
 * @brief Enumeration of Known Devices.
 */
enum class EDeviceType
{
  Unknown = 0, //!< this defines All Devices if requesting for.
  All = 0,     //!< this defines All Devices if requesting for.
  Cpu,         //!< CPU with at least one core with ability to start and stop threads
  Uart,        //!< Universal Asynchronous Receivce Transmit Device.
  Spi,         //!< Serial Protocol Interface Device
  I2C,         //!< I2C-Device
  Display,     //!< LCD-Device
  TouchPanel,  //!< TouchPanel-Device
  Network,     //!< Network-Device
  Timer,       //!< Timer-Device
  Camera,      //!< Camera-Modul as Device
  Led,         //!< single LED-Device.
  Hdd,         //!< Device is a Hard Disk Drive
  GpioPin,     //!< Get a Device connected with a range of Gpio-Pins
  GpioPort,    //!< Get a Device connected with a range of Gpio-Pins
  Wlan,             //!< Wireles network device wich can be connected as Client and/or Accesspoint
  WlanAccessPoint,  //!< Generated AccessPoint from IWlan
  WlanClient,       //!< Generated Client from IWlan
  Eeprom,           //!< Eeprom or flash device with device informations.
  Clock,            //!< Clock generator
  Usb,              //!< Usb Device
  BoardSupport      //!< Board support device to enable or disable Functions and Ports
};

/**
 * @brief Basic Class for all Devices in System.
 */
class CcKernelSHARED IDevice : public CcObject
{
public:
  /**
   * @brief Current device state as enumeration.
   */
  enum class EState
  {
    Stopped = 0,
    Start,
    Starting,
    Run,
    Running,
    Pause,
    Paused,
    Stop,
    Stopping
  };

  IDevice() = default;

  /**
   * @brief Destructor
   *        Every derived device has to stop it self before destructor is called.
   */
  virtual ~IDevice() = default;

  /**
   * @brief This method can be overloaded wind with registerIdle and deregisterIdle
   * @todo Move this to an idle enabled class.
   */
  virtual void idle(){}

  //! @return Get current state of device. This can be overloaded from device.
  EState getState() const
  { return m_eState; }

  /**
   * @brief Change current state of device, to start, stop or pause them for example;
   *        If current device state is not matching, onState will be called until State is reached.
   * @param eState: New state as enum to set
   * @return Status value for reporting success or known error.
   */
  CcStatus setState(EState eState);

  /**
   * @brief On state method will be called on every changed state.
   *        It can be handled by the derived classed if action is needed.
   * @param eState: New state of device
   * @return Success if state was set successfully, if error occured device will be called with EState::Stop.
   */
  virtual CcStatus onState(EState eState) { CCUNUSED(eState); return CcStatus(); }

  //! @return True if device state is >EState::Starting
  bool isStarted()
  { return m_eState > EState::Starting;}

  /**
   * @brief Start device and enter EState::Start
   * @return Status of operation
   */
  CcStatus start()
  { return setState(EState::Start); }

  /**
   * @brief Set device to sleep and enter EState::Pause
   * @return Status of operation
   */
  CcStatus pause()
  { return setState(EState::Pause); }

  /**
   * @brief Stop device and enter EState::Stop
   * @return Status of operation
   */
  CcStatus stop()
  { return setState(EState::Stop); }

  /**
   * @brief Execute stop and start on device
   * @return Status of operation
   */
  CcStatus restart();

protected:
  /**
   * @brief This method registers idle() in system and can be removed with deregisterIdle()
   * @todo Move this to an idle enabled class.
   */
  void registerIdle();

  /**
   * @brief This method removes a registered idle() in system.
   * @todo Move this to an idle enabled class.
   */
  void deregisterIdle();
protected:
  EState m_eState = EState::Starting; //!< Current device state as enumeration.
};
