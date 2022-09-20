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
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Interface IDevice
 */
#pragma once

#include "CcBase.h"
#include "CcKernelModule.h"
#include "CcList.h"

namespace NKernelModule
{

class IDriver;
class CcConnection;
class CcRequest;

/**
 * @brief Abstract Class for inheriting to every Device
 */
class IDevice
{
public:
  class CContext;
  /**
   * @brief Known type of driver supported
   */
  enum class EType
  {
    Basic = 0,   //!< A non functional device
    FileSystem,  //!< A filesystem device 
    Disk,        //!< A disk device 
  };

  /**
   * @brief Init device with it's type and parent driver object
   * @param pDriver: Parent driver for init
   * @param eType:   Type of device to initialize subsystem
   */
  IDevice(IDriver* pDriver, EType eType) : 
    m_pDriver(pDriver),
    m_eType(eType)
  {}

  virtual ~IDevice()
  {
    stop();
  }

  /**
   * @brief Start device if not already done
   * @return Status of operation
   */
  virtual CcStatus start();

  /**
   * @brief Stop device if not already done
   * @return Status of operation
   */
  virtual CcStatus stop() ;

  /**
   * @brief Create an communication instance to this device and setup request context.
   * @param oRequest: Incoming request to setup for further communications
   */
  virtual void open(CcRequest& oRequest);

  /**
   * @brief Shutdown request for stopping device by command
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void shutdown(CcRequest& oRequest);

  /**
   * @brief Cleanup request for free all resources
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void cleanup(CcRequest& oRequest);

  /**
   * @brief Shutdown request for close communications instance and cleanup request context.
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void close(CcRequest& oRequest);

  /**
   * @brief Execute an read on device
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void read(CcRequest& oRequest);

  /**
   * @brief Execute an write on device
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void write(CcRequest& oRequest);

  /**
   * @brief Execute an power control request on device
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void powerControl(CcRequest& oRequest);

  /**
   * @brief Execute an io control request form userspace or other device on this device
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void ioControl(CcRequest& oRequest);

  /**
   * @brief Incomming known requests from Operating system
   */
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

  /**
   * @brief Specific control request received and filetered by enum.
   * @param eRequestType: Type of request as enum
   * @param oRequest: Request context initialize by @ref open
   */
  virtual void specificControl(ESpecificRequests eRequestType, CcRequest& oRequest);

  EType getDeviceType() const
  { return m_eType; }

  const IDriver* getDriver() const
  { return m_pDriver; }
protected:
  /**
   * @brief Get device context, initialized by subsystem
   * @return
   */
  CContext* getContext()
  { return m_pContext; }

  /**
   * @brief Debug break point wich will hold on the whole operating system.
   * @attention Be carefull with this command!
   */
  void dbgBreak();

private:
  CContext*   m_pContext = nullptr;
  IDriver*    m_pDriver;
  EType       m_eType;
};

}
