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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDevice
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"

/**
 * @brief Create a device handle which can receive events from device if required.
 *        It can handle different type of devices.
 */
class CcKernelSHARED CcDevice : public CcHandle<IDevice>
{
public:
  //! Empty device
  CcDevice() = default;
  /**
   * @brief Create a device with existing interface but without known type.
   * @param pDevice: Target device interface for this handle
   */
  CcDevice(IDevice* pDevice) : CcHandle<IDevice>(pDevice)
    {}

  /**
   * @brief Create a handle by type, but fill device later.
   * @param eType: Type of device to set
   */
  CcDevice(EDeviceType eType) :
    CcHandle<IDevice>(),
    m_eType(eType)
    {}

  /**
   * @brief Create full device handle with device interface and type.
   * @param pDevice:  Target device for handl
   * @param eType:    Type of device
   */
  CcDevice(IDevice* pDevice, EDeviceType eType) :
    CcHandle<IDevice>(pDevice),
    m_eType(eType),
    m_uiId(s_uiId++)
  {}

  /**
   * @brief Set full device handle with device interface and type.
   * @param pDevice:  Target device for handl
   * @param eType:    Type of device
   */
  void set(IDevice* pDevice, EDeviceType eType)
  { CcHandle<IDevice>::operator =(pDevice); m_eType = eType;}

  /**
   * @brief Assign new device interface to handl
   * @param pDevice: Next device interface to set
   * @return reference to this.
   */
  CcDevice& operator=(IDevice* pDevice)
  { CcHandle<IDevice>::operator =(pDevice); return *this;}
  //! @return Get type of current handle
  EDeviceType getType() const
  { return m_eType; }
  //! @return Get current device id
  uint32 getId() const
  { return m_uiId; }
  //! @return Get casted device as pointer
  template <class TYPE>
  TYPE* getDevice() const
  { return static_cast<TYPE*>(ptr()); }
  //! @return Get type of this device as string.
  const CcString& getTypeString()
  { return getTypeString(m_eType); }

  /**
   * @brief Get a known type as string
   *        The string could be used with @ref getTypeString to get enum back.
   * @param eType: Type to generate string for
   * @return String of known type
   */
  static const CcString& getTypeString(EDeviceType eType);

  /**
   * @brief Translate device type string into it's enum pendant.
   *        The string could be created with @ref getTypeString.
   * @param sType
   * @param bOk
   * @return
   */
  static EDeviceType getTypeFromString(const CcString& sType, bool* bOk = nullptr);

public:
  static CcDevice NullDevice;   //!< Default nullptr device to avoid crashes on access.
private:
  EDeviceType   m_eType   = EDeviceType::Unknown;
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
  CcConstStringClass_H(sUsb);
  CcConstStringClass_H(sBoardSupport);
};
