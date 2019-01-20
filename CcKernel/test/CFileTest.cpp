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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CFileTest
 */
#include "CFileTest.h"
#include "CcFile.h"
#include "CcDirectory.h"
#include "CcKernel.h"
#include "Hash/CcCrc32.h"
#include "CcFileInfoList.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcConsole.h"
#include "CcUser.h"
#include "CcUserList.h"
#include "CcGroupList.h"

const CcString CFileTest::c_sTestFileName("TestFile.bin");
CcString CFileTest::s_sTestFilePath("");
CcString CFileTest::s_sTestDirPath("");

CFileTest::CFileTest(void ) :
  CcTest("CcTest")
{
  if (s_sTestFilePath.length() == 0)
  {
    s_sTestFilePath = CcKernel::getTempDir();
    s_sTestFilePath.appendPath(c_sTestFileName);
    s_sTestDirPath = s_sTestFilePath + "Dir";
  }

  appendTestMethod("Basic tests", &CFileTest::testBasics);
  appendTestMethod("Crc test", &CFileTest::crcTest);
  appendTestMethod("Copy file test", &CFileTest::testCopyFile);
  appendTestMethod("Move file test", &CFileTest::testMoveFile);
  appendTestMethod("Append file test", &CFileTest::testAppendFile);
  appendTestMethod("Set user id test", &CFileTest::testUserId);
  appendTestMethod("Set group id test", &CFileTest::testGroupId);
  appendTestMethod("Attributes test", &CFileTest::testAttributes);
  appendTestMethod("Directory create test", &CFileTest::testDirectoryCreate);
  appendTestMethod("Directory create bug test", &CFileTest::testDirectoryCreateBug);
  
#ifdef WINDOWS
  appendTestMethod("Special test for Windows", &CFileTest::testWindows);
#endif
}

CFileTest::~CFileTest(void )
{
}

bool CFileTest::createTestFile()
{
  bool bSuccess = true;
  CcFile oFile(s_sTestFilePath);
  if (CcFile::exists(s_sTestFilePath))
    bSuccess = CcFile::remove(s_sTestFilePath);
  if (bSuccess == true)
  {
    if (oFile.open(EOpenFlags::Write))
      bSuccess = oFile.close();
    else
      bSuccess = false;
  }
  return bSuccess;
}

bool CFileTest::removeTestFile()
{
  if (CcFile::exists(s_sTestFilePath))
  {
    return CcFile::remove(s_sTestFilePath);
  }
  return false;
}

CcFile CFileTest::getTestFile(EOpenFlags eFlags)
{
  CcFile oFile(s_sTestFilePath);
  oFile.open(eFlags);
  return oFile;
}

bool CFileTest::testBasics()
{
  bool bRet = true;
  CcString sPath = CcKernel::getTempDir();
  CcString sFilePath(sPath);
  sFilePath.appendPath(c_sTestFileName);
  if (CcFile::exists(sFilePath) && !CcFile::remove(sFilePath))
  {
    CcConsole::writeLine("Unable to remove TestFile");
    bRet &= false;
  }
  if (bRet)
  {
    CcFile oFile(sFilePath);
    CcFile oDirectory(sPath);
    if (oFile.open(EOpenFlags::Write) &&
        oDirectory.isDir() )
    {
      if (  oFile.exists() )
      {
        CcFileInfoList oInfoLst = oDirectory.getFileList();
        if (oInfoLst.containsFile(c_sTestFileName) &&
            oInfoLst.getFormatedList(EFileInfoListFormats::NamesOnly).contains(c_sTestFileName))
        {
          if (oFile.close())
          {
            if (  CcFile::remove(sFilePath) &&
                  !oDirectory.getFileList().containsFile(c_sTestFileName))
            {
              bRet = true;
            }
            else
            {
              CcConsole::writeLine("Unable to delete used TestFile");
              bRet = false;
            }
          }
          else
          {
            CcConsole::writeLine("Closing TestFile failed");
            bRet = false;
          }
        }
        else
        {
          CcConsole::writeLine("Searching for previously created TestFile failed in CcDirectory");
          CcConsole::writeLine("\tDir:  " + sPath);
          CcConsole::writeLine("\tFile: " + c_sTestFileName);
          CcConsole::writeLine("\tFiles in dir: ");
          for(CcFileInfo& rFileName : oInfoLst)
            CcConsole::writeLine("\t\t" + rFileName.getName());
          CcConsole::writeLine("\tFormated Files in dir: ");
          for (CcString& sFileName : oInfoLst.getFormatedList(EFileInfoListFormats::ExtendedLs))
            CcConsole::writeLine("\t\t" + sFileName);
          bRet = false;
        }
      }
      else
      {
        CcConsole::writeLine("Successfully created File not existing?");
        bRet &= false;
      }
    }
    else
    {
      CcConsole::writeLine("Opening Testfile failed, or directory not found.");
      bRet &= false;
    }
  }
  return bRet;
}

