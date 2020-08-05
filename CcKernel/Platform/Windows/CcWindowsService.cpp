#include "CcWindowsService.h"
#include "CcStatic.h"

CcWindowsService* CcWindowsService::s_pService = nullptr;

class CcWindowsService::CPrivate
{
public:
  SERVICE_STATUS oStaus;
  SERVICE_STATUS_HANDLE hStatus = NULL;
};

CcWindowsService::CcWindowsService(const CcWString& sName) : 
  m_sName(sName)
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->hStatus = NULL;

  CcStatic::memset(&m_pPrivate->oStaus, 0, sizeof(m_pPrivate->oStaus));

  // The service runs in its own process.
  m_pPrivate->oStaus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

  // The service is starting.
  m_pPrivate->oStaus.dwCurrentState = SERVICE_START_PENDING;

  // The accepted commands of the service.
  DWORD dwControlsAccepted = 0;
  dwControlsAccepted |= SERVICE_ACCEPT_STOP;
  dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
  dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;
  m_pPrivate->oStaus.dwControlsAccepted = dwControlsAccepted;

  m_pPrivate->oStaus.dwWin32ExitCode = NO_ERROR;
  m_pPrivate->oStaus.dwServiceSpecificExitCode = 0;
  m_pPrivate->oStaus.dwCheckPoint = 0;
  m_pPrivate->oStaus.dwWaitHint = 0;
}

CcWindowsService::~CcWindowsService()
{
  CCDELETE(m_pPrivate);
}

bool CcWindowsService::init()
{
  bool bRet = false;

  s_pService = this;

  SERVICE_TABLE_ENTRYW serviceTable[] =
  {
    {const_cast<wchar_t*>(m_sName.getWcharString()) , CcWindowsService::serviceMain},
    {NULL, NULL}
  };
  if (StartServiceCtrlDispatcherW(serviceTable) == FALSE)
  {
    DWORD uError = GetLastError();
    if (uError == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
    {
      // We are running on console not as service
      bRet = true;
      s_pService->serviceMain(0, nullptr);
    }
  }
  else
  {
    bRet = true;
  }
  return bRet;
}

void CcWindowsService::onStop()
{

}

void CcWindowsService::onStart()
{
}

void CcWindowsService::onPause()
{

}

void CcWindowsService::onContinue()
{
}

void CcWindowsService::onShutdown()
{
}

DWORD CcWindowsService::onCustomCommand(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData)
{
  CCUNUSED(dwCtrl);
  CCUNUSED(dwEventType);
  CCUNUSED(pEventData);
  return TYPE_MAX(DWORD);
}

DWORD CcWindowsService::onCommand(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData)
{
  CCUNUSED(dwCtrl);
  CCUNUSED(dwEventType);
  CCUNUSED(pEventData);
  return TYPE_MAX(DWORD);
}


DWORD CcWindowsService::command(DWORD dwCtrl, DWORD dwEventType, PVOID pEventData)
{
  DWORD dwRet = ~((DWORD) 0);
  bool bWasCustom = false;
  switch (dwCtrl)
  {
    case SERVICE_CONTROL_STOP:      stop(); break;
    case SERVICE_CONTROL_PAUSE:     pause(); break;
    case SERVICE_CONTROL_CONTINUE:  cont(); break;
    case SERVICE_CONTROL_SHUTDOWN:  shutdown(); break;
    case SERVICE_CONTROL_INTERROGATE: break;
    default: 
      bWasCustom = true;
      onCustomCommand(dwCtrl, dwEventType, pEventData);
  }
  if (bWasCustom == false)
  {
    dwRet = onCommand(dwCtrl, dwEventType, pEventData);
  }
  return dwRet;
}

void CcWindowsService::setStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
  static DWORD dwCheckPoint = 1;

  // Fill in the SERVICE_STATUS structure of the service.

  m_pPrivate->oStaus.dwCurrentState = dwCurrentState;
  m_pPrivate->oStaus.dwWin32ExitCode = dwWin32ExitCode;
  m_pPrivate->oStaus.dwWaitHint = dwWaitHint;

  m_pPrivate->oStaus.dwCheckPoint =
    ((dwCurrentState == SERVICE_RUNNING) ||
    (dwCurrentState == SERVICE_STOPPED)) ?
    0 : dwCheckPoint++;

  // Report the status of the service to the SCM.
  SetServiceStatus(m_pPrivate->hStatus, &m_pPrivate->oStaus);
}

void CcWindowsService::serviceMain(DWORD dwArgc, LPWSTR *lpszArgv)
{
  CCUNUSED(lpszArgv);
  CCUNUSED(dwArgc);
  if (s_pService == nullptr)
  {
    CCNEW(s_pService, CcWindowsService, L"");
  }

  SERVICE_STATUS_HANDLE hStatus = RegisterServiceCtrlHandlerExW(s_pService->m_sName.getWcharString(), CcWindowsService::serviceCtrlHandler, s_pService);
  s_pService->setStatusHandle(hStatus);
  s_pService->start();
}

DWORD  CcWindowsService::serviceCtrlHandler(DWORD dwControl, DWORD dwEventType, PVOID pEventData, PVOID pContext)
{
  DWORD dwRet = ~((DWORD) 0);
  CcWindowsService* pService = static_cast<CcWindowsService*>(pContext);
  if (pService != NULL)
  {
    pService->command(dwControl, dwEventType, pEventData);
  }
  return dwRet;
}

SERVICE_STATUS_HANDLE CcWindowsService::getServiceHandle()
{
  return m_pPrivate->hStatus;
}

void CcWindowsService::setStatusHandle(SERVICE_STATUS_HANDLE hStatus)
{
  m_pPrivate->hStatus = hStatus;
}

void CcWindowsService::start()
{
  // Tell SCM that the service is starting.
  setStatus(SERVICE_START_PENDING);

  // Perform service-specific initialization.
  onStart();

  // Tell SCM that the service is started.
  setStatus(SERVICE_RUNNING);
}

void CcWindowsService::stop()
{
  // Tell SCM that the service is stopping.
  setStatus(SERVICE_STOP_PENDING);

  // Perform service-specific stop operations.
  onStop();

  // Tell SCM that the service is stopped.
  setStatus(SERVICE_STOPPED);
}

void CcWindowsService::pause()
{
  // Tell SCM that the service is pausing.
  setStatus(SERVICE_PAUSE_PENDING);

  // Perform service-specific pause operations.
  onPause();

  // Tell SCM that the service is paused.
  setStatus(SERVICE_PAUSED);
}

void CcWindowsService::cont()
{
  // Tell SCM that the service is resuming.
  setStatus(SERVICE_CONTINUE_PENDING);

  // Perform service-specific continue operations.
  onContinue();

  // Tell SCM that the service is running.
  setStatus(SERVICE_RUNNING);
}

void CcWindowsService::shutdown()
{
  // Perform service-specific continue operations.
  onShutdown();

  // Tell SCM that the service is stopped.
  setStatus(SERVICE_STOPPED);
}

