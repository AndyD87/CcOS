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
 * @page      Devices
 * @subpage   IWlan
 *
 * @page      IWlan
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IWlan
 */

#ifndef H_IWlan_H_
#define H_IWlan_H_

#include "CcBase.h"
#include "IDevice.h"

class IWlanClient;
class IWlanAccessPoint;

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED IWlan : public IDevice
{
public: // Types
  class CCapabilities
  {
  public:
    CCapabilities(uint16 uiCapabilities = 0) : uiCapabilities(uiCapabilities){}
    bool hasAccessPoint()
      { return (uiCapabilities & AccesssPoint) != 0; }
    bool hasClient()
      { return (uiCapabilities & AccesssPoint) != 0; }
  public:
    uint16 uiCapabilities;

  public:
    static const uint16 AccesssPoint; //!< 1
    static const uint16 Client;       //!< 2
  };

public:
  virtual ~IWlan() = default;
  virtual IWlanAccessPoint* getAccessPoint() = 0;
  virtual IWlanClient* getClient() = 0;
  virtual CCapabilities getCapabilities() = 0;
};

#endif // _IWlan_H_