bool CFileTest::crcTest()
{
  bool bSuccess = false;
  if (createTestFile())
  {
    CcByteArray oTestArray("Hello World!\nThis is a Sample TextFile", CcStringUtil::strlen("Hello World!\nThis is a Sample TextFile"));
    CcFile oFile = getTestFile(EOpenFlags::ReadWrite);
    oFile.writeArray(oTestArray);
    if (oFile.setFilePointer(0))
    {
      CcByteArray oFileContent = oFile.readAll();
      if (oTestArray == oFileContent)
      {
        CcCrc32 uiCrcArray = oFileContent.getCrc32();
        CcCrc32 uiCrcFile = oFile.getCrc32();
        if ( uiCrcArray == uiCrcFile &&
             uiCrcArray == 0x7893973c)
        {
          bSuccess = true;
        }
        else
        {
          CcConsole::writeLine("Crc of TestFile failed");
        }
      }
      else
      {
        CcConsole::writeLine("Written not mathching in ReadWrite opened file");
      }
    }
    else
    {
      CcConsole::writeLine("setFilePointer failed");
    }
    oFile.close();
    removeTestFile();
  }
  else
  {
    CcConsole::writeLine("Creating TestFile failed");
  }
  return bSuccess;
}

bool CFileTest::testCopyFile()
{
  bool bSuccess = false;
  if (createTestFile())
  {
    CcFile oFile(s_sTestFilePath);
    if (oFile.open(EOpenFlags::Overwrite))
    {
      oFile.writeArray("Test data for CopyFileTest");
      oFile.close();
      CcString sCopyFilePath = s_sTestFilePath;
      sCopyFilePath.append(".copy");
      if (CcFile::copy(s_sTestFilePath, sCopyFilePath) &&
          CcFile::exists(sCopyFilePath))
      {
        if (CcFile::compare(sCopyFilePath, s_sTestFilePath, true) &&
          CcFile::compare(sCopyFilePath, s_sTestFilePath, true))
        {
          bSuccess = true;
        }
        CcFile::remove(sCopyFilePath);
      }
    }
    removeTestFile();
  }
  return bSuccess;
}

bool CFileTest::testMoveFile()
{
  bool bSuccess = false;
  if (createTestFile())
  {
    CcFile oFile(s_sTestFilePath);
    if (oFile.open(EOpenFlags::Overwrite))
    {
      oFile.writeArray("Test data for WriteFileTest");
      oFile.close();
      CcString sCopyFilePath = s_sTestFilePath;
      sCopyFilePath.append(".copy");
      if (CcFile::move(s_sTestFilePath, sCopyFilePath)  &&
          CcFile::exists(sCopyFilePath)                 &&
          !CcFile::exists(s_sTestFilePath)              )
      {
        bSuccess = true;
        CcFile::remove(sCopyFilePath);
      }
    }
  }
  return bSuccess;
}

bool CFileTest::testAppendFile()
{
  const CcString sFileDataBegin("Test data for testAppendFile");
  const CcString sFileDataAppend("Test append data for testAppendFile");
  CcString sFullString = sFileDataBegin + sFileDataAppend;
  CcString sResultString;
  bool bSuccess = false;
  if (createTestFile())
  {
    bSuccess = true;
    CcFile oFile(s_sTestFilePath);
    if (oFile.open(EOpenFlags::Overwrite))
    {
      oFile.writeString(sFileDataBegin);
      if (!oFile.close())
      {
        bSuccess = false;
      }
      if (bSuccess)
      {
        bSuccess = oFile.open(EOpenFlags::Append);
      }
      if (bSuccess)
      {
        bSuccess = oFile.writeString(sFileDataAppend);
        oFile.close();
      }
      if (bSuccess)
      {
        bSuccess = oFile.open(EOpenFlags::Read);
      }
      if (bSuccess)
      {
        sResultString = oFile.readAll();
        if (sResultString != sFullString)
        {
          bSuccess = false;
        }
        oFile.close();
      }
      CcString sCopyFilePath = s_sTestFilePath;
    }
    removeTestFile();
  }
  return bSuccess;
}

bool CFileTest::testUserId()
{
  bool bSuccess = false;
#ifndef WIN32
  CcUserList oUserlist = CcKernel::getUserList();
  if (createTestFile())
  {
    if(oUserlist.currentUser()->getId() == 0)
    {
      CcFile oFile(s_sTestFilePath);
      if(oFile.open(EOpenFlags::Write | EOpenFlags::Attributes))
      {
        if(oFile.write("test", 4) &&
           oFile.setUserId(0))
        {
          CcFileInfo oFileInfo = oFile.getInfo();
          if(oFileInfo.getUserId() == 0)
          {
            bSuccess = true;
          }
          if(bSuccess && !oFile.setUserId(1000))
          {
            bSuccess = false;
          }
          oFileInfo = oFile.getInfo();
          if(bSuccess && oFileInfo.getUserId() != 1000)
          {
            bSuccess = false;
          }
        }
        oFile.close();
      }
    }
    else
    {
      bSuccess = true;
      CCDEBUG("No root user can change with chown, no reason to fail");
    }
    removeTestFile();
  }
#else
  // Windows does not supper UserId
  bSuccess = true;
#endif
  return bSuccess;
}

