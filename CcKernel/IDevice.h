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
 * @page      CcKernel
 * @subpage   IDevice
 *
 * @page      IDevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IDevice
 */

#ifndef H_IDevice_H_
#define H_IDevice_H_

#include "CcBase.h"
#include "CcBase.h"
#include "Types/CcHandle.h"
#include "CcObject.h"
#include "CcString.h"

/**
 * @brief Enumeration of Known Devices.
 */
enum class EDeviceType
{
  All = 0,     //!< this defines All Devices if requesting for.
  Cpu,
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
  Usb
};

/**
 * @brief Basic Class for all Devices in System.
 */
class CcKernelSHARED IDevice : public CcObject
{
public:
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

  /**
   * @brief Constructor
   */
  IDevice() = default;

  /**
   * @brief Destructor
   *        Every derived device has to stop it self before destructor is called.
   */
  virtual ~IDevice() = default;

  virtual void idle(){}
  virtual EState getState() const
    { return m_eState; }

  /**
   * @brief Change current state of device, to start, stop or pause them for example;
   *    Example Implementations in driver
   *    @code{.cpp}
   *      virtual CcStatus setState(EState eState);
   *      CcStatus Class::setState(EState eState)
   *      {
   *        CcStatus oStatus = false;
   *        switch(eState)
   *        {
   *          case EState::Start:
   *            oStatus = true;
   *            break;
   *          case EState::Pause:
   *            oStatus = true;
   *            break;
   *          case EState::Stop:
   *            oStatus = true;
   *            break;
   *        }
   *        return oStatus;
   *      }
   *    @endcode
   * @param eState: New state as enum to set
   * @return Status value for reporting success or known error.
   */
  virtual CcStatus setState(EState eState);

  bool isStarted()
    { return m_eState > EState::Starting;}

  CcStatus start()
    { return setState(EState::Start); }
  CcStatus pause()
    { return setState(EState::Pause); }
  CcStatus stop()
    { return setState(EState::Stop); }
  CcStatus restart();

protected:
  void registerIdle();
  void deregisterIdle();
protected:
  EState m_eState = EState::Starting;
};

#endif // H_IDevice_H_
