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
 * @page      CcRestApiDevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRestApiDevice
 */
#ifndef _CcRestApiDevice_H_
#define _CcRestApiDevice_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IRestApi.h"
#include "IDevice.h"

class CcRestApiDevices;
class CcJsonData;

/**
 * @brief CcRestApiDevice impelmentation
 */
class CcHttpSHARED CcRestApiDevice : public IRestApi
{
public:
  /**
   * @brief Constructor
   */
  CcRestApiDevice(CcRestApiDevices* pParent, const CcDeviceHandle& oDeviceHandle);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiDevice();

  virtual bool get(CcHttpWorkData& oData) override;
  virtual bool post(CcHttpWorkData& oData) override;
  CcJsonData getInfo();

  void setName(const CcString& sName)
    { m_sName = sName; };
  const CcString& getName() const
    { return m_sName; }

private:
  bool getGpioDeviceInfo(CcHttpWorkData& oData);

  bool postGpioDeviceInfo(CcHttpWorkData& oData);
private:
  CcString       m_sName;
  CcDeviceHandle m_oDevice;
};

#endif /* _CcRestApiDevice_H_ */
