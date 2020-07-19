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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IDevice
 */

#ifndef H_CcDevice_H_
#define H_CcDevice_H_

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

  virtual EState getState() const
    { return m_eState; }

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
  EState m_eState = EState::Starting;
};


class CcKernelSHARED CcDeviceHandle : public CcHandle<IDevice>
{
public:
  CcDeviceHandle() = default;
  CcDeviceHandle(IDevice* pDevice) : CcHandle<IDevice>(pDevice)
    {}
  CcDeviceHandle(EDeviceType eType) :
    CcHandle<IDevice>(),
    m_eType(eType)
    {}
  CcDeviceHandle(IDevice* pDevice, EDeviceType eType) :
    CcHandle<IDevice>(pDevice),
    m_eType(eType),
    m_uiId(s_uiId++)
  {}

  void set(IDevice* pDevice, EDeviceType eType)
    { CcHandle<IDevice>::operator =(pDevice); m_eType = eType;}

  CcDeviceHandle& operator=(IDevice* pDevice)
  { CcHandle<IDevice>::operator =(pDevice); return *this;}

  EDeviceType getType() const
    { return m_eType; }
  uint32 getId() const
    { return m_uiId; }

  template <class TYPE>
  TYPE* getDevice()
    { return static_cast<TYPE*>(ptr()); }
  const CcString& getTypeString()
    { return getTypeString(m_eType); }
  static const CcString& getTypeString(EDeviceType eType);
  static EDeviceType getTypeFromString(const CcString& sType, bool* bOk = nullptr);
private:
  EDeviceType   m_eType   = EDeviceType::All;
  uint32        m_uiId    = 0;
  static        uint32 s_uiId;
  CcConstStringClass_H(sAll);
  CcConstStringClass_H(sCpu);
  CcConstStringClass_H(sUart);
  CcConstStringClass_H(sSpi);
  CcConstStringClass_H(sI2C);
  CcConstStringClass_H(sDisplay);
  CcConstStringClass_H(sTouchPanel);
  CcConstStringClass_H(sNetwork);
  CcConstStringClass_H(sTimer);
  CcConstStringClass_H(sCamera);
  CcConstStringClass_H(sLed);
  CcConstStringClass_H(sHdd);
  CcConstStringClass_H(sGpioPort);
  CcConstStringClass_H(sGpioPin);
  CcConstStringClass_H(sWlan);
  CcConstStringClass_H(sWlanAccessPoint);
  CcConstStringClass_H(sWlanClient);
  CcConstStringClass_H(sEeprom);
  CcConstStringClass_H(sClock);
};


#endif // H_CcDevice_H_
