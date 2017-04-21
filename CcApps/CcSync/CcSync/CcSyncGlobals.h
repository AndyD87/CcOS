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
 * @page      CcSync
 * @subpage   CcSyncGlobals
 * 
 * @page      CcSyncGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Definitions for CcSync
 */
#ifndef CcSyncGlobals_H_
#define CcSyncGlobals_H_

#include "CcBase.h"
#include "CcString.h"

enum class ESyncRights
{
  None      = 0,
  ReadOnly  = 1,
  User      = 2,
  Account   = 3,
  Admin
};

namespace CcSyncGlobals
{
  static const CcString sTest           ("Test");
  static const CcString ConfigDirName   ("CcSync");
  static const uint64 TransferSize      = 1024000;
  static const uint64 MaxRequestSize    = TransferSize;
  static const uint64 MaxResponseSize   = MaxRequestSize;
  static const CcString DefaultCertFile ("SslCertificate.pem");
  static const CcString DefaultKeyFile  ("SslPrivateKey.pem");
  static const CcString SqliteExtension (".sqlite");
  static const CcString XmlExtension    (".xml");
  static const uint64 SessionLength      = 1*60*60; // 1 hour

  static const CcString IndexName("Id");
  static const CcString NameName("Name");
  static const CcString SizeName("Size");

  namespace FileInfo
  {
    static const CcString& Id     = IndexName;
    static const CcString DirId   ("DirId");
    static const CcString Name    = NameName;
    static const CcString Modified("Modified");
    static const CcString CRC     ("CRC");
    static const CcString MD5     ("MD5");
    static const CcString Size    = SizeName;
    static const CcString UserId  ("UserId");
    static const CcString GroupId ("GroupId");
    static const CcString Changed ("Changed");
    static const CcString IsFile  ("IsFile");
  }

  namespace Database
  {
    static const uint64 RootDirId        = 1;
    static const CcString CreateTable    ("CREATE TABLE `");
    static const CcString CreateTableEnd ("`");
    static const CcString Insert         ("INSERT INTO `");
    static const CcString& InsertEnd     = CreateTableEnd;
    static const CcString Update         ("Update `");
    static const CcString& UpdateEnd     = CreateTableEnd;

    static const CcString DirectoryListAppend ("_DirList");
    static const CcString FileListAppend   ("_FileList");
    static const CcString QueueAppend      ("_Queue");
    static const CcString HistoryAppend    ("_History");

    namespace FileList
    {
      static const CcString& Id       = IndexName;
      static const CcString& DirId    = FileInfo::DirId;
      static const CcString& Name     = NameName;
      static const CcString& Size     = SizeName;
      static const CcString& UserId   = FileInfo::UserId;
      static const CcString& GroupId  = FileInfo::GroupId;
      static const CcString Modified  ("Modified");
      static const CcString MD5       ("MD5");
      static const CcString CRC       ("CRC");
      static const CcString Changed   ("Changed");
    }

    namespace Queue
    {
      static const CcString& Id      = IndexName;
      static const CcString QueueId  ("QueueId");
      static const CcString Type     ("Type");
      static const CcString FileId   ("FileId");
      static const CcString& DirId    = FileInfo::DirId;
      static const CcString& UserId   = FileList::UserId;
      static const CcString& GroupId  = FileList::GroupId;
      static const CcString Name     ("Name");
      static const CcString Attempts ("Attempts");
    }

    namespace DirectoryList
    {
      static const CcString& Id         = IndexName;
      static const CcString& DirId      = FileInfo::DirId;
      static const CcString& Name       = NameName;
      static const CcString& UserId     = FileList::UserId;
      static const CcString& GroupId    = FileList::GroupId;
      static const CcString Modified    ("Modified");
      static const CcString ChangedMd5  ("ChangedMd5");
    }
    
    namespace History
    {
      static const CcString& Id       = IndexName;
      static const CcString& DirId    = FileList::DirId;
      static const CcString  TargetId ("TargetId");
      static const CcString& Type     = Queue::Type;
      static const CcString& Name     = FileList::Name;
      static const CcString& Size     = FileList::Size;
      static const CcString& UserId   = FileList::UserId;
      static const CcString& GroupId  = FileList::GroupId;
      static const CcString& Modified = FileList::Modified;
      static const CcString& MD5      = FileList::MD5;
      static const CcString& CRC      = FileList::CRC;
      static const CcString  Stamp    ("Stamp");
    }

    namespace User
    {
      static const CcString& Id      = IndexName;
      static const CcString Account    ("Account");
      static const CcString Username   ("Username");
      static const CcString Session    ("Session");
      static const CcString SessionEnd ("SessionEnd");
    }
  }

