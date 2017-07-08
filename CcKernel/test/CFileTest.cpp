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
#include <cstring>

const CcString CFileTest::c_sTestFileName("TestFile.bin");

CFileTest::CFileTest( void )
{
}

CFileTest::~CFileTest( void )
{
}

bool CFileTest::createTestFile()
{
  bool bSuccess = true;
  CcFile oFile(c_sTestFileName);
  if (CcFile::exists(c_sTestFileName))
    bSuccess = CcFile::remove(c_sTestFileName);
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
  if (CcFile::exists(c_sTestFileName))
  {
    return CcFile::remove(c_sTestFileName);
  }
  return false;
}

CcFile CFileTest::getTestFile(EOpenFlags eFlags)
{
  CcFile oFile(c_sTestFileName);
  oFile.open(eFlags);
  return oFile;
}

bool CFileTest::test()
{
  bool bSuccess = true;
  bSuccess &= test1();
  bSuccess &= crcTest();
  return bSuccess;
}

bool CFileTest::test1()
{
  bool bRet = true;
  CcString sPath = CcKernel::getDataDir();
  CcString sFilePath(sPath);
  sFilePath.appendPath(c_sTestFileName);
  if (CcFile::exists(sFilePath) && !CcFile::remove(sFilePath))
  {
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
        if (oInfoLst.find(c_sTestFileName) &&
            oInfoLst.getFormatedList(SHOW_HIDDEN).find(c_sTestFileName))
        {
          if (oFile.close())
          {
            if (  CcFile::remove(sFilePath) &&
                  !oDirectory.getFileList().find(c_sTestFileName))
            {
              bRet = true;
            }
            else
            {
              bRet = false;
            }
          }
          else
          {
            bRet = false;
          }
        }
        else
        {
          bRet = false;
        }

      }
      else
      {
        bRet &= false;
      }
    }
    else
    {
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
    CcByteArray oTestArray("Hello World!\nThis is a Sample TextFile", strlen("Hello World!\nThis is a Sample TextFile"));
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
      }
    }
    removeTestFile();
  }
  return bSuccess;
}
