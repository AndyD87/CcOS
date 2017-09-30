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
 * @page      Windows
 * @subpage   CcSystem
 *
 * @page      CcSystem
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSystem
 **/

#ifndef CcSystem_H_
#define CcSystem_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Network/CcSocket.h"
#include "WindowsDisplay.h"
#include "WindowsTouch.h"
#include "WindowsService.h"
#include "CcGlobalStrings.h"
#include "CcHandle.h"
#include "CcFileSystemAbstract.h"
#include "CcMapCommon.h"

class WindowsTimer;
class CcThreadObject;
class CcProcess;
class CcUserList;
class CcGroupList;

class CcSystem
{
public:
  CcSystem();
  ~CcSystem();

  void init(void);
  bool initGUI(void);
  bool initCLI(void);
  int initService();
  bool createThread(CcThreadObject &Thread);
  bool createProcess(CcProcess &processToStart);
  void loadModule(const CcString& Path);

  CcStringMap getEnvironmentVariables() const;
  CcString getEnvironmentVariable(const CcString& sName) const;
  bool getEnvironmentVariableExists(const CcString& sName) const;
  bool setEnvironmentVariable(const CcString& sName, const CcString& sValue);
  bool removeEnvironmentVariable(const CcString& sName);


  CcDateTime getDateTime();
  void sleep(uint32 timeoutMs);
  CcSocket* getSocket(ESocketType type);

  /**
   * @brief Load a User List from System if Users are availbale
   * @param UserList: UserList to appen the System Users
   * @return true, if System has Users to store
   */
  CcUserList getUserList(void);

  /**
   * @brief Load a User List from System if Users are availbale
   * @param UserList: UserList to appen the System Users
   * @return true, if System has Users to store
   */
  CcGroupList getGroupList(void);

  CcDeviceHandle getDevice(EDeviceType Type, const CcString& Name)
    { CCUNUSED(Type); CCUNUSED(Name); return nullptr; }

  CcString getConfigDir() const;
  CcString getDataDir() const;
  CcString getBinaryDir() const;
  CcString getWorkingDir(void) const;
  CcString getTemporaryDir(void) const;
  CcString getUserDir() const;
  CcString getUserDataDir() const;
protected:

private:
  void initSystem(void);
  void initTimer( void );
  void initDisplay( void );
  void initFilesystem( void );

  void systemTick(void);
  CcHandle<WindowsDisplay>  m_Display;
  CcHandle<WindowsTimer>    m_Timer;
  CcHandle<WindowsService>  m_Service;
  CcFileSystemHandle    m_Filesystem;
  bool m_GuiInitialized = false;
  bool m_CliInitialized = false;
  bool m_bSystemState   = false;
};

#endif /* CcSystem_H_ */
