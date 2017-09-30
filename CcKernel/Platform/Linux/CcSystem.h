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
 * @page      Linux
 * @subpage   CcSystem
 * 
 * @page      CcSystem
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSystem
 */
#ifndef CcTargetSystem_H_
#define CcTargetSystem_H_

#include "CcBase.h"
#include "Network/CcSocket.h"
#include "CcFileSystemAbstract.h"
#include "CcFileSystem.h"
#include "CcDeviceList.h"
#include "CcMapCommon.h"

//forward declarations
class CcProcess;
class CcThreadObject;
class CcUserList;
class CcGroupList;

class CcSystem
{
public:
  CcSystem();
  ~CcSystem();

  void init(void);
  bool start( void );
  bool stop();
  bool initGUI(void);
  bool initCLI(void);
  int  initService();
  bool createThread(CcThreadObject& object);
  bool createProcess(CcProcess& oProcessToStart);
  CcSocket* getSocket(ESocketType type);

  CcStringMap getEnvironmentVariables() const;
  CcString getEnvironmentVariable(const CcString& sName) const;
  bool getEnvironmentVariableExists(const CcString& sName) const;
  bool setEnvironmentVariable(const CcString& sName, const CcString& sValue);
  bool removeEnvironmentVariable(const CcString& sName);

  CcDateTime getDateTime( void );
  void sleep(uint32 timeoutMs);
  CcHandle<CcDevice> getDevice(EDeviceType Type, const CcString &Name);
  CcUserList getUserList(void);

  /**
   * @brief Load a User List from System if Users are availbale
   * @param UserList: UserList to appen the System Users
   * @return true, if System has Users to store
   */
  CcGroupList getGroupList(void);
  
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
  void initTouch( void );

  void systemTick( void );

private: //Member
  bool m_bSystemState=false;
  CcFileSystemAbstract *m_Filesystem;
  CcDeviceList m_cDeviceList;
};

#endif /* CcTargetSystem_H_ */
