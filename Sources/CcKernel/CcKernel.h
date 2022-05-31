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
 * @brief     Class CcKernel
 */
#pragma once

#include "CcBase.h"
#include "CcApp.h"
#include "IDevice.h"
#include "CcMapCommon.h"
#include "CcVersion.h"

// forward declarations
class ISocket;
class INetworkStack;
class CcSystem;
class CcProcess;
class IThread;
class CcVersion;
class CcUserList;
class CcEventHandler;
class CcInputEvent;
class CcLog;
class CcFileSystem;
class CcDeviceList;
class CcAppList;
class CcDateTime;
class CcGroupList;
class ISharedMemory;
class IKernel;
class CcDevice;
class CcService;
class CcServiceSystem;
enum class ESocketType;

CCEXTERNC_BEGIN
  void CcKernel_Start();
  int main(int iArgc, char **ppArgv);
CCEXTERNC_END

/**
 * @brief Supported platform as enum
 */
enum class EPlatform
{
  Generic,
  Linux,
  Windows,
  Unknown
};

/**
 * @brief Kernel message type
 */
enum class EMessage
{
  Error,
  Warning,
  Info,
  Debug,
  Verbose,
};

class CcKernelPrivate;

/**
 * @brief The Global Kernel, alle methods and variables are static because only
 *        one Kernel can run at the same time
 */
class CcKernelSHARED CcKernel
{
public: // Methods
  /**
   * @brief Initialize the Kernel
   */
  CcKernel();

  /**
   * @brief Kernel shutdown will be initialized from external signal.
   *        This destructor is required on GENERIC mode.
   */
  ~CcKernel();

  /**
   * @brief Initialize the Graphical User Interface of System
   * @return true if GUI is available and started
   */
  static bool initGUI();

  /**
   * @brief Initialize the Command Line Interface of System
   * @return true if CLI is available and started
   */
  static bool initCLI();

  /**
   * @brief Deinitialize Command Line Interface
   * @return true if CLI is now stopped
   */
  static bool deinitCLI();

  /**
   * @brief Check if current privelegues are admin privilegues
   * @return true if running as admin
   */
  static bool isAdmin();

  /**
   * @brief Stop kernel.
   *        It will stop all all threads wich are currently running.
   */
  static void stop();

  /**
   * @brief Shutdown kernel and system.
   *        It will stop all all threads wich are currently running.
   */
  static void shutdown();

  /**
   * @brief Shutdown kernel and system.
   *        It will stop all all threads wich are currently running.
   */
  static void terminate();

  /**
   * @brief Set the global Debug Mode
   * @param bOnOff: Debug Mode
   *                true: Debug Mode On
   *                false: Debug Mode Off
   */
  static void setDebug(bool bOnOff);

  /**
   * @brief Get the global Debug Mode
   * @return Debug Mode
   *                true: Debug Mode On
   *                false: Debug Mode Off
   */
  static bool getDebug();

  /**
   * @brief Hold on for an amount of time.
   *        A delay of 0 will tell the system that it is ok to interrupt this process/thread.
   * @param uiDelay: Delay in ms
   */
  static void delayMs(uint32 uiDelay);

  /**
   * @brief Hold on for an amount of seconds.
   *        Internaly it will call delayMs() uiDelay times.
   *        A dalay of 0 seconds will be passed to delayMs with 0 ms, to give system the chance
   *        to switch threads.
   * @param uiDelay: Delay in s
   */
  static void delayS(uint32 uiDelay);

  /**
   * @brief Hold System for a number of milliseconds.
   * @copydetails CcKernel::delayMs()
   * @return Always true. This allowes the sleep method to be used in if() statements.
   */
  inline static bool sleep(uint32 uiDelay)
  { delayMs(uiDelay); return true; }

  /**
   * @brief Create a Thread from an Thread Object
   * @param Thread: ThreadObject to start
   * @return true if startup succeeded
   */
  static bool createThread(IThread &Thread);

  /**
   * @brief Create a Process of an external Programm
   *        If external Process is available intern, the internal
   *        process will be started.
   * @param processToStart: Process to get started
   * @return true if Process startup succeded
   */
  static bool  createProcess(CcProcess& processToStart);

  /**
   * @brief Get kernel interrupt handler for register callback.
   */
  static CcEventHandler& getInputEventHandler();

  /**
   * @brief Send an Input Event to Kernel, for example an Mouse or Keyboard Event.
   * @param InputEvent: Event value to transport.
   */
  static void emitInputEvent(CcInputEvent& InputEvent);

  /**
   * @brief Register an available Application to Kernel
   * @param hApplication: Pointer to Function wich is able to Start the app
   */
  static void addApp(const CcAppHandle &hApplication);

  /**
   * @brief Get all Applications registered to Kernel
   */
  static const CcAppList& getAppList();

  /**
   * @brief Get the System, for calling System-Methods directly if required
   * @return Handle to System.
   */
  static CcSystem &getSystem();

  /**
   * @brief Get the System, for calling System-Methods directly if required
   * @return Handle to System.
   */
  static CcServiceSystem &getServiceSystem();

  /**
   * @brief Get System Time
   * @return time in UTC
   */
  static CcDateTime getDateTime();

  /**
   * @brief Get Time the current system is running
   * @return Time duration since starup.
   */
  static CcDateTime getUpTime();

  /**
   * @brief Get a List of all available Users on System
   * @return UserList
   */
  static CcUserList getUserList();

  /**
   * @brief Get a List of all available Users on System
   * @return UserList
   */
  static CcGroupList getGroupList();

  /**
   * @brief Register a new Device to Kernel
   * @param Device: Pointer to Device
   */
  static const CcDevice& addDevice(const CcDevice& Device);

