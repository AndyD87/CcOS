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

#ifndef _CcDevice_H_
#define _CcDevice_H_

#include "CcBase.h"
#include "CcKernelBase.h"
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
  Network,    //!< Network-Device
  Timer,       //!< Timer-Device
  Camera,      //!< Camera-Modul as Device
  Led,         //!< single LED-Device.
  Hdd,          //!< Device is a Hard Disk Drive
  GPIOPin,     //!< Get a Device connected with a range of GPIO-Pins
  GPIOPort     //!< Get a Device connected with a range of GPIO-Pins
};

enum class EDeviceState
{
  Starting = 0,
  Running,
  Stopping,
  Off,
};

/**
 * @brief Basic Class for all Devices in System.
 */
class CcKernelSHARED IDevice : public CcObject
{
public:
  /**
   * @brief Constructor
   */
  IDevice() = default;

  /**
   * @brief Destructor
   */
  virtual ~IDevice()
  {
    if (m_eState < EDeviceState::Stopping)
      setState(EDeviceState::Off);
  };

  virtual EDeviceState getState() const
    { return m_eState; }
  
  virtual void setState(EDeviceState eState)
    { m_eState = eState; }

  static const CcString& getString(EDeviceType eType);
protected:
  EDeviceState m_eState = EDeviceState::Starting;

private:
  static const CcString sAll;
  static const CcString sCpu;
  static const CcString sUart;
  static const CcString sSpi;
  static const CcString sI2C;
  static const CcString sDisplay;
  static const CcString sTouchPanel;
  static const CcString sNetwork;
  static const CcString sTimer;
  static const CcString sCamera;
  static const CcString sLed;
  static const CcString sHdd;
  static const CcString sGPIOPort;
  static const CcString sGPIOPin;
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
    m_eType(eType)
    {}
  void set(IDevice* pDevice, EDeviceType eType)
    { CcHandle<IDevice>::operator =(pDevice); m_eType = eType;}

  CcDeviceHandle& operator=(IDevice* pDevice)
  { CcHandle<IDevice>::operator =(pDevice); return *this;}

  const EDeviceType& getType()
    { return m_eType; }
  uint64 getId() const
    { return reinterpret_cast<uint64>(ptr()); }
private:
  EDeviceType m_eType = EDeviceType::All;
};


#endif /* _CcDevice_H_ */
