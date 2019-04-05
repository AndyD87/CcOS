#pragma once
#include "CcWString.h"


enum class EWindowsServiceStartType : DWORD
{
  BootStart   = SERVICE_BOOT_START,
  SystemStart = SERVICE_SYSTEM_START,
  AutoStart   = SERVICE_AUTO_START,
  DemandStart = SERVICE_DEMAND_START,
  Disabled    = SERVICE_DISABLED
};

class CcWindowsServiceControl
{
public:
  CcWindowsServiceControl(const CcWString& sName);
  ~CcWindowsServiceControl();

  bool setDisplayName(const CcWString& sName);
  bool setStartType(EWindowsServiceStartType eStartType);

  bool setUsername(const CcWString& sName, const CcWString& sPassword);
  bool setServiceAccessRigths(unsigned int uiAccessRights);
  bool addDependency(const CcWString& sName);
  
  bool create();
  bool open();
  bool openOrCreate()
    { return open() || create();}

  bool updateConfig();

private:
  bool serviceManagerAvailable();
  bool serviceOpened();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
  CcWString m_sName;
  CcWString m_sDisplayName;
  EWindowsServiceStartType m_eStartType = EWindowsServiceStartType::Disabled;
};

