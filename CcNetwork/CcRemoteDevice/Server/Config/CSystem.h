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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CSystem
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CWlanAccessPoint.h"
#include "CWlanClient.h"
#include "CcJson/CcJsonObject.h"
#include "IIo.h"
#include "CcConfig/CcConfigBinary.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
{

/**
 * @brief Configuration for devices
 */
class CcRemoteDeviceSHARED CDevice
{
public:
  /**
   * @brief Type of current device
   */
  enum class EType
  {
    Gpio
  };

  CCDEFINE_EQUAL_OPERATORS(CDevice)

  EType eType;        //!< Type of this device
  union
  {
    struct
    {
      uint8 uiPort;     //!< Port of gpio device
      uint8 uiPin;      //!< Pin in uiPort of gpio device
    } Gpio;             //!< Gpio device settings
  } Data;               //!< Union with device specific informations
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
  /**
   * @brief Parse json node to get settings for device from.
   *        The json node should previosly written by @ref writeJson
   * @param rJson: Target node to parse
   */
  void parseJson(CcJsonNode& rJson);

  /**
   * @brief Write current configuration to json node.
   *        The json node should be readable by @ref parseJson
   * @param rNode: Node to write configuration data to.
   */
  void writeJson(CcJsonNode& rNode);

  /**
   * @brief Parse binary Item stream to determine common informations from it.
   * @param pItem:      Initial item to get config from
   * @param uiMaxSize:  Size left available for paring
   * @return Next item after all device config was parsed.
   */
  const CcConfigBinary::CItem* parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);

  /**
   * @brief Write binary config data to output stream.
   * @param pStream: Target stream to write data to.
   * @return Number of bytes written to pStream.
   */
  size_t writeBinary(IIo& pStream);

public:
  CcString          sName;            //!< Name of system or machine
  CWlanAccessPoint  oWlanAccessPoint; //!< Configuration of Accesspoint if available
  CWlanClient       oWlanClient;      //!< Configuration of Client if available
};

}
}
}
