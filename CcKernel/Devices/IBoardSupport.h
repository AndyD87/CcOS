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
 * @brief     Class IBoardSupport
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "CcPoint.h"
#include "CcArray.h"
#include "CcVector.h"
#include "CcString.h"
#include "CcDevice.h"
#include "CcDeviceList.h"

/**
 * @brief Communication Device for I2C
 */
class CcKernelSHARED IBoardSupport : public IDevice
{
public:
  virtual ~IBoardSupport();

  size_t getDeviceSize() const
  { return m_oHwDevices.size(); }

  /**
   * @brief Get number of Devices available on board of specific type
   * @param eDeviceType: Type of Device to search for
   * @return Number of available Devices
   */
  size_t getDeviceSize(EDeviceType eDeviceType) const;

  virtual CcDevice createDevice(EDeviceType eDeviceType, size_t uiDeviceNumber) = 0;

protected:
  class CcKernelSHARED CHwDevice
  {
  public:
    class CcKernelSHARED CPort
    {
    public:
      CPort() = default;
      CPort(uint16 uiPort, const CcVector<uint16>& oPins) :
        uiPort(uiPort),
        oPins(oPins)
      {}

      uint16 uiPort = UINT16_MAX;
      typedef CcVector<uint16> CPinList;
      #ifdef _MSC_VER
        class CcKernelSHARED CPinList;
      #endif
      CPinList oPins;
    };

    CHwDevice() = default;
    CHwDevice(EDeviceType eDevice,
              size_t      uiDeviceNr,
              const CcVector<CPort>& oRequiredPins,
              const CcVector<CPort>& oOptionalPins = {},
              IDevice*               pDevice = nullptr) :
      eDevice(eDevice),
      uiDeviceNr(uiDeviceNr),
      oRequiredPins(oRequiredPins),
      oOptionalPins(oOptionalPins),
      pDevice(pDevice)
    {}

    EDeviceType     eDevice     = EDeviceType::Unknown;
    size_t          uiDeviceNr  = 0;
    
    typedef CcVector<CPort> CPortList;
    #ifdef _MSC_VER
      class CcKernelSHARED CPortList;
    #endif
    CcVector<CPort> oRequiredPins;
    CcVector<CPort> oOptionalPins;
    IDevice*        pDevice     = nullptr;
  };

  class CcKernelSHARED CHwPin
  {
  public:
    CHwPin() = default;
    CHwPin( uint16 uiSoftwarePort,
            uint16 uiSoftwarePin,
            CcVector<size_t> oSupportedDevices,
            CcString sDescription) :
      oSupportedDevices(oSupportedDevices),
      uiSoftwarePort(uiSoftwarePort),
      uiSoftwarePin(uiSoftwarePin),
      sDescription(sDescription)
    {}

    size_t uiSelectedDeviceNr = SIZE_MAX;           //!< Number of Device in board support list this pin is mapped to, or SIZE_MAX if not.
    typedef CcVector<size_t> CDeviceList;
    #ifdef _MSC_VER
      class CcKernelSHARED CDeviceList;
    #endif
    CDeviceList oSupportedDevices;
    uint16 uiSoftwarePort       = UINT16_MAX;
    uint16 uiSoftwarePin        = UINT16_MAX;
    CcString sDescription;
  };

  class CcKernelSHARED CHwPort
  {
  public:
    CHwPort() = default;
    CHwPort(uint32 uiPort, CcVector<CHwPin> oHwPins) :
      uiPortNr(uiPort),
      oHwPins(oHwPins)
    {}

    CHwPin& getHwPin(uint16 uiPort);

    uint32            uiPortNr = UINT32_MAX;
    typedef CcVector<CHwPin> CPinList;
    #ifdef _MSC_VER
      class CcKernelSHARED CPinList;
    #endif
    CPinList  oHwPins;
  };


  IBoardSupport(const CcVector<CHwDevice>& oDevices, const CcVector<CHwPort>& oPorts) :
    m_oHwDevices(oDevices),
    m_oHwPorts(oPorts)
  {}

  CHwDevice& getHwDevice(EDeviceType eDeviceType, size_t uiDeviceNumber, size_t&uiFunctionNr);
  CHwPort&   getHwPort(uint16 uiPort);
  bool verifyFreePort(const CHwDevice& oHwDevice);
  bool setHwDeviceUsage(const CHwDevice& oHwDevice, size_t uiUsedFunction);

protected:

  //! Define list of hw devices to execute
  typedef CcVector<CHwDevice> CHwDeviceList;
  #ifdef _MSC_VER
    class CcKernelSHARED CHwDeviceList;
  #endif
    //! Define list of hw ports to execute
  typedef CcVector<CHwPort> CHwPortList;
  #ifdef _MSC_VER
    class CcKernelSHARED CHwPortList;
  #endif
  CHwDeviceList    m_oHwDevices;
  CHwPortList      m_oHwPorts;

  static CHwDevice  InvalidDevice;
  static CHwPin     InvalidPin;
  static CHwPort    InvalidPort;
};
