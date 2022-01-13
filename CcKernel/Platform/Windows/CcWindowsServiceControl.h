#pragma once

#include "CcWString.h"
#include <winsvc.h>

class CcArguments;

/**
 * @brief Manage a windows service
 */
class CcWindowsServiceControl
{
public:
  /**
   * @brief Service start type as enumeration
   */
  enum class EStartType : uint32
  {
    BootStart = SERVICE_BOOT_START,
    SystemStart = SERVICE_SYSTEM_START,
    AutoStart = SERVICE_AUTO_START,
    DemandStart = SERVICE_DEMAND_START,
    Disabled = SERVICE_DISABLED
  };

  /**
   * @brief Create service control instance by service name
   * @param sName: Name of service
   */
  CcWindowsServiceControl(const CcWString& sName, bool bOpenReadOnly = false);
  ~CcWindowsServiceControl();

  /**
   * @brief Change display name of service
   * @param sName:  New name
   * @return True if succeeded
   */
  bool setDisplayName(const CcWString& sName);

  /**
   * @brief Change Arguments for service
   * @param oArguments: New arguments for service
   * @return True if succeeded
   */
  bool setArguments(const CcArguments& sName);

  /**
   * @brief Change start type of service
   * @param eStartType: New start type of service as enum
   * @return True if succeeded
   */
  bool setStartType(CcWindowsServiceControl::EStartType eStartType);

  /**
   * @brief Set username of service runtime, correct password of user is required.
   * @param sName:      Target username of service environment
   * @param sPassword:  Password of user
   * @return True if user changed
   */
  bool setUsername(const CcWString& sName, const CcWString& sPassword);

  /**
   * @brief Set new access rights of service
   * @param uiAccessRights: Access flags
   * @return True if successfully changed
   */
  bool setServiceAccessRigths(unsigned int uiAccessRights);

  /**
   * @brief Add other services as dependency before this servic will be started.
   * @param sName: Name service to add as dependency
   * @return True if dependency added
   */
  bool addDependency(const CcWString& sName);

  static CcStringList getAllServices();

  CcStatus  setExectuable(const CcString& sExePath);

  /**
   * @brief Create service if not already existing.
   * @return True if creation succeeded
   */
  bool create();

  /**
   * @brief Remove an existing service from system
   * @return True if removing succeeded
   */
  bool remove();

  /**
   * @brief Stop service instance
   * @return True if removing succeeded
   */
  bool start();

  /**
   * @brief Stop service instance
   * @return True if removing succeeded
   */
  bool stop();

  /**
   * @brief Open service if already existing.
   * @return True if service was opened succeeded
   */
  bool open(bool bOpenReadOnly);

  /**
   * @brief Create service or open it.
   * @return True if creation or open succeeded
   */
  bool openOrCreate()
  { return open(false) || create();}

  /**
   * @brief Update current config with system
   * @return True if successfully updated.
   */
  bool updateConfig();

  uint32 getStatus();

private:
  bool serviceManagerAvailable();
  bool serviceOpened();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
  CcWString m_sName;
  CcWString m_sDisplayName;
  CcWindowsServiceControl::EStartType m_eStartType = CcWindowsServiceControl::EStartType::Disabled;
};

