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
#include "CcGlobalStrings.h"

class WindowsTimer;
class CcThreadObject;
class CcProcess;
class CcUserList;
class CcDateTime;
class CcSocket;
enum class ESocketType;
class CcFileSystem;
class CcDevice;
enum class EDeviceType;

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

  /**
   * @brief Load a User List from System if Users are availbale
   * @param UserList: UserList to appen the System Users
   * @return true, if System has Users to store
   */
  CcHandle<CcFileSystem> getFileSystemManager(void);

  CcHandle<CcDevice> getDevice(EDeviceType Type, const CcString& Name)
    { CCUNUSED(Type); CCUNUSED(Name); return nullptr; }

  const CcString& getConfigDir() const
    { return CcGlobalStrings::Empty; }
  const CcString& getDataDir() const
    { return CcGlobalStrings::Empty; }
  const CcString& getBinaryDir() const
    { return CcGlobalStrings::Empty; }
  const CcString& getWorkingDir(void) const
    { return CcGlobalStrings::Empty; }
  const CcString& getTemporaryDir(void) const
    { return CcGlobalStrings::Empty; }

protected:

private:
  bool m_bSystemState = false;
};

#endif /* CcSystem_H_ */
