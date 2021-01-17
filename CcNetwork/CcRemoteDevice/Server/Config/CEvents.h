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
#ifndef H_CEvents_H_
#define H_CEvents_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
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
  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);
  size_t writeBinary(IIo& pStream);

public:
  CcList<CEvent> oTimeEvents;
};

}
}
}
#endif // H_CEvents_H_