bool CFileTest::testGroupId()
{
  bool bSuccess = false;
#ifndef WIN32
  CcGroupList oSystemGroups = CcKernel::getGroupList();
  CcUserList oUserlist = CcKernel::getUserList();
  if (createTestFile())
  {
    if(oUserlist.currentUser()->getId() == 0)
    {
      CcFile oFile(s_sTestFilePath);
      if(oFile.open(EOpenFlags::Write | EOpenFlags::Attributes))
      {
        if(oFile.write("test", 4) &&
           oFile.setGroupId(0))
        {
          CcFileInfo oFileInfo = oFile.getInfo();
          if(oFileInfo.getGroupId() == 0)
          {
            bSuccess = true;
          }
          if(bSuccess && !oFile.setGroupId(1000))
          {
            bSuccess = false;
          }
          oFileInfo = oFile.getInfo();
          if(bSuccess && oFileInfo.getGroupId() != 1000)
          {
            bSuccess = false;
          }
        }
        oFile.close();
      }
    }
    else
    {
      bSuccess = true;
      CCDEBUG("No root user can change with chown, no reason to fail");
    }
    removeTestFile();
  }
#else
  // Windows does not supper UserId
  bSuccess = true;
#endif
  return bSuccess;
}

bool CFileTest::testAttributes()
{
  bool bSuccess = false;
#ifndef WIN32
  if (createTestFile())
  {
    CcFile oFile(s_sTestFilePath);
    if(oFile.open(EOpenFlags::Write | EOpenFlags::Attributes))
    {
      if(oFile.write("test", 4) &&
         oFile.setAttributes(EFileAttributes::None))
      {
        CcFileInfo oFileInfo = oFile.getInfo();
        if(oFileInfo.getAttributes() == EFileAttributes::None &&
           oFileInfo.getAttributesString() == "----------")
        {
          bSuccess = true;
        }
        if(bSuccess && !oFile.setAttributes(EFileAttributes::UserRead | EFileAttributes::UserWrite))
        {
          bSuccess = false;
        }
        oFileInfo = oFile.getInfo();
        if(bSuccess && oFileInfo.getAttributesString() == "-rw-------")
        {
          bSuccess = false;
        }
        if(bSuccess && !oFile.setAttributes(EFileAttributes::GroupRead | EFileAttributes::GroupWrite))
        {
          bSuccess = false;
        }
        oFileInfo = oFile.getInfo();
        if(bSuccess && oFileInfo.getAttributesString() == "----rw----")
        {
          bSuccess = false;
        }
        if(bSuccess && !oFile.setAttributes(EFileAttributes::GroupRead | EFileAttributes::GroupWrite))
        {
          bSuccess = false;
        }
        oFileInfo = oFile.getInfo();
        if(bSuccess && oFileInfo.getAttributesString() == "-------rw-")
        {
          bSuccess = false;
        }
      }
      oFile.close();
    }
    removeTestFile();
  }
#else
  // Windows does not supper UserId
  bSuccess = true;
#endif
  return bSuccess;
}

bool CFileTest::testDirectoryCreate()
{
  bool bSuccess = true;
  if (CcDirectory::exists(s_sTestDirPath))
  {
    bSuccess = CcDirectory::remove(s_sTestDirPath, true);
  }
  CcString sRecursivePath = s_sTestDirPath;
  sRecursivePath.appendPath("/Dir1/Dir2/Dir3");
  if (bSuccess &&
      !CcDirectory::exists(sRecursivePath))
  {
    bSuccess = false;
    if (CcDirectory::create(sRecursivePath, true))
    {
      if (CcDirectory::exists(sRecursivePath))
      {
        if (! CcDirectory::remove(s_sTestDirPath) && // must fail, because not empty
              CcDirectory::remove(s_sTestDirPath, true))   // musst succeed now
        {
          bSuccess = !CcDirectory::exists(s_sTestDirPath);
        }
      }
    }
  }
  return bSuccess;
}

bool CFileTest::testDirectoryCreateBug()
{
  bool bSuccess = true;
  bSuccess = CcDirectory::create("TestDir", true);
  return bSuccess;
}

#ifdef WINDOWS
bool CFileTest::testWindows()
{
  bool bRet = true;
  return bRet;
}
#endif