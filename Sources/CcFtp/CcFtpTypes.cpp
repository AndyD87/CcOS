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

const CcFtpCommandMap g_oFtpCommandList[24] =
{
  { EFtpCommands::FTP_OPTS, "OPTS" },
  { EFtpCommands::FTP_USER, "USER" },
  { EFtpCommands::FTP_PASS, "PASS" },
  { EFtpCommands::FTP_AUTH, "AUTH" },
  { EFtpCommands::FTP_SYST, "SYST" },
  { EFtpCommands::FTP_FEAT, "FEAT" },
  { EFtpCommands::FTP_CWD,  "CWD"  },
  { EFtpCommands::FTP_PWD,  "PWD"  },
  { EFtpCommands::FTP_NLST, "NLST" },
  { EFtpCommands::FTP_TYPE, "TYPE" },
  { EFtpCommands::FTP_PASV, "PASV" },
  { EFtpCommands::FTP_LIST, "LIST" },
  { EFtpCommands::FTP_NLST, "NLST" },
  { EFtpCommands::FTP_CDUP, "CDUP" },
  { EFtpCommands::FTP_RETR, "RETR" },
  { EFtpCommands::FTP_RNFR, "RNFR" },
  { EFtpCommands::FTP_RNTO, "RNTO" },
  { EFtpCommands::FTP_STOR, "STOR" },
  { EFtpCommands::FTP_MKD,  "MKD"  },
  { EFtpCommands::FTP_RMD,  "RMD"  },
  { EFtpCommands::FTP_DELE, "DELE" },
  { EFtpCommands::FTP_PORT, "PORT" },
  { EFtpCommands::FTP_SIZE, "SIZE" },
  { EFtpCommands::FTP_MDTM, "MDTM" },
};

const size_t g_uiFtpCommandListSize = sizeof(g_oFtpCommandList) / sizeof(CcFtpCommandMap);