  namespace Server
  {
    static const CcString ConfigFileName ("Server.xml");
    static const CcString DatabaseFileName ("Server.sqlite");
    static const CcString RootAccountName("Root");
    namespace Database
    {
      static const CcString TableNameUser ("User");
    }
    namespace ConfigTags
    {
      static const CcString Root  ("CcSyncServer");
      static const CcString Port ("Port");
      static const CcString Ssl ("Ssl");
      static const CcString SslRequired ("SslRequired");
      static const CcString SslCert ("SslCert");
      static const CcString SslKey  ("SslKey");
      static const CcString Account ("Account");
      static const CcString AccountName ("Name");
      static const CcString AccountPassword ("Password");
      static const CcString AccountPasswordType ("Type");
      static const CcString Locations ("Locations");
      static const CcString Location ("Location");
      static const CcString LocationPath ("Path");
      static const CcString LocationType ("Type");
    }
  }

  namespace Client
  {
    static const CcString ConfigFileName   ("Client.xml");
    static const CcString DatabaseFileName ("Client.sqlite");
    namespace ConfigTags
    {
      static const CcString Root ("CcSyncClient");
      static const CcString Name ("Name");
      static const CcString User ("User");
      static const CcString UserName ("Name");
      static const CcString UserPassword ("Password");

      static const CcString Admin("Admin");
      static const CcString Account ("Account");

      static const CcString Directory("Directory");
      static const CcString& DirectoryName    = Name;
      static const CcString DirectoryLocation ("Location");

      static const CcString Command ("Command");
      static const CcString CommandExecutable ("Executable");
      static const CcString CommandParameters ("Parameters");
      static const CcString CommandWorkingDirectory ("WorkingDirectory");
      static const CcString CommandTimeMask ("TimeMask");

      static const CcString Server ("Server");
      static const CcString ServerHost ("Host");
      static const CcString ServerPort ("Port");

      static const CcString Database ("Database");
    }
  }

  namespace Commands
  {
    namespace Crc
    {
      static const CcString Crc ("Crc");
    }

    namespace ServerGetInfo
    {
      
    }

    namespace AccountCreate
    {
      
    }

    namespace AccountLogin
    {
      static const CcString& Account    = Database::User::Account;
      static const CcString& Username   = Database::User::Username;
      static const CcString Password    ("Password");
      static const CcString& Session    = Database::User::Session;
    }

    namespace AccountGetData
    {
      static const CcString& Account     = Database::User::Account;
      static const CcString& Name        = Client::ConfigTags::Name;
      static const CcString& Password    = Client::ConfigTags::UserPassword;
      static const CcString& Server      = Client::ConfigTags::Server;
      static const CcString& ServerHost  = Client::ConfigTags::ServerHost;
      static const CcString& ServerPort  = Client::ConfigTags::ServerPort;
      static const CcString& Database    = Client::ConfigTags::UserPassword;

      static const CcString& Directories = Client::ConfigTags::Directory;
      static const CcString& DirName     = Client::ConfigTags::DirectoryName;
      static const CcString& DirLocation = Client::ConfigTags::DirectoryLocation;
    }
    namespace AccountSetData
    {

    }
    namespace AccountGetDirectoryList
    {
    }
    namespace UserGetCommandList
    {

    }
    namespace DirectoryGetFileList
    {
      static const CcString DirectoryName    ("DirectoryName");
      static const CcString& Id           = FileInfo::Id;
      static const CcString FilesNode     ("Files");
      static const CcString DirsNode      ("Directories");
    }
    namespace DirectoryGetFileInfo
    {
      static const CcString& Id           = FileInfo::Id;
    }
    namespace DirectoryCreateDirectory
    {
      static const CcString& DirectoryName = DirectoryGetFileList::DirectoryName;
    }
    namespace DirectoryRemoveDirectory
    {
      static const CcString& DirectoryName = DirectoryGetFileList::DirectoryName;
    }
    namespace DirectoryUploadFile
    {
      static const CcString& DirectoryName = DirectoryGetFileList::DirectoryName;
    }
    namespace DirectoryGetDirectoryInfo
    {
      static const CcString& DirectoryName = DirectoryGetFileList::DirectoryName;
      static const CcString& Id         = FileInfo::Id;
    }
    namespace DirectoryDownloadFile
    {
      static const CcString& DirectoryName = DirectoryGetFileList::DirectoryName;
      static const CcString& Id         = FileInfo::Id;

    }
    static const CcString Command    ("Command");
    static const CcString Session    ("Session");
    static const CcString Result     ("Result");
    static const CcString ErrorCode  ("Error");
    static const CcString ErrorMsg   ("ErrorMsg");
  }
}

#endif /* CcSyncGlobals_H_ */
