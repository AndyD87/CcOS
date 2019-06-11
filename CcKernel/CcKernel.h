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
 * @page      CcKernel
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcKernel
 */

#ifndef _CCKERNEL_H_
#define _CCKERNEL_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcApp.h"
#include "IDevice.h"
#include "CcMapCommon.h"

// forward declarations
class ISocket;
class CcSystem;
class CcProcess;
class IThread;
class CcVersion;
class CcUserList;
class CcEventHandler;
class CcInputEvent;
class CcThreadManager;
class CcLog;
class CcFileSystem;
class CcDeviceList;
class CcAppList;
class CcDateTime;
class CcGroupList;
class ISharedMemory;
enum class ESocketType;

CCEXTERNC_BEGIN
  void CcKernel_Start();
  int main(int iArgc, char **ppArgv);
CCEXTERNC_END

enum class EPlatform
{
  Generic,
  Linux,
  Windows,
  Unknown
};

enum class EMessage
{
  Error,
  Warning,
  Info,
  Debug,
  Verbose,
};

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
   * @brief Delete all open handles, apps and mods
   *        Shutdown the System.
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
   * @brief Initialize Kernel to run as Service
   * @return true if CLI is available and started
   */
  static int initService();

  /**
   * @brief Shutdown kernel and system.
   *        It will stop all all threads wich are currently running.
   */
  static void shutdown();

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

  //! @copydoc CcKernel::delayMs
  inline static void sleep(uint32 uiDelay)
    { delayMs(uiDelay);}

  /**
   * @brief Create a Thread from an Thread Object
   * @param Thread: ThreadObject to start
   * @param sName:  Name of Thread for System-Information
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
   * @brief To receive global Input Events, Register an Object with this Method
   * @param Receiver: Object get called on Event
   * @param callbackNr: The id the Object gets called with
   */
  static CcEventHandler& getInputEventHandler();

  /**
   * @brief Send an Input Event to Kernel, for example an Mouse or Keyboard Event.
   * @param InputEvent: Event value to transport.
   */
  static void emitInputEvent(CcInputEvent& InputEvent);

  /**
   * @brief Register an available Application to Kernel
   * @param appFunc:  Pointer to Function wich is able to Start the app
   * @param Name:     Name of Application for identification
   */
  static void addApp(const CcAppHandle &hApplication);

  /**
   * @brief Get all Applications registered to Kernel
   * @return All Apps in List
   */
  static const CcAppList& getAppList();

  /**
   * @brief Get the System, for calling System-Methods directly if required
   * @return Handle to System.
   */
  static CcSystem &getSystem();

  /**
   * @brief Get System Time
   * @return time in UTC
   */
  static CcDateTime        getDateTime();

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
   * @param Type:   Device Type
   */
  static void addDevice(CcDeviceHandle Device);

  /**
   * @brief Remove a device from Kernel
   * @param Device: Pointer to Device
   * @param Type:   Device Type
   */
  static void removeDevice(CcDeviceHandle Device);

  /**
   * @brief Get a specific Device
   * @param Type: Type of Device to search for
   * @param nr:   If more than one device are available, take the xth of it
   * @return Handle to Device
   */
  static CcDeviceHandle getDevice(EDeviceType Type, size_t nr=0);

  /**
   * @brief Get a specific Device
   * @param Type: Type of Device to search for
   * @param Name: Identify device to get by it's name
   * @return Handle to Device
   */
  static CcDeviceHandle getDevice(EDeviceType Type, const CcString& Name);

  /**
   * @brief Get all Devices registered in Kernel of specified type
   * @return List of Devices
   */
  static CcDeviceList getDevices(EDeviceType Type);

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

  static ISharedMemory* getSharedMemory(const CcString& sName, size_t uiSize);

  static CcStringMap getEnvironmentVariables();
  static CcString getEnvironmentVariable(const CcString& sName);
  static bool getEnvironmentVariableExists(const CcString& sName);
  static bool setEnvironmentVariable(const CcString& sName, const CcString& sValue);
  static bool removeEnvironmentVariable(const CcString& sName);
  static void registerOnDevicePnpEvent(EDeviceType eType, IEvent* pEventHandle);
  static void deregisterOnDevicePnpEvent(EDeviceType eType, CcObject* pHandler);

  static EPlatform getPlatform();
  static CcEventHandler& getShutdownHandler();

  /**
   * Get CcOS Kernel Version
   * @return Version information
   */
  static const CcVersion& getVersion();

  static CcString getConfigDir();
  static CcString getDataDir();
  static CcString getBinaryDir();
  static CcString getWorkingDir();
  static CcString getTempDir();
  static CcString getUserDir();
  static CcString getUserDataDir();

  static void message(EMessage eType);
  //static void message(EMessage eType, const CcString& sMessage);

private:
  // always on last position!!!
  static CcKernel Kernel;            //!< create a single instance of it self, for startup initializing
};

#endif /* _CCKERNEL_H_ */

/**
 * Register Subpages for Kernel
 * @page      CcKernel
 * @subpage   Events
 * @subpage   Network
 * @subpage   Types
 * @subpage   Driver
 * @subpage   Platform
 *
 * @page      Events
 * @page      Network
 * @page      Types
 * @page      Driver
 * @page      Platform
 *
 * @page      Types
 * @subpage   Hash
 *
 * @page      Hash
 */
