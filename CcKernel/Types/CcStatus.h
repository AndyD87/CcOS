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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStatus
 **/
#ifndef H_CcStatus_H_
#define H_CcStatus_H_

#include "CcBase.h"
#include "CcBase.h"

//! Forward Declaration
class CcString;

enum class EStatus : uint32
{
  AllOk           = 0x00000000, //!< No error occured, same as NoError
  NoError         = 0x00000000, //!< No error occured, same as AllOk
  Success         = 0x00000000, //!< No error occured, all succeeded, same as AllOk
  Error           = 0x00000001, //!< Common Error indication if not specified, also set if error is set by boolean
  NotSupported    = 0x00000002, //!< Common Error a not supported operation
  InvalidHandle   = 0x00000003, //!< Common Error for usage of an invalid handle
  TimeoutReached  = 0x00000004, //!< Common Error for reached timeout
  MoreProcessingRequired = 0x00000005, //!< Common Error for processes has to be continued
  CreateFailed    = 0x00000006, //!< Creating an object did not finish as expected.
  CloseFailed     = 0x00000007, //!< Closing a connection or File did not finish as expected.
  OutOfRange      = 0x00000008, //!< Current request was out of range
  AlreadyStopped  = 0x00000009, //!< Device or thread ist already stopped, no need to stop again
  AlreadyStarted  = 0x0000000a, //!< Device or thread ist already started, no need to start again
  AlreadyRunning  = 0x0000000b, //!< Device or thread ist already running, no need to start again
  NotFound        = 0x0000000c, //!< Searched item was not found
  NotStarted      = 0x0000000d, //!< Device or thread is not started but is tried to be used.

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
  FSDirNotFound,
  FSDirNotEmpty,
  FSDirAlreadyExists,
  FSDirCreateFailed,

  NetworkError        = 0x00040000, //!< Unspecified Network error occured
  FileTransferFailed,
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
   * @brief Constructor
   */
  CcStatus(bool bTrueFalse) : m_eError(bTrueFalse ? EStatus::AllOk : EStatus::Error)
    {}
  
  /**
   * @brief Constructor
   */
  CcStatus(uint uiError)
    {operator=(uiError);}

  /**
   * @brief Constructor
   */
  CcStatus(int iError)
    {operator=(iError);}

  /**
   * @brief Constructor
   */
  CcStatus(EStatus eError)
    {operator=(eError);}

  /**
   * @brief Constructor
   */
  CcStatus(const CcStatus& oStatus) : m_eError(oStatus.m_eError)
  {}            

#ifdef WINDOWS
  CcStatus(unsigned long iErrorCode) : m_eError((EStatus) iErrorCode)
    {}
#endif 

  /**
   * @brief Destructor
   */
  ~CcStatus() = default;

  
  inline uint getErrorUint() const
    { return (uint)m_eError; }
  inline int getErrorInt() const
    { return (int)m_eError; }
  inline EStatus getError() const
    { return m_eError; }
  inline uint32 getSystemError()
    { return static_cast<uint32>(m_eError) & (~static_cast<uint32>(EStatus::SystemError));}
  inline CcStatus& setError(EStatus eError)
    { m_eError = eError; return *this;}
  inline CcStatus& setError(uint32 eError)
    { m_eError = static_cast<EStatus>(eError); return *this;}
  inline CcStatus& setSystemError(int iError)
    { m_eError = static_cast<EStatus>(static_cast<uint32>(EStatus::SystemError) | iError) ; return *this;}
  inline CcStatus& setSystemError(uint32 uiError)
    { m_eError = static_cast<EStatus>(static_cast<uint32>(EStatus::SystemError) | uiError); return *this; }
#ifdef WINDOWS
  inline CcStatus& setSystemError(unsigned int uiError)
    { m_eError = static_cast<EStatus>(uiError > static_cast<uint32>(EStatus::SystemError) ? uiError: ~uiError + 1); return *this; }
#endif

  bool isSystemError() const
    { return m_eError > EStatus::SystemError; }

  CcStatus& operator=(const CcStatus& oError);
  CcStatus& operator=(EStatus eError);
  CcStatus& operator=(bool bTrueFalse);
  CcStatus& operator=(int iErrorCode);
  CcStatus& operator=(uint iErrorCode);
#ifdef WINDOWS
  CcStatus& operator=(unsigned long iErrorCode);
#endif 

  bool operator==(EStatus eError) const
    { return m_eError == eError; }
  bool operator!=(EStatus eError) const
    { return m_eError != eError; }

  inline operator bool() const
    { return m_eError == EStatus::AllOk; }

private:
  EStatus m_eError = EStatus::AllOk;
};

#endif // H_CcStatus_H_
