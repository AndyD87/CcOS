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
 * @subpage   CcDeviceTimer
 *
 * @page      CcDeviceTimer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceTimer
 */

#ifndef H_CcDeviceTimer_H_
#define H_CcDeviceTimer_H_

#include "CcBase.h"
#include "Devices/ITimer.h"
#include "Devices/CcDevice.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcDevice<ITimer>;
#endif

/**
 * @brief Abstract Timer Device for triggered events
 * @todo Implementation is not yet done for timers
 */
class CcKernelSHARED CcDeviceTimer : public CcDevice<ITimer>
{
public:
  CcDeviceTimer(const CcDeviceHandle& oHandle) :
    CcDevice<ITimer>(oHandle)
  {}
  virtual ~CcDeviceTimer() = default;
};

#endif // H_CcDeviceTimer_H_
