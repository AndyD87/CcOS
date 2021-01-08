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
 * @brief    CcFtpTypes values
 */

#include "CcFtpTypes.h"

sFtpCommandMap sFtpCommandList[] =
{
  { FTP_OPTS, "OPTS" },
  { FTP_USER, "USER" },
  { FTP_PASS, "PASS" },
  { FTP_AUTH, "AUTH" },
  { FTP_SYST, "SYST" },
  { FTP_FEAT, "FEAT" },
  { FTP_CWD,  "CWD"  },
  { FTP_PWD,  "PWD"  },
  { FTP_NLST, "NLST" },
  { FTP_TYPE, "TYPE" },
  { FTP_PASV, "PASV" },
  { FTP_LIST, "LIST" },
  { FTP_NLST, "NLST" },
  { FTP_CDUP, "CDUP" },
  { FTP_RETR, "RETR" },
  { FTP_RNFR, "RNFR" },
  { FTP_RNTO, "RNTO" },
  { FTP_STOR, "STOR" },
  { FTP_MKD,  "MKD"  },
  { FTP_RMD,  "RMD"  },
  { FTP_DELE, "DELE" },
  { FTP_PORT, "PORT" },
  { FTP_SIZE, "SIZE" },
  { FTP_MDTM, "MDTM" },
};

size_t sFtpCommandListSize = sizeof(sFtpCommandList) / sizeof(sFtpCommandMap);
