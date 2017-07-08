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

class WindowsTimer;
class CcThreadObject;
class CcProcess;
class CcUserList;

class CcSystem
{
public:
  CcSystem();
  ~CcSystem();

  void init(void);
  bool initGUI(void);
  bool initCLI(void);
  int initService();
  bool start(void);
  void stop(void);
  bool createThread(CcThreadObject &Thread);
  bool createProcess(CcProcess &processToStart);
  void loadModule(const CcString& Path);
  CcDateTime getDateTime();
  void sleep(uint32 timeoutMs);
  CcSocket* getSocket(ESocketType type);

  /**
   * @brief Load a User List from System if Users are availbale
   * @param UserList: UserList to appen the System Users
   * @return true, if System has Users to store
   */
  CcUserList getUserList(void);

  CcHandle<CcDevice> getDevice(EDeviceType Type, const CcString& Name)
    { CCUNUSED(Type); CCUNUSED(Name); return nullptr; }

  const CcString& getConfigDir() const
    { return m_sConfigDir; }
  const CcString& getDataDir() const
    { return m_sDataDir; }
  const CcString& getBinaryDir() const
    { return CcGlobalStrings::Empty; }
  const CcString& getWorkingDir(void) const
    { return m_sWorking; }
  const CcString& getTemporaryDir(void) const
    { return m_sTempDir; }

protected:

private:
  void initSystem(void);
  void initTimer( void );
  void initDisplay( void );
  void initFilesystem( void );
  void setProgramConfigDir(const CcString& sProgramConfDir)
    { m_sConfigDir = sProgramConfDir; }
  void setProgramDataDir(const CcString& sProgramDataDir)
    { m_sDataDir = sProgramDataDir; }

  void systemTick(void);
  CcHandle<WindowsDisplay>  m_Display;
  CcHandle<WindowsTimer>    m_Timer;
  CcHandle<WindowsService>  m_Service;
  CcFileSystemHandle    m_Filesystem;
  bool m_GuiInitialized;
  bool m_bSystemState;
  CcString m_sConfigDir;
  CcString m_sDataDir;
  CcString m_sBinaryDir;
  CcString m_sWorking;
  CcString m_sTempDir;
};

#endif /* CcSystem_H_ */
