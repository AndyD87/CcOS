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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CFileTest
 */
#include "CFileTest.h"
#include "CcFile.h"
#include "CcKernel.h"
#include "Hash/CcCrc32.h"
#include "CcFileInfoList.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcConsole.h"

const CcString CFileTest::c_sTestFileName("TestFile.bin");
CcString CFileTest::s_sTestFilePath("");

CFileTest::CFileTest( void )
{
  if (s_sTestFilePath.length() == 0)
  {
    s_sTestFilePath = CcKernel::getTempDir();
    s_sTestFilePath.appendPath(c_sTestFileName);
  }
}

CFileTest::~CFileTest( void )
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

bool CFileTest::test()
{
  bool bSuccess = true;
  bSuccess &= test1();
  if (!bSuccess)
  {
    CcConsole::writeLine("CFileTest test1 failed");
  }
  bSuccess &= crcTest();
  if(!bSuccess)
  {
    CcConsole::writeLine("CFileTest CrcTest failed");
  }
  bSuccess &= testCopyFile();
  if (!bSuccess)
  {
    CcConsole::writeLine("CFileTest testCopyFile failed");
  }
  bSuccess &= testAppendFile();
  if (!bSuccess)
  {
    CcConsole::writeLine("CFileTest testAppendFile failed");
  }
  bSuccess &= testMoveFile();
  if (!bSuccess)
  {
    CcConsole::writeLine("CFileTest testMoveFile failed");
  }
  return bSuccess;
}

bool CFileTest::test1()
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
        if (oInfoLst.contains(c_sTestFileName) &&
            oInfoLst.getFormatedList(EFileInfoListFormats::NamesOnly).contains(c_sTestFileName))
        {
          if (oFile.close())
          {
            if (  CcFile::remove(sFilePath) &&
                  !oDirectory.getFileList().contains(c_sTestFileName))
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
