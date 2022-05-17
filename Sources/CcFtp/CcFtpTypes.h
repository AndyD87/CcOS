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
 * @brief     Class CcFtpTypes
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"

/**
 * @brief Enumeration of FTP comands
 */
enum class EFtpCommands
{
  FTP_UNKNOWN = 0,    //!< Undefined command
  FTP_ABOR,           //!<    Abort an active file transfer.
  FTP_ACCT,           //!<    Account information.
  FTP_ADAT,           //!<  RFC 2228  Authentication / Security Data
  FTP_ALLO,           //!<    Allocate sufficient disk space to receive a file.
  FTP_APPE,           //!<    Append.
  FTP_AUTH,           //!<  RFC 2228  Authentication / Security Mechanism
  FTP_CCC,            //!<  RFC 2228  Clear Command Channel
  FTP_CDUP,           //!<    Change to Parent Directory.
  FTP_CONF,           //!<  RFC 2228  Confidentiality Protection Command
  FTP_CWD,            //!<    Change working directory.
  FTP_DELE,           //!<    Delete file.
  FTP_ENC,            //!<  RFC 2228  Privacy Protected Channel
  FTP_EPRT,           //!<  RFC 2428  Specifies an extended address and port to which the server should connect.
  FTP_EPSV,           //!<  RFC 2428  Enter extended passive mode.
  FTP_FEAT,           //!<  RFC 2389  Get the feature list implemented by the server.
  FTP_HELP,           //!<    Returns usage documentation on a command if specified, else a general help document is returned.
  FTP_LANG,           //!<  RFC 2640  Language Negotiation
  FTP_LIST,           //!<    Returns information of a file or directory if specified, else information of the current working directory is returned.
  FTP_LPRT,           //!<  RFC 1639  Specifies a long address and port to which the server should connect.
  FTP_LPSV,           //!<  RFC 1639  Enter long passive mode.
  FTP_MDTM,           //!<  RFC 3659  Return the last - modified time of a specified file.
  FTP_MIC,            //!<  RFC 2228  Integrity Protected Command
  FTP_MKD,            //!<    Make directory.
  FTP_MLSD,           //!<  RFC 3659  Lists the contents of a directory if a directory is named.
  FTP_MLST,           //!<  RFC 3659  Provides data about exactly the object named on its command line, and no others.
  FTP_MODE,           //!<    Sets the transfer mode(Stream, Block, or Compressed).
  FTP_NLST,           //!<    Returns a list of file names in a specified directory.
  FTP_NOOP,           //!<    No operation(dummy packet; used mostly on keepalives).
  FTP_OPTS,           //!<  RFC 2389  Select options for a feature.
  FTP_PASS,           //!<    Authentication password.
  FTP_PASV,           //!<    Enter passive mode.
  FTP_PBSZ,           //!<  RFC 2228  Protection Buffer Size
  FTP_PORT,           //!<    Specifies an address and port to which the server should connect.
  FTP_PROT,           //!<  RFC 2228  Data Channel Protection Level.
  FTP_PWD,            //!<  Print working directory.Returns the current directory of the host.
  FTP_QUIT,           //!<    Disconnect.
  FTP_REIN,           //!<    Re initializes the connection.
  FTP_REST,           //!<    Restart transfer from the specified point.
  FTP_RETR,           //!<    Retrieve a copy of the file
  FTP_RMD,            //!<    Remove a directory.
  FTP_RNFR,           //!<    Rename from.
  FTP_RNTO,           //!<    Rename to.
  FTP_SITE,           //!<    Sends site specific commands to remote server.
  FTP_SIZE,           //!<  RFC 3659  Return the size of a file.
  FTP_SMNT,           //!<    Mount file structure.
  FTP_STAT,           //!<    Returns the current status.
  FTP_STOR,           //!<    Accept the data and to store the data as a file at the server site
  FTP_STOU,           //!<    Store file uniquely.
  FTP_STRU,           //!<    Set file transfer structure.
  FTP_SYST,           //!<    Return system type.
  FTP_TYPE,           //!<    Sets the transfer mode(ASCII / Binary).
  FTP_USER,           //!<    Authentication username.
  FTP_XCUP,           //!<  RFC 775  Change to the parent of the current working directory
  FTP_XMKD,           //!<  RFC 775  Make a directory
  FTP_XPWD,           //!<  RFC 775  Print the current working directory
  FTP_XRCP,           //!<  RFC 743
  FTP_XRMD,           //!<  RFC 775  Remove the directory
  FTP_XRSQ,           //!<  RFC 743
  FTP_XSEM,           //!<  RFC 737  Send, mail if cannot
  FTP_XSEN,           //!<  RFC 737  Send to terminal
};

/**
 * @brief Item of Commandlist for mapping enum and string
 */
class CcFtpCommandMap
{
public:
  /**
   * @brief Initialize command map by enum and matching string
   * @param e:  Command as enum
   * @param c:  Command as char string
   */
  CcFtpCommandMap(EFtpCommands e, const char* c) : eCommand(e), strCommand(c)
  {}

  EFtpCommands eCommand;  //!< Enum of command
  const char *strCommand; //!< String of enumarted command
};

//! @brief CommandListArray, filled in cpp
extern const CcFtpCommandMap  g_oFtpCommandList[24];
//! @brief Size of CommandListArray
extern const size_t           g_uiFtpCommandListSize;

static const CcString FTP_200("200 Command successfully procressed \r\n");        //!< FTP string for code 200
static const CcString FTP_230("230 User login complete \r\n");                    //!< FTP string for code 230
static const CcString FTP_215("215 UNIX Type: l8\r\n");                           //!< FTP string for code 215
static const CcString FTP_211("211 \r\n");                                        //!< FTP string for code 211
static const CcString FTP_226("226 Transfer complete. \r\n");                     //!< FTP string for code 226
static const CcString FTP_250("250 Requested file action okay, completed.\r\n");  //!< FTP string for code 250

static const CcString FTP_425("425 no connection established\r\n");               //!< FTP error string 425
static const CcString FTP_426("426 Connection closed; transfer aborted.\r\n");    //!< FTP error string 426
static const CcString FTP_430("430 Invalid username or password.\r\n");           //!< FTP error string 430
static const CcString FTP_450("450 Requested file action not taken.\r\n");        //!< FTP error string 450
//! FTP error string 451
static const CcString FTP_451("451 Requested action aborted.Local error in processing.\r\n");
//! FTP error string 452
static const CcString FTP_452("452 Requested action not taken.Insufficient storage space in system.File unavailable(e.g., file busy).\r\n");

//! FTP error string 500
static const CcString FTP_500("500 Syntax error");
//! FTP error string 501
static const CcString FTP_501("501 Connection Error.\r\n");
//! FTP error string 550
static const CcString FTP_550("550 Requested action not taken. File unavailable (e.g., file not found, no access).\r\n");
//! FTP error string 551
static const CcString FTP_551("551 Error reading file from disk \r\n");
