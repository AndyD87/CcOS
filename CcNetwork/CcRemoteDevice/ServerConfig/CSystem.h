/*
 * This file is part of CSystem.
 *
 * CSystem is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CSystem is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CSystem.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CSystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CSystem
 **/
#ifndef H_CSystem_H_
#define H_CSystem_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"

namespace NsRemoteDeviceServerConfig
{

class CcRemoteDeviceSHARED CDevice
{
public:
  enum class EType
  {
    Gpio
  };

  CCDEFINE_EQUAL_OPERATORS(CDevice)

  EType eType;
  union
  {
    struct
    {
      uint8 uiPort;
      uint8 uiPin;
    } Gpio;
  } Data;
};

#ifdef _MSC_VER
  template class CcRemoteDeviceSHARED CcList<CDevice>;
#endif

/**
 * @brief CSystem implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CSystem
{
public:
  CcList<CDevice> oDevices;
};
}

#endif // H_CSystem_H_
