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
#ifndef H_CcDevice_H_
#define H_CcDevice_H_

#include "CcBase.h"
#include "IDevice.h"

class CcKernelSHARED CcDevice : public CcHandle<IDevice>
{
public:
  CcDevice() = default;
  CcDevice(IDevice* pDevice) : CcHandle<IDevice>(pDevice)
    {}
  CcDevice(EDeviceType eType) :
    CcHandle<IDevice>(),
    m_eType(eType)
    {}
  CcDevice(IDevice* pDevice, EDeviceType eType) :
    CcHandle<IDevice>(pDevice),
    m_eType(eType),
    m_uiId(s_uiId++)
  {}

  void set(IDevice* pDevice, EDeviceType eType)
  { CcHandle<IDevice>::operator =(pDevice); m_eType = eType;}

  CcDevice& operator=(IDevice* pDevice)
  { CcHandle<IDevice>::operator =(pDevice); return *this;}

  EDeviceType getType() const
  { return m_eType; }
  uint32 getId() const
  { return m_uiId; }

  template <class TYPE>
  TYPE* getDevice() const
  { return static_cast<TYPE*>(ptr()); }
  const CcString& getTypeString()
  { return getTypeString(m_eType); }
  static const CcString& getTypeString(EDeviceType eType);
  static EDeviceType getTypeFromString(const CcString& sType, bool* bOk = nullptr);

public:
  static CcDevice NullDevice;
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
  CcConstStringClass_H(sUsb);
};
#endif // _CcDevice_H_