  /**
   * @brief Remove a device from Kernel
   * @param Device: Pointer to Device
   * @return True if Device was found and removed
   */
  static bool removeDevice(const CcDevice& Device);

  /**
   * @brief Get a specific Device
   * @param Type: Type of Device to search for
   * @param nr:   If more than one device are available, take the xth of it
   * @return Handle to Device
   */
  static const CcDevice& getDevice(EDeviceType Type, size_t nr=0);

  /**
   * @brief Get a specific Device
   * @param Type: Type of Device to search for
   * @param Name: Identify device to get by it's name
   * @return Handle to Device
   */
  static const CcDevice& getDevice(EDeviceType Type, const CcString& Name);

  /**
   * @brief Get all Devices registered in Kernel of specified type
   * @return List of Devices
   */
  static CcDeviceList getDevices(EDeviceType Type);

  /**
   * @brief Get Kernel interface for memory operations
   * @return Handle to Interface with kernel operators
   */
  static IKernel getInterface();

  /**
   * @brief Get all Devices registered in Kernel
   * @return List of Devices
   */
  static const CcDeviceList &getDeviceList();

  /**
   * @brief Get a Socket Handle from Kernel
   * @param eType: Type of socket, for example TCP or UDP
   * @return Handle to created Socket.
   */
  static ISocket* getSocket(ESocketType eType);

  /**
   * @brief Get systems network stack
   * @return Global network stack or null if not existing.
   */
  static INetworkStack* getNetworkStack();

  /**
   * @brief Get shared memory interface by name
   * @param sName:  Name of shared memory
   * @param uiSize: Size of shared memory
   * @return Pointer to shared memory interface or nullptr if error occured
   */
  static ISharedMemory* getSharedMemory(const CcString& sName, size_t uiSize);

  /**
   * @brief Get list of all environt variables
   * @return Map with all variables
   */
  static CcStringMap getEnvironmentVariables();

  /**
   * @brief Get environment variable by name
   * @param sName: Name of variable to query
   * @return Value or "" if not existing and empty
   */
  static CcString getEnvironmentVariable(const CcString& sName);

  /**
   * @brief Check if environment variable exists.
   * @param sName: Name of variable
   * @return True if variable is available
   */
  static bool getEnvironmentVariableExists(const CcString& sName);

  /**
   * @brief Set environment variable
   * @param sName:  Name of variable
   * @param sValue: Value for variable
   * @return True if variable was successfully set
   */
  static bool setEnvironmentVariable(const CcString& sName, const CcString& sValue);

  /**
   * @brief Remove environment variable by name
   * @param sName:  Name of variable to remove
   * @return True if remove succeeded
   */
  static bool removeEnvironmentVariable(const CcString& sName);

  /**
   * @brief Register an method to get called on Kernel shutdown
   */
  static void registerAtExit(void (*fAtExit)(void));

  /**
   * @brief Remove a registered method from Kernel shutdown
   */
  static void deregisterAtExit(void(*fAtExit)(void));

  /**
   * @brief Register Event to get called on device events of specific device type
   * @param eType:        Type of device to track
   * @param pEventHandle: Event to call an change
   */
  static void registerOnDevicePnpEvent(EDeviceType eType, const CcEvent& pEventHandle);

  /**
   * @brief Remove an registered event from pnp tracking
   * @param eType:    Type of device to remove from
   * @param pHandler: Object to remove
   */
  static void deregisterOnDevicePnpEvent(EDeviceType eType, CcObject* pHandler);

  /**
   * @brief Get current platform as enum
   * @return Platform as enum
   */
  static EPlatform getPlatform();

  static void addShutdownHandler(const CcEvent& oEvent);
  static void removeShutdownHandler(CcObject* pObject);

  /**
   * Get CcOS Kernel Version
   * @return Version information
   */
  static const CcVersion& getVersion();

  //! @return Get configuration dir like /etc
  static CcString getConfigDir();
  //! @return Get configuration dir like /var/lib
  static CcString getDataDir();
  //! @return Get configuration dir like /bin
  static CcString getBinaryDir();
  //! @return Get current working directory
  static CcString getWorkingDir();
  //! @return Get configuration dir like /tmp
  static CcString getTempDir();
  //! @return Get configuration dir like /home/[user]
  static CcString getUserDir();
  //! @return Get configuration dir like /home/[user]/.CcOS
  static CcString getUserDataDir();
  //! @return Get path to current running executable
  static CcString getCurrentExecutablePath();

  //! @param sPath: New path to set set Working dir. It can be relative or absolute
  static CcStatus setWorkingDir(const CcString& sPath);

  /**
   * @brief Override Kernel methods from another kernel.
   * @param oInterface: Handle to Interface with kernel operators
   */
  static void setInterface(const IKernel& oInterface);
  
  /**
   * @brief Load module by path
   * @param sPath:  Path to file to load
   * @return Status of operation
   */
  static CcStatus loadModule(const CcString& sPath);

  /**
   * @brief Call message type. Good for debug.
   * @param eType: Target type
   */
  static void message(EMessage eType);

  /**
   * @brief Write message to kernel
   * @param eType: Target type of message
   * @param sMessage: Message to write.
   */
  static void message(EMessage eType, const CcString& sMessage);

  /**
   * @brief Force a breakpoint because of unknown operation.
   */
  static void forceBreakpoint();

private:
  CcKernelPrivate* m_pPrivate;

  // always on last position!!!
  static CcVersion        s_oKernelVersion;
  static bool             s_bShutdownInProgress;  //!< Prepare for shutting down before starting.
};
