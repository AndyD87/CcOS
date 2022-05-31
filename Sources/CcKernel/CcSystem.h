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
 * @brief     Class CcSystem
 */
#pragma once

#include "CcBase.h"
#include "Network/ISocket.h"
#include "IFileSystem.h"
#include "CcFileSystem.h"
#include "CcDeviceList.h"
#include "CcMapCommon.h"
#include "CcGlobalStrings.h"

//forward declarations
class CcProcess;
class IThread;
class CcUserList;
class CcGroupList;
class ISharedMemory;
class CcVersion;
class CcString;
class INetworkStack;
class IModuleBase;
class IKernel;
class CcKernelPrivate;
class CcDevice;
class CcService;

/**
 * @brief Main System class.
 *        This class is the connection from CcOS universe to Hardware or underlying OS.
 *        Every Platform has to implement this methods.
 *        For Example in Windows the File CcWindowsSystem.cpp is doing this,
 *        in Linux it would be done by CcLinuxSystem.com
 */
class CcKernelSHARED CcSystem
{
public:
  //! Create system and all required variables
  CcSystem(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel);
  //! Cleanup system and all required variables
  ~CcSystem();

  /**
   * @brief Initialize system and all substems like console,
   *        network, filesystem, thread manager, etc..
   */
  void init();

  /**
   * @brief Deinitialize all subsystems and sysem to shutdown.
   */
  void deinit();

  /**
   * @brief Initialize gui interface
   * @return True if gui can be used
   */
  bool initGUI();

  /**
   * @brief Initialize cli if required.
   *        Most system do support this without initializing.
   * @return True if cli can be used.
   */
  bool initCLI();

  /**
   * @brief Cleanup cli if required
   * @return True if cli was cleaned up successfully
   */
  bool deinitCLI();

  /**
   * @brief Check if calling application is runnig with elevated privilegues.
   * @return True with admin rights.
   */
  bool isAdmin();

  /**
   * @brief Create thread and start interface in it's own conext
   * @param oThread: Thread to start
   * @return True if thread creation succeded
   */
  bool createThread(IThread& oThread);

  /**
   * @brief Create process will start an external application in seperate thread.
   * @param oProcessToStart: Process to start.
   * @return True if process creation was started successfully
   */
  bool createProcess(CcProcess& oProcessToStart);

  /**
   * @brief Call this method to indicate error, for example with a LED.
   */
  void error();

  /**
   * @brief Call this method to indicate warning, for example with a LED.
   */
  void warning();

  /**
   * @brief Create network socket from network stack.
   *        The socket has to be deleted later.
   * @param eType: Type of socket to create
   * @return Interface to socket.
   */
  ISocket* getSocket(ESocketType eType);

  /**
   * @brief Get runnning network stack if one exists.
   * @return Networkstack or null for no available network.
   */
  INetworkStack* getNetworkStack();
  //! @return Get name of device
  CcString getName();
  //! @return Get version of system
  CcVersion getVersion();
  //! @return Get system time
  CcDateTime getDateTime();
  //! @return Get system uptime
  CcDateTime getUpTime();
  //! @return Get system users as list
  CcUserList getUserList();
  //! @return Get system groups as list
  CcGroupList getGroupList();
  //! @return Get Configuration dir /etc on linux
  CcString getConfigDir() const;
  //! @return Get Data dir /var/lib on linux
  CcString getDataDir() const;
  //! @return Get Binary dir /usr/bin on linux
  CcString getBinaryDir() const;
  //! @return Get working dir where application was executed
  CcString getWorkingDir() const;
  //! @return Get Temporary directory on system like /tmp on linux
  CcString getTemporaryDir() const;
  //! @return Get Home direcotry of current user
  CcString getUserDir() const;
  //! @return Get Data directory like %APPDATA% on windows
  CcString getUserDataDir() const;

  //! @return Get path to current running executable
  CcString getCurrentExecutablePath() const;

  //! @return Get all environment variables as map
  CcStringMap getEnvironmentVariables() const;

  /**
   * @brief Get environment variable by name
   * @param sName: Name of variable to query
   * @return Value of environment variable
   */
  CcString getEnvironmentVariable(const CcString& sName) const;

  /**
   * @brief Check if environment variable by name exists
   * @param sName: Name of variable to query
   * @return True if variable exists
   */
  bool getEnvironmentVariableExists(const CcString& sName) const;

  /**
   * @brief Set environment variable for instance
   * @param sName:  Name of variable to set
   * @param sValue: Value of variable to set
   * @return True if variable set successfully
   */
  bool setEnvironmentVariable(const CcString& sName, const CcString& sValue);

  /**
   * @brief Remove environment variable from instance
   * @param sName: Name of variable to remove
   * @return True if variable was removed successully
   */
  bool removeEnvironmentVariable(const CcString& sName);

  /**
   * @brief Sleep miliseconds
   * @param uiTimeoutMs: Miliseconds to timeout
   */
  void sleep(uint32 uiTimeoutMs);

  /**
   * @brief Get device by type and name
   * @param eType:  Type of device to get
   * @param sName:  Name of device to search for
   * @return Found device or invalid device if not
   */
  const CcDevice& getDevice(EDeviceType eType, const CcString& sName);

  /**
   * @brief Get shared memory object by name and size.
   * @param sName:  Name has to be unique on system
   * @param uiSize: Size of shared memor to get.
   * @return Shared memory interface as pointer or null on error.
   */
  ISharedMemory* getSharedMemory(const CcString& sName, size_t uiSize);

  /**
   * @brief Load module by path
   * @param sPath:    Module to load
   * @param oKernel:  Kernel interface for common memory rang
   * @return Status of operation
   */
  CcStatus loadModule(const CcString& sPath, const IKernel& oKernel);

  /**
   * @brief Change working directory for current application
   * @param sPath: Path to set as working directory
   * @return Status of operation
   */
  CcStatus setWorkingDir(const CcString& sPath);

  /**
   * @brief Set new name for this PC. This will take effekt after restarting.
   * @param sName: New name for PC
   * @return Status of operation
   */
  CcStatus setName(const CcString& sName);

  /**
   * @brief Register a device to be called on system idle method
   * @param pDevice: Device to call on idle
   */
  void registerForIdle(IDevice* pDevice);

  /**
   * @brief Remove a registered device from idle
   * @param pDevice: Device to remove from idle
   */
  bool deregisterForIdle(IDevice* pDevice);

  CcStatus restart(const CcString& sMessage = CcGlobalStrings::System::DefaultRebootMessage, bool bForce = true);
  CcStatus shutdown(const CcString& sMessage = CcGlobalStrings::System::DefaultShutdownMessage, bool bForce = true);

public: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};
