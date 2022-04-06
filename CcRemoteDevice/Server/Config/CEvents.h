/*
 * This file is part of CEvents.
 *
 * CEvents is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CEvents is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CEvents.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CEvents
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "Json/CcJsonObject.h"
#include "IIo.h"
#include "Config/CcConfigBinary.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
{

/**
 * @brief Event definitions for timing events
 */
class CcRemoteDeviceSHARED CEvent
{
public:
  /**
   * @brief Type of Event
   */
  enum class EType
  {
    Unknown = 0  //!< Unknown event, do nothing if called
  };
  /**
   * @brief Implementation of unused operator for list
   * @return Always false
   */
  inline bool operator==(const CEvent&) const { return false; }


  EType eType     = EType::Unknown; //!< Event type of this object.
  CcString sData;                   //!< Data containing infromations for execute event.
};

#ifdef _MSC_VER
template class CcRemoteDeviceSHARED CcList<CEvent>;
#endif

/**
 * @brief CEvents implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CEvents
{
public:
  /**
   * @brief Parse json node to get config
   * @param rJson: Json to get config from
   */
  void parseJson(CcJsonNode& rJson);

  /**
   * @brief Generate json from config and write to config json document.
   * @param rNode: Node to write json to
   */
  void writeJson(CcJsonNode& rNode);

  /**
   * @brief Parse binary config file
   * @param pItem:      First item to search for this config
   * @param uiMaxSize:  Max size of binary left to read
   * @return Pointer to next itemp
   */
  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);

  /**
   * @brief Write config to binary stream
   * @param pStream: Target stream
   * @return Number of bytes written
   */
  size_t writeBinary(IIo& pStream);

public:
  CcList<CEvent> oTimeEvents;   //!< List of time events from config
};

}
}
}
