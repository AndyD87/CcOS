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

//forward declarations
class CcProcess;
class CcThreadObject;
class CcUserList;

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
  CcDateTime getDateTime( void );
  void sleep(uint32 timeoutMs);
  CcHandle<CcDevice> getDevice(EDeviceType Type, const CcString &Name);
  CcUserList getUserList(void);

  const CcString& getConfigDir() const
    { return m_sConfigDir; }
  const CcString& getDataDir() const
    { return m_sDataDir; }
  const CcString& getBinaryDir() const
    { return m_sBinaryDir; }
  const CcString& getWorkingDir(void) const
    { return m_sWorkingDir; }
  const CcString& getTemporaryDir(void) const
    { return m_sTempDir; }

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

  CcString m_sWorkingDir;
  CcString m_sConfigDir;
  CcString m_sDataDir  ;
  CcString m_sBinaryDir;
  CcString m_sTempDir;
};

#endif /* CcTargetSystem_H_ */
