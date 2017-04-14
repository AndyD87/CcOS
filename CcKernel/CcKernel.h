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
 * @subpage   com
 * @subpage   dev
 * @subpage   CcApp
 * @subpage   CcAppList
 * @subpage   CcConsole
 * @subpage   CcDevice
 * @subpage   CcDeviceList
 * @subpage   CcFile
 * @subpage   CcFileSystem
 * @subpage   CcGroup
 * @subpage   CcInputEvent.defs
 * @subpage   CcInputEvent
 * @subpage   CcIODevice
 * @subpage   CcLog
 * @subpage   CcModule
 * @subpage   CcProcess
 * @subpage   CcStdIn
 * @subpage   CcStdOut
 * @subpage   CcStdFile
 * @subpage   CcStdErr
 * @subpage   CcSystem
 * @subpage   CcThread
 * @subpage   CcThreadManager
 * @subpage   CcThreadObject
 * @subpage   CcUser
 * @subpage   CcUserList
 * @subpage   CcWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcKernel
 */

#ifndef CCKERNEL_H_
#define CCKERNEL_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcHandle.h"
#include "CcVector.h"
#include "CcObject.h"
#include "CcEventHandler.h"
#include "CcInputEvent.h"
#include "CcDevice.h"
#include "CcApp.h"
#include "CcThreadObject.h"
#include "CcThreadManager.h"
#include "CcLog.h"
#include "CcFileSystemManager.h"
#include "CcDeviceList.h"
#include "CcAppList.h"
#include "CcProcess.h"
#include "CcUserList.h"

// forward declarations
class CcSocket;
enum class ESocketType;

enum class EKernelState
{
  Initializing = 0,
  Starting,
  Running,
  Stopping,
  Stopped //!< This set is last action of Kernel
};

// forward declarations
class CcSystem;

/**
 * @brief The Global Kernel, alle methods and variables are static because only
 *        one Kernel can run at the same time
 */
class CcKernelSHARED CcKernel  {
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
   * @brief Inititalize the Kernel
   */
  static void init( void );

  /**
   * @brief Start Kernel, this function get called from Target System
   */
  static void start( void );

  /**
   * @brief Stop Kernel and close all handles and Applications
   *        Kernel is ready to delete and shutdown.
   */
  static void stop(void);

  /**
   * @brief Initialize the Graphical User Interface of System
   * @return true if GUI is available and started
   */
  static bool initGUI( void );

  /**
   * @brief Initialize the Command Line Interface of System
   * @return true if CLI is available and started
   */
  static bool initCLI( void );
  
  /**
   * @brief Initialize Kernel to run as Service
   * @return true if CLI is available and started
   */
  static int initService();

  /**
   * @brief Store the global callup arguments in CcKernel::
   * @param argc: Number of Arguments in Array
   * @param argv: Array with Arguments as std c strings.
   */
  static void setArg(int argc, char **argv);

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
  static bool getDebug(void);

  /**
   * @brief Generate a Tick to Kerne-Clock for Timing-Functions
   *        Function should get called every 1ms from System
   */
  static void systemTick( void );

  /**
   * @brief Function get called from System to indicate that
   *        System is initialized.
   */
  static void systemReady( void );

  /**
   * @brief Hold on for an Amount of Time
   * @param uiDelay: Delay in ms
   */
  static void delayMs(uint32 uiDelay);

  /**
   * @brief Hold on for an Amount of Time
   * @param uiDelay: Delay in s
   */
  static void delayS(uint32 uiDelay);

  /**
   * @brief Create a Thread from an Thread Object
   * @param Thread: ThreadObject to start
   * @param sName:  Name of Thread for System-Information
   * @return true if startup succeeded
   */
  static bool createThread(CcThreadObject &Thread);

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
   * @brief Send an Event to Kernel, for example an Mouse or Keyboard Event.
   * @param InputEvent: Event value to transport.
   */
  static void emitEvent(CcInputEvent& InputEvent);

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
  static const CcAppList& getAppList(void);

  /**
   * @brief Get the System, for calling System-Methods directly if required
   * @return Handle to System.
   */
  static const CcSystem &getSystem(void);

  /**
   * @brief Get System Time
   * @return time in UTC
   */
  static CcDateTime        getDateTime(void);

  /**
   * @brief Get a List of all available Users on System
   * @return UserList
   */
  static const CcUserList    &getUserList(void);

  /**
   * @brief Register a new Device to Kernel
   * @param Device: Pointer to Device
   * @param Type:   Device Type
   */
  static void addDevice(CcHandle<CcDevice> Device, EDeviceType Type = EDeviceType::All);

  /**
   * @brief Get a specific Device
   * @param Type: Type of Device to search for
   * @param nr:   If more than one device are available, take the xth of it
   * @return Handle to Device
   */
  static CcHandle<CcDevice> getDevice(EDeviceType Type, uint16 nr=0);

  /**
   * @brief Get a specific Device
   * @param Type: Type of Device to search for
   * @param Name: Identify device to get by it's name
   * @return Handle to Device
   */
  static CcHandle<CcDevice>     getDevice(EDeviceType Type, const CcString& Name);

  /**
   * @brief Get all Devices registered in Kernel
   * @return List of Devices
   */
  static const CcDeviceList &getDeviceList(void);

  /**
   * @brief Get a Socket Handle from Kernel
   * @param eType: Type of socket, for example TCP or UDP
   * @return Handle to created Socket.
   */
  static CcSocket*     getSocket(ESocketType eType);

  /**
   * @brief Get Kernel-FilesystemManager
   * @return FileSystemManager with all available mounting-points
   */
  static const CcFileSystemManager& getFileSystemManager(void);

  static const CcString& getConfigDir();
  static const CcString& getDataDir();
  static const CcString& getBinaryDir();
  static const CcString& getWorkingDir(void);

private:
  static EKernelState s_eState;
  static CcSystem* m_System;      //!< Pointer to System wich is getting initialized when Kernel is created
  static time_t m_SystemTime;           //!< System Time in UTC
  static CcEventHandler m_EventHandler; //!< Object Handler with all Event-Receiver
  static int    m_argc;                 //!< Count of Startup Parameters todo: replace with StringList
  static char **m_argv;                 //!< Startup parameters todo: replace with StringList
  static bool   m_SystemStarted;        //!< Check if Target-System is started
  static bool   m_bDebug;               //!< Set Debug-Mode on for debug messages
  static CcFileSystemManager  m_oFileSystemManager;  //!< Handle to Filesystem, set by System or Kernel itself.
  static CcAppList            m_AppList;       //!< Applications currently registered to Kernel
  static CcThreadManager      m_Threads;       //!< Managing all created Threads
  static CcDeviceList         m_DeviceList;    //!< List of Devices registered to Kernel for lowlevel access
  static CcLog                m_Log;           //!< Log-Manager to handle Kernel-Output messages
  static CcUserList           m_UserList;      //!< List of Users available on System

  // always on last position!!!
  static CcKernel Kernel;            //!< create a single instance of it self, for startup initializing
};

#endif /* CCKERNEL_H_ */
