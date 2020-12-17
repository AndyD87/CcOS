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
 * @page      CcKernelModule
 * @subpage   IFsDevice
 *
 * @page      IFsDevice
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IFsDevice
 */

#ifndef H_IFsDevice_H_
#define H_IFsDevice_H_

#include "CcBase.h"
#include "CcKernelModule.h"
#include "CcList.h"
#include "IDevice.h"

namespace NKernelModule
{

class IDriver;
class CcConnection;
class CcRequest;

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelModuleSHARED IFsDevice : public IDevice
{
public:
  IFsDevice(IDriver* pDriver);
  virtual ~IFsDevice();

  virtual void open(CcRequest& oRequest);
  virtual void shutdown(CcRequest& oRequest);
  virtual void cleanup(CcRequest& oRequest);
  virtual void close(CcRequest& oRequest);
  virtual void read(CcRequest& oRequest);
  virtual void write(CcRequest& oRequest);
  virtual void powerControl(CcRequest& oRequest);
  virtual void ioControl(CcRequest& oRequest);
  virtual void specificControl(IDevice::ESpecificRequests eRequestType, CcRequest& oRequest);

private:
  void checkMountVolume(CcRequest& oRequest);
};

}

#endif // _IFsDevice_H_
