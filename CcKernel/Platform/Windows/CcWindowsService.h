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
 * @copyright Andreas Dirmeier (C) 2021
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsService
 */
#pragma once

#include "CcWindowsGlobals.h"
#include "CcWString.h"

/**
 * @brief Service class, it has to be adopted to  CcOS
 */
class CcWindowsService
{
public:
  /**
   * @brief Create Service by name
   * @param sName: Target name of service to identify it in system
   */
  CcWindowsService(const CcWString& sName);
  ~CcWindowsService();

  /**
   * @brief Initialize Servce
   * @return True if init succeeded
   */
  bool init();

  /**
   * @brief Stop command received
   */
  virtual void onStop();

  /**
   * @brief Start command received
   */
  virtual void onStart();

  /**
   * @brief Pause command received
   */
  virtual void onPause();

  /**
   * @brief Continue command received
   */
  virtual void onContinue();

  /**
   * @brief System shutdown command received
   */
  virtual void onShutdown();

  /**
   * @brief Incomming system command like device or system actions
   * @param dwCtrl:       Control code like IO-Contorls
   * @param dwEventType:  Additional Informations
   * @param pEventData:   Data if available
   * @return 0 on Success otherwise error message
   */
  DWORD onCommand(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData);

  /**
   * @brief Incomming system was a custom command, so it is forwarded to this
   *        Method
   * @param dwCtrl:       Control code like IO-Contorls
   * @param dwEventType:  Additional Informations
   * @param pEventData:   Data if available
   * @return 0 on Success otherwise error message
   */
  DWORD onCustomCommand(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData);

protected: // Methods
  /**
   * @brief System command receiving method. Informations are forwarded to onCommand or onCustomCommand.
   * @param dwCtrl:       Control code like IO-Contorls
   * @param dwEventType:  Additional Informations
   * @param pEventData:   Data if available
   * @return 0 on Success otherwise error message
   */
  DWORD command(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData);

  /**
   * @brief If service status has changed internaly, it will be set here.
   * @param dwCurrentState:   New state
   * @param dwWin32ExitCode:  Exit code if service stopped.
   * @param dwWaitHint        Wait if required.
   */
  void setStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode = NO_ERROR, DWORD dwWaitHint = 0);

  /**
   * @brief Main entry point for Service, called from service manager on startup.
   * @param dwArgc: Count of additional arguments for server
   * @param lpszArgv: Additional arguments for server
   */
  static void WINAPI serviceMain(DWORD dwArgc, LPWSTR *lpszArgv);

  /**
   * @brief Static interface for windows to submich service commands. They will be forwarded
   *        To Service Object, which is set in pContext
   * @param dwControl:    Control code like IO-Contorls
   * @param dwEventType:  Additional Informations
   * @param pEventData:   Data if available
   * @param pContext:     Context data of service object.
   * @return 0 on Success otherwise error message
   */
  static DWORD WINAPI serviceCtrlHandler(DWORD dwControl, DWORD dwEventType, PVOID pEventData, PVOID pContext);

  //! @return Get service handle
  SERVICE_STATUS_HANDLE getServiceHandle();


private:
  void setStatusHandle(SERVICE_STATUS_HANDLE hStatus);
  void start();
  void stop();
  void pause();
  void cont();
  void shutdown();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
  CcWString m_sName;

  static CcWindowsService* s_pService;
};

