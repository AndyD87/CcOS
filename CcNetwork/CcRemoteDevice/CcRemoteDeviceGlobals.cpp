/*
 * This file is part of CcRemoteDeviceGlobals.
 *
 * CcRemoteDeviceGlobals is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceGlobals is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceGlobals.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceGlobals
 */
#include "CcRemoteDeviceGlobals.h"

namespace CcRemoteDeviceGlobals
{
  const CcString ProjectName("CcRemoteDevice");
  namespace Defaults
  {
    const CcString SslKeyFilename("Key.crt");
    const CcString SslCertificateFilename("Certificate.crt");
  }
  namespace Names
  {
    const CcString ConfigName("Config.json");
    const CcString ClientAppName("CcRemoteDeviceClient");
  }
}
