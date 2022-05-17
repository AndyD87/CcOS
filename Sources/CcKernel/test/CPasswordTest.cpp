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
 * @brief     Implemtation of class CPasswordTest
 */
#include "CPasswordTest.h"
#include "CcPassword.h"
#include "CcConsole.h"

CPasswordTest::CPasswordTest() :
  CcTest("CPasswordTest")
{
  appendTestMethod("Test constructor", &CPasswordTest::testConstructor);
  appendTestMethod("Test generate hash", &CPasswordTest::testGenHash);
}

CPasswordTest::~CPasswordTest()
{
}

bool CPasswordTest::testConstructor()
{
  bool bRet = true;
  CcPassword oPassword;
  if(oPassword.getString() ==  "")
  {
    oPassword.setPassword("hello world!", EHashType::Sha256);
    CcPassword oPassword2("hello world!", EHashType::Unknown);
    if(oPassword != oPassword2)
    {
      oPassword2.setType(EHashType::Sha256);
      if(oPassword == oPassword2)
      {
        CcPassword oCopyPassword(oPassword2);
        CcPassword oMovePassword(CCMOVE(oPassword2));
        if(oPassword == oCopyPassword &&
           oPassword == oMovePassword)
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
    bRet = false;
  }
  return bRet;
}

bool CPasswordTest::testGenHash()
{
  bool bRet = true;
  // Test Md5
  if(bRet)
  {
    CcPassword oPasswordGenerator;
    if(oPasswordGenerator.generatePassword("hello world!", EHashType::Md5))
    {
      if(oPasswordGenerator.getString() != "fc3ff98e8c6a0d3087d515c0473f8677")
      {
        bRet = false;
      }
    }
    else
    {
      bRet = false;
    }
  }
  // Test Sha256
  if(bRet)
  {
    CcPassword oPasswordGenerator;
    if(oPasswordGenerator.generatePassword("hello world!", EHashType::Sha256))
    {
      if(oPasswordGenerator.getString() != "7509e5bda0c762d2bac7f90d758b5b2263fa01ccbc542ab5e3df163be08e6ca9")
      {
        bRet = false;
      }
    }
    else
    {
      bRet = false;
    }
  }
  return bRet;
}
