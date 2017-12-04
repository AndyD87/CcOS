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
 * @brief     Implemtation of class CSharedMemoryTest
 */
#include "CSharedMemoryTest.h"
#include "CcConsole.h"
#include "CcString.h"

const CcString c_sName("CSharedMemoryTest");
const CcString c_sTestStringTrue("This String is true");
const CcString c_sTestStringFalse("This String is false");

CSharedMemoryTest::CSharedMemoryTest( void ) : 
  m_oSharedMemory(c_sName)
{
}

CSharedMemoryTest::~CSharedMemoryTest( void )
{
}

bool CSharedMemoryTest::test()
{
  bool bSuccess = true;
  bSuccess &= testCreate();
  bSuccess &= testClaim();
  bSuccess &= testClose();
  if(!bSuccess)
  {
    CcConsole::writeLine("CSharedMemoryTest failed");
  }
  return bSuccess;
}

bool CSharedMemoryTest::testCreate()
{
  bool bRet = false;
  if (m_oSharedMemory.open(EOpenFlags::ReadWrite))
  {
    bRet = m_oSharedMemory.write(c_sTestStringTrue.getCharString(), c_sTestStringTrue.length()) != SIZE_MAX;
  }
  return bRet;
}

bool CSharedMemoryTest::testClaim()
{
  bool bRet = false;
  CcSharedMemory oMemory(c_sName);
  if (oMemory.exists()                &&
      oMemory.claim(EOpenFlags::Read) )
  {
    char oTestData[100];
    size_t uiReadData = oMemory.read(oTestData, 100);
    if (uiReadData == 100)
    {
      CcString sTestString(oTestData);
      if (sTestString == c_sTestStringTrue)
      {
        bRet = true;
      }
    }
    oMemory.close();
    bRet = true;
  }
  return bRet;
}

bool CSharedMemoryTest::testClose()
{
  bool bRet = false;
  if (m_oSharedMemory.close())
  {
    bRet = true;
  }
  return bRet;
}