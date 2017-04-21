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
 * @page      CcTftp
 * @subpage   ETftpServer
 *
 * @page      ETftpServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTftpTypes
 */
#ifndef CcTftpTypes_H_
#define CcTftpTypes_H_

#include "CcBase.h"
#include "CcString.h"

#ifdef ERROR
  #undef ERROR
#endif
enum class ETftpServerCommands : uint16
{
  Unkown= 0, //!< Unkown Kommand, Error
  RRQ   = 1, //!< Read Request
  WRQ   = 2, //!< Write Request
  DATA  = 3, //!< Read or write the next block of data
  ACK   = 4, //!< Acknowledgement
  ERROR = 5, //!< Error message
  OACK  = 6, //!< Options acknowledgment
};

enum class ETftpServerErrors : uint8
{
  NoError           = 0, //!< NoError          
  FileNotFound      = 1, //!< FileNotFound     
  AccessViolation   = 2, //!< AccessViolation  
  DiskFull          = 3, //!< DiskFull         
  IllegalOperation  = 4, //!< IllegalOperation 
  UnknownTransfer   = 5, //!< UnknownTransfer  
  FileAlreadyExists = 6, //!< FileAlreadyExists
  NoSuchUser        = 7, //!< NoSuchUser       
  Termination       = 8  //!< Termination      
};

namespace STftpServerErrorMessages
{
  static const CcString NoError           ("NoError");
  static const CcString FileNotFound      ("FileNotFound");
  static const CcString AccessViolation   ("AccessViolation");
  static const CcString DiskFull          ("DiskFull");
  static const CcString IllegalOperation  ("IllegalOperation");
  static const CcString UnknownTransfer   ("UnknownTransfer");
  static const CcString FileAlreadyExists ("FileAlreadyExists");
  static const CcString NoSuchUser        ("NoSuchUser");
  static const CcString Termination       ("Termination");
};

enum class ETftpServerTransferType : uint8
{
  octet = 0, //!< NoError
  netascii,
  mail
};

#endif /* CcTftpTypes_H_ */
