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
 * @brief     Implemtation of class CIniFileTest
 */
#include "CIniFileTest.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcKernel.h"
#include "CcStringStream.h"
#include "Documents/CcIniFile.h"

const char pExampleIni[] =
    "#  Global Comment 0\r\n"
    " # Global Comment 1\r\n"
    " ; Global Comment 2\r\n"
    "[Unit]\r\n"
    "Description=TestService\r\n"
    "\r\n"
    "# Comment 1\r\n"
    "[Service]\r\n"
    "Type=simple\r\n"
    "ExecStart=/opt/bin/TestService\r\n"
    "WorkingDirectory=/opt/bin\r\n"
    "\r\n"
    "Restart=on-failure\r\n"
    "RestartSec=10\r\n"
    "KillMode=process\r\n"
    "\r\n"
    " [Test] # Section comment\r\n"
    "Hallo1=\"'Test' \\\"Value\\\"\" # Comment behind\r\n"
    "Hallo2='\\'Test\\' \"Value\"' ; Comment behind\r\n"
    "Hallo3=Test \"Value\" # Comment behind\r\n"
    "Hallo4= Test \"Value\"\r\n"
    " [Install] \r\n"
    "WantedBy=multi-user.target\r\n";

CIniFileTest::CIniFileTest() :
  CcTest("CIniFileTest")
{
  appendTestMethod("Test example values and keys", &CIniFileTest::testFileExampleKeyValue);
  appendTestMethod("Test example comments", &CIniFileTest::testFileExampleComments);
  appendTestMethod("Test parse and rebuild example", &CIniFileTest::testFileExampleRegenerate);
}

CIniFileTest::~CIniFileTest()
{
}

bool CIniFileTest::testFileExampleKeyValue()
{
  bool bRet = false;
  CcIniFile oFile;
  CcString oStrig(pExampleIni);
  CcStringStream oSteam(oStrig);
  if (oFile.readStream(oSteam))
  {
    CcIniFile::CSection& oSection = oFile["Test"];
    if (oSection.isValid())
    {
      CcString sValue1 = oSection[0].sValue;
      CcString sValue2 = oSection[1].sValue;
      if (sValue1 == sValue2)
      {
        sValue1 = oSection[2].sValue;
        sValue2 = oSection[3].sValue;
        if (sValue1 == sValue2)
        {
          CcIniFile::CLine& oLine1 = oSection[2];
          CcIniFile::CLine& oLine2 = oSection[3];
          if (oLine1.sPostValue == oLine2.sPreValue &&
              oLine2.sPostValue == oLine1.sPreValue &&
              oLine2.sPostValue != oLine2.sPreValue &&
              oLine1.sPostValue != oLine1.sPreValue)
          {
            bRet = true;
          }
        }
      }
    }
  }
  return bRet;
}

bool CIniFileTest::testFileExampleComments()
{
  bool bRet = false;
  CcIniFile oFile;
  CcString oStrig(pExampleIni);
  CcStringStream oSteam(oStrig);
  if (oFile.readStream(oSteam))
  {
    CcIniFile::CSection& oSection = oFile["Test"];
    if (oSection.isValid() && oFile["TestFailed"].isValid() != true)
    {
      CcString sValue1 = oSection[0].sComment;
      CcString sValue2 = oSection[1].sComment;
      if (sValue1 == oSection[1].sComment &&
          oSection[0].sComment == sValue2)
      {
        if (oSection[0].cCommentSign != oSection[1].cCommentSign &&
            oSection[0].cCommentSign == oSection[2].cCommentSign)
        {
          if (oSection.getSectionLine().sComment == " Section comment" &&
              oSection.getSectionLine().cCommentSign == '#')
          {
            bRet = true;
          }
        }
      }
    }
  }
  return bRet;
}

bool CIniFileTest::testFileExampleRegenerate()
{
  bool bRet = false;
  CcIniFile oFile;
  CcString oStrig(pExampleIni);
  CcStringStream oSteam(oStrig);
  if (oFile.readStream(oSteam))
  {
    CcString oStrigRegenerate;
    CcStringStream oSteamRegenerate(oStrigRegenerate);
    if (oFile.writeStream(oSteamRegenerate, "\r\n"))
    {
      if (oStrigRegenerate == pExampleIni)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}
