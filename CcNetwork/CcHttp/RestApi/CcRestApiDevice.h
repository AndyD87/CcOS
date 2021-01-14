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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRestApiDevice
 */
#ifndef H_CcRestApiDevice_H_
#define H_CcRestApiDevice_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IRestApi.h"
#include "CcDevice.h"

class CcRestApiDevices;
class CcJsonNode;

/**
 * @brief CcRestApiDevice implementation
 */
class CcHttpSHARED CcRestApiDevice : public IRestApi
{
public:
  /**
   * @brief Constructor
   */
  CcRestApiDevice(CcRestApiDevices* pParent, const CcDevice& oDeviceHandle);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiDevice();

  virtual CcJsonNode getDeviceNode() = 0;

  void setName(const CcString& sName)
    { m_sName = sName; }
  const CcString& getName() const
    { return m_sName; }
  const CcDevice& getDevice() const
    { return m_oDevice; }

private:

  bool postGpioDeviceInfo(CcHttpWorkData& oData);
private:
  CcString   m_sName;
  const CcDevice&  m_oDevice;
};

#endif // H_CcRestApiDevice_H_
