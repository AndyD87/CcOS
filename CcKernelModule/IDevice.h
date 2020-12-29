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
 * @subpage   IDevice
 *
 * @page      IDevice
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IDevice
 */

#ifndef H_IDevice_H_
#define H_IDevice_H_

#include "CcBase.h"
#include "CcKernelModule.h"
#include "CcList.h"

namespace NKernelModule
{

class IDriver;
class CcConnection;
class CcRequest;

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class IDevice
{
public:
  class CContext;
  enum class EType
  {
    Basic = 0,   //!< A non functional device
    FileSystem,  //!< A filesystem device 
    Disk,        //!< A disk device 
  };

  IDevice(IDriver* pDriver, EType eType) : 
    m_pDriver(pDriver),
    m_eType(eType)
  {}

  virtual ~IDevice()
  {
    stop();
  }

  virtual CcStatus start();
  virtual CcStatus stop() ;

  virtual void open(CcRequest& oRequest);
  virtual void shutdown(CcRequest& oRequest);
  virtual void cleanup(CcRequest& oRequest);
  virtual void close(CcRequest& oRequest);
  virtual void read(CcRequest& oRequest);
  virtual void write(CcRequest& oRequest);
  virtual void powerControl(CcRequest& oRequest);
  virtual void ioControl(CcRequest& oRequest);

  enum class ESpecificRequests
  {
    Unknown = 0,        //!< Unknown Request type
    QueryInformation,
    SetInformation,
    QueryEa,
    SetEa,
    FlushBuffers,
    QueryVolumeInformation,
    SetVolumeInformation,
    DirectoryControl,
    FileystemControl,
    LockControl
  };

  virtual void specificControl(ESpecificRequests eRequestType, CcRequest& oRequest);

protected:
  CContext* getContext()
  { return m_pContext; }
  void dbgBreak();

private:
  CContext*   m_pContext = nullptr;
  IDriver*    m_pDriver;
  EType       m_eType;
};

}

#endif // _IDevice_H_
