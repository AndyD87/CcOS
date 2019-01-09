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
 * @page      Types
 * @subpage   CcStatus
 *
 * @page      CcStatus
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStatus
 **/
#ifndef _CcStatus_H_
#define _CcStatus_H_

#include "CcKernelBase.h"
#include "CcBase.h"

//! Forward Declaration
class CcString;

enum class EStatus : uint32
{
  AllOk           = 0x00000000,
  NoError         = 0x00000000,
  Success         = 0x00000000,
  Error           = 0x00000001, //!< Common Error indication if not specified, also set if error is set by boolean
  NotSupported    = 0x00000002, //!< Common Error a not supported operation
  InvalidHandle   = 0x00000003, //!< Common Error for usage of an invalid handle
  TimeoutReached  = 0x00000003, //!< Common Error for reached timeout

  IoDeviceError = 0x00020000,

  FileSystemError     = 0x00030000,
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
  NetworkError        = 0x00040000,
  FileTransferFailed,
  UserError           = 0x00050000,
  UserNotFound,
  UserLoginFailed,
  UserSessionExpiered,
  UserAccessDenied,
  LoginFailed,
  CommandError        = 0x00060000,
  CommandUnknown,
  CommandUnknownParameter,
  CommandInvalidParameter,
  CommandRequiredParameter,
  CommandNotImplemented,
  ConfigError  = 0x0006000,
  ConfigFileNotFound,
  ConfigFolderNotFound,
  SystemError         = 0x80000000,
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
  CcStatus(void) 
    {}
  
  /**
   * @brief Constructor
   */
  CcStatus(bool bTrueFalse) : m_eError(bTrueFalse ? EStatus::AllOk : EStatus::Error)
    {}
  
  /**
   * @brief Constructor
   */
  CcStatus(uint32 uiError) : m_eError((EStatus) uiError)
    {}

  /**
   * @brief Constructor
   */
  CcStatus(int iError) : m_eError((EStatus) iError)
    {}

  /**
   * @brief Constructor
   */
  CcStatus(EStatus eError) : m_eError(eError)
    {}

  /**
   * @brief Destructor
   */
  ~CcStatus(void )
    {}
  
  inline uint getErrorUint() const
    { return (uint)m_eError; }
  inline int getErrorInt() const
    { return (int)m_eError; }
  inline EStatus getError() const
    { return m_eError; }
  inline CcStatus& setError(EStatus eError)
    { m_eError = eError; return *this;}
  inline CcStatus& setError(uint32 eError)
    { m_eError = static_cast<EStatus>(eError); return *this;}
  inline CcStatus& setSystemError(int iError)
    { m_eError = static_cast<EStatus>(iError < 0 ? iError : -iError) ; return *this;}
  inline CcStatus& setSystemError(uint32 uiError)
    { m_eError = static_cast<EStatus>(uiError > static_cast<uint32>(EStatus::SystemError) ? uiError: ~uiError + 1); return *this; }

  bool isSystemError() const
    { return m_eError > EStatus::SystemError; }

  CcStatus& operator=(const CcStatus& oError);
  CcStatus& operator=(EStatus eError);
  CcStatus& operator=(bool bTrueFalse);
  CcStatus& operator=(int iErrorCode);

  operator bool()
    { return m_eError == EStatus::AllOk; }
  
private:
  EStatus m_eError = EStatus::AllOk;
};

#endif /* _CcStatus_H_ */
