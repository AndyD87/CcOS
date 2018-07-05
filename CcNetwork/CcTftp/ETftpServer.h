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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTftpTypes
 */
#ifndef _CcTftpTypes_H_
#define _CcTftpTypes_H_

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
  Termination       = 8, //!< Termination        
  Unknown           = 9  //!< Unknown Error    
};

enum class ETftpServerTransferType : uint8
{
  octet = 0, //!< NoError
  netascii,
  mail
};

#endif /* _CcTftpTypes_H_ */
