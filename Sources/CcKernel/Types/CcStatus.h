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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStatus
 **/
#pragma once

#include "CcBase.h"

//! Forward Declaration
class CcString;

/**
 * @brief Status enumerations for CcStatus
 */
enum class EStatus : uint32
{
  AllOk                   = 0x00000000, //!< No error occured, same as NoError
  NoError                 = 0x00000000, //!< No error occured, same as AllOk
  Success                 = 0x00000000, //!< No error occured, all succeeded, same as AllOk
  Error                   = 0x00000001, //!< Common Error indication if not specified, also set if error is set by boolean
  NotSupported            = 0x00000002, //!< Common Error a not supported operation
  InvalidHandle           = 0x00000003, //!< Common Error for usage of an invalid handle
  TimeoutReached          = 0x00000004, //!< Common Error for reached timeout
  MoreProcessingRequired  = 0x00000005, //!< Common Error for processes has to be continued
  CreateFailed            = 0x00000006, //!< Creating an object did not finish as expected.
  CloseFailed             = 0x00000007, //!< Closing a connection or File did not finish as expected.
  OutOfRange              = 0x00000008, //!< Current request was out of range
  AlreadyStopped          = 0x00000009, //!< Device or thread ist already stopped, no need to stop again
  AlreadyStarted          = 0x0000000a, //!< Device or thread ist already started, no need to start again
  AlreadyRunning          = 0x0000000b, //!< Device or thread ist already running, no need to start again
  NotFound                = 0x0000000c, //!< Searched item was not found
  NotStarted              = 0x0000000d, //!< Device or thread is not started but is tried to be used.
  NotPermitted            = 0x0000000e, //!< Current operation is not permitted
  AccessDenied            = 0x0000000f, //!< Access denied
  AlreadyExisting         = 0x00000010, //!< Object already existing
  NotConnected            = 0x00000011, //!< Object is in an unconnected state

  IoDeviceError   = 0x00020000, //!< Unspecified IO Device error occured

  FileSystemError     = 0x00030000, //!< Unspecified FileSystem error occured
  FileSystemNotFound,
  FSFileError         = 0x00031000,
  FSFileNotFound,
  FSFileCreateFailed,
  FSFileAlreadyExisting,
  FSFileMoveFailed,
  FSFileRemoveFailed,
  FSFileCrcFailed,
  FSDirError          = 0x00032000,
  FSDirNotEmpty,

  NetworkError        = 0x00040000, //!< Unspecified Network error occured
  FileTransferFailed,
  NetworkHostNotReachable,
  NetworkPortInUse,

  UserError           = 0x00050000, //!< Unspecified User error occured
  UserNotFound,
  UserLoginFailed,
  UserSessionExpiered,
  UserAccessDenied,
  LoginFailed,
  CommandError        = 0x00060000, //!< Unspecified Command error occured
  CommandUnknown,
  CommandUnknownParameter,
  CommandInvalidParameter,
  CommandRequiredParameter,
  CommandNotImplemented,

  ConfigError  = 0x00070000,        //!< Unspecified Configuration error occured
  ConfigFileNotFound,
  ConfigFolderNotFound,
  ConfigReadFailed,

  DeviceError  = 0x00080000,        //!< Unspecified Device error occured
  DeviceNotRunning,
  SystemError         = 0x80000000, //!< Unspecified System error occured. 
                                    //!  System error ared ORed with this value.
};

/**
 * @brief Status class
 *        This class is designed to exchange status informations by number.
 *        The numbers can be found in enum class EStatus.
 *
 *        ## CcStatus as boolean
 *
 *        Additionaly to error numbers, CcStatus can operate like a boolean.
 *        On assignments:
 *          - if true will assigned to CcStatus, CcStatus becomes the number EStatus::AllOk
 *          - if false will assigned to CcStatus, CcStatus becomes the number EStatus::Error
 *
 *        In conditional expressions like "if", CcStatus will compare against 0.
 *        So if no error number is set, the condition is true, otherwise false.
 */
class CcKernelSHARED CcStatus 
{
public:
  /**
   * @brief Constructor
   */
  CcStatus() = default;
  
