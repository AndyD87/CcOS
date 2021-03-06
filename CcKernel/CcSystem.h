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
 * @subpage   CcSystem
 *
 * @page      CcSystem
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcSystem
 */
#ifndef H_CcTargetSystem_H_
#define H_CcTargetSystem_H_

#include "CcBase.h"
#include "CcBase.h"
#include "Network/ISocket.h"
#include "IFileSystem.h"
#include "CcFileSystem.h"
#include "CcDeviceList.h"
#include "CcMapCommon.h"

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
class CcDevice;

/**
 * @brief Main System class.
 *        This class is the connection from CcOS universe to Hardware.
 *        Every Platform has to implement this methods.
 *        For Example in Window the File CcWindowsSystem.cpp is doing this.
 */
class CcKernelSHARED CcSystem
{
public:
  CcSystem();
  ~CcSystem();

  void init();
  void deinit();
  bool initGUI();
  bool initCLI();
  bool deinitCLI();
  int  initService();
  bool isAdmin();
  bool createThread(IThread& oThread);
  bool createProcess(CcProcess& oProcessToStart);
  void error();
  void warning();
  ISocket* getSocket(ESocketType eType);
  INetworkStack* getNetworkStack();
  CcString getName();
  CcVersion getVersion();
  CcStringMap getEnvironmentVariables() const;
  CcString getEnvironmentVariable(const CcString& sName) const;
  bool getEnvironmentVariableExists(const CcString& sName) const;
  bool setEnvironmentVariable(const CcString& sName, const CcString& sValue);
  bool removeEnvironmentVariable(const CcString& sName);
  CcDateTime getDateTime();
  CcDateTime getUpTime();
  void sleep(uint32 uiTimeoutMs);
  const CcDevice& getDevice(EDeviceType eType, size_t uiNr);
  const CcDevice& getDevice(EDeviceType eType, const CcString& sName);
  CcUserList getUserList();
  ISharedMemory* getSharedMemory(const CcString& sName, size_t uiSize);

  /**
   * @brief Load a User List from System if Users are availbale
   * @param UserList: UserList to appen the System Users
   * @return true, if System has Users to store
   */
  CcGroupList getGroupList();

  CcString getConfigDir() const;
  CcString getDataDir() const;
  CcString getBinaryDir() const;
  CcString getWorkingDir() const;
  CcString getTemporaryDir() const;
  CcString getUserDir() const;
  CcString getUserDataDir() const;

  CcStatus loadModule(const CcString& sPath, const IKernel& oKernel);

  CcStatus setWorkingDir(const CcString& sPath);

  void registerForIdle(IDevice* pDevice);
  bool deregisterForIdle(IDevice* pDevice);
public: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};

#endif // H_CcTargetSystem_H_
