#pragma once
#include "CcWString.h"
#include <windows.h>

class CcWindowsService
{
public:
  CcWindowsService(const CcWString& sName);
  ~CcWindowsService();

  bool init();
  virtual void onStop();
  virtual void onStart();
  virtual void onPause();
  virtual void onContinue();
  virtual void onShutdown();
  virtual DWORD onCommand(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData);
  virtual DWORD onCustomCommand(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData);

protected: // Methods
  DWORD command(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData);
  void setStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode = NO_ERROR, DWORD dwWaitHint = 0);
  static void WINAPI serviceMain(DWORD dwArgc, LPWSTR *lpszArgv);
  static DWORD WINAPI serviceCtrlHandler(DWORD dwControl, DWORD dwEventType, PVOID pEventData, PVOID pContext);
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

