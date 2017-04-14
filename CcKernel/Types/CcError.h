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
 * @page      CcError
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcError
 **/
#ifndef CcError_H_
#define CcError_H_

#include "CcKernelBase.h"
#include "CcBase.h"

//! Forward Declaration
class CcString;

enum class EError : uint32
{
  AllOk         = 0x00000000,
  SystemError   = 0x00010000,
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
  CustomError         = 0x80000000,
};

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcError {
public:
  /**
   * @brief Constructor
   */
  CcError(void) 
    {}
  
  /**
   * @brief Constructor
   */
  CcError(uint32 uiError) : m_eError((EError) uiError)
    {}

  /**
   * @brief Constructor
   */
  CcError(EError eError) : m_eError(eError)
    {}

  /**
   * @brief Destructor
   */
  ~CcError( void )
    {}
  
  inline uint32 getErrorUnit() const
    { return (uint32)m_eError; }
  inline EError getError() const
    { return m_eError; }
  inline void setError(EError eError)
    { m_eError = eError; }
  inline void setError(uint32 eError)
    { m_eError = (EError)eError; }

  void setWindowsError(uint32 uiErrorCode);

  CcError& operator=(const CcError& oError);
  CcError& operator=(EError eError);

private:
  EError m_eError = EError::AllOk;
};

#endif /* CcError_H_ */