  /**
   * @brief Initialize status with bool
   * @param bTrueFalse: True for AllOk, False for error
   */
  CcStatus(bool bTrueFalse) : m_eError(bTrueFalse ? EStatus::AllOk : EStatus::Error)
    {}
  
  /**
   * @brief Initialize status with unsigned integer
   * @param uiError: Value to set
   */
  CcStatus(uint uiError)
    {operator=(uiError);}

  /**
   * @brief Initialize status with integer
   * @param iError: Value to set
   */
  CcStatus(int iError)
    {operator=(iError);}

  /**
   * @brief Initialize status with enumerated status
   * @param eError: Status from enumberation
   */
  CcStatus(EStatus eError)
    {operator=(eError);}

  /**
   * @brief Initialize status by copy from another status
   * @param oStatus: Status to copy from
   */
  CcStatus(const CcStatus& oStatus) : m_eError(oStatus.m_eError)
  {}            

#ifdef WINDOWS
  /**
   * @brief Initialize status with unsigned long (DWORD)
   * @param iErrorCode: Value to set
   */
  CcStatus(unsigned long iErrorCode) : m_eError((EStatus) iErrorCode)
    {}
#endif 

  /**
   * @brief Destructor
   */
  ~CcStatus() = default;

  //! @return Get current error code as uint
  inline uint getErrorUint() const
  { return (uint)m_eError; }
  //! @return Get current error code as int
  inline int getErrorInt() const
  { return (int)m_eError; }
  //! @return Get current error code
  inline EStatus getError() const
  { return m_eError; }
  //! @return Get current error as string
  const CcString& getString() const ;
  //! @return Get system error by strapping system error flags
  inline uint32 getSystemError() const
  { return static_cast<uint32>(m_eError) & (~static_cast<uint32>(EStatus::SystemError));}
  //! @return True if system error flags are set
  bool isSystemError() const
  { return m_eError > EStatus::SystemError; }

  //! @param eError: Set new error code by enum
  inline CcStatus& setError(EStatus eError)
  { m_eError = eError; return *this;}
  //! @param eError: Set new error code by uint32
  inline CcStatus& setError(uint32 eError)
  { m_eError = static_cast<EStatus>(eError); return *this;}
  //! @param iError: Set system error and add system error flags
  inline CcStatus& setSystemError(int iError)
  { return setSystemError(static_cast<uint32>(iError));}
#ifdef WINDOWS
  //! @param uiError: Set system error and add system error flags on windows with DWORD
  inline CcStatus& setSystemError(unsigned int uiError)
  { return setSystemError(static_cast<uint32>(uiError));}
#endif
  //! @param uiError: Set system error and add system error flags
  CcStatus& setSystemError(uint32 uiError);

  //! @param oError: Set from other Status
  //! @return Handle to this;
  CcStatus& operator=(const CcStatus& oError);
  //! @param eError: Set new error code by enum
  //! @return Handle to this;
  CcStatus& operator=(EStatus eError);
  //! @param bTrueFalse: Set new error code by bool
  //! @return Handle to this;
  CcStatus& operator=(bool bTrueFalse);
  //! @param iErrorCode: Set new error code by integer
  //! @return Handle to this;
  CcStatus& operator=(int iErrorCode);
  //! @param iErrorCode: Set new error code by unsigned integer
  //! @return Handle to this;
  CcStatus& operator=(uint iErrorCode);
#ifdef WINDOWS
  CcStatus& operator=(unsigned long iErrorCode);
#endif 

  /**
   * @brief Compare this status with enumerated status
   * @param eError: Status to compare with
   * @return True if status is same
   */
  bool operator==(EStatus eError) const
  { return m_eError == eError; }

  /**
   * @brief Compare this status with enumerated status
   * @param eError: Status to compare with
   * @return True if status is not same
   */
  bool operator!=(EStatus eError) const
  { return m_eError != eError; }

  //! @return Convert to bool, just EStatus::AllOk will generate true.
  inline operator bool() const
  { return m_eError == EStatus::AllOk; }

private:
  EStatus m_eError = EStatus::AllOk;
};
