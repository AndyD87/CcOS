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
 * @subpage   IWlanAccessPoint
 *
 * @page      IWlanAccessPoint
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IWlanAccessPoint
 */

#ifndef H_IWlanAccessPoint_H_
#define H_IWlanAccessPoint_H_

#include "CcBase.h"
#include "CcEventHandler.h"
#include "INetwork.h"

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED IWlanAccessPoint : public INetwork
{
public:
  virtual ~IWlanAccessPoint() = default;

  virtual CcStatus setCredentials(const CcString& sSsid, const CcString& sPassord) = 0;
};

#endif // _IWlanAccessPoint_H_
