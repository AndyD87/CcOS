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
 * @brief     Implemtation of class CBufferTest
 */
#include "CBufferTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcStatic.h"
#include "CcBufferList.h"
#include "CcConsole.h"

CBufferTest::CBufferTest() :
  CcTest("CBufferTest")
{
  appendTestMethod("Test basic allocation tests", &CBufferTest::testBasic);
  appendTestMethod("Test collapsing", &CBufferTest::testCollapsing);
  appendTestMethod("Test collapsing to array", &CBufferTest::testBufferCollapsing);
  appendTestMethod("Test transfering buffer", &CBufferTest::testBufferTransfering);
}

CBufferTest::~CBufferTest()
{
}

bool CBufferTest::testBasic()
{
  bool bRet = false;
  CcBufferList oBuffer;
  oBuffer.append("Test", sizeof("Test"));
  if (sizeof("Test") == oBuffer.size())
  {
    // ovewrite \0
    oBuffer.setPosition(sizeof("Test")-1);
    oBuffer.write(" in", sizeof(" in"));
    if (sizeof("Test in") == oBuffer.size())
    {
      oBuffer.setPosition(sizeof("Test in") - 3);
      oBuffer.write("in progress", sizeof("in progress"));
      if (sizeof("Test in progress") == oBuffer.size())
      {
        char cTestArray[sizeof("Test in progress") + 1];
        size_t uiReadSize = oBuffer.read(cTestArray, sizeof(cTestArray));
        if (uiReadSize == 0)
        {
          oBuffer.setPosition(0);
          uiReadSize = oBuffer.read(cTestArray, sizeof(cTestArray));
          if (uiReadSize == sizeof("Test in progress"))
          {
            if (CcStringUtil::strcmp("Test in progress", cTestArray) == 0)
            {
              oBuffer.setPosition(3);
              CcStatic_memsetZeroObject(cTestArray);
              uiReadSize = oBuffer.read(cTestArray, 6);
              if (uiReadSize == 6)
              {
                if (CcStringUtil::strcmp("t in p", cTestArray) == 0)
                {
                  CcStatic::memset(cTestArray,'f', sizeof(cTestArray));
                  oBuffer.setPosition(3);
                  uiReadSize = oBuffer.write(cTestArray, 6);
                  if (uiReadSize == 6)
                  {
                    oBuffer.setPosition(0);
                    uiReadSize = oBuffer.read(cTestArray, sizeof(cTestArray));
                    if (uiReadSize == oBuffer.size())
                    {
                      bRet = CcStringUtil::strcmp("Test in progress", cTestArray);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return bRet;
}

bool CBufferTest::testCollapsing()
{
  bool bRet = false;
  CcBufferList oBuffer;
  oBuffer.append("Test", sizeof("Test"));
  oBuffer.append("Test", sizeof("Test"));
  oBuffer.append("Test", sizeof("Test"));
  oBuffer.append("Test", sizeof("Test"));
  if (oBuffer.getChunkCount() == 4)
  {
    oBuffer.collapse();
    if (oBuffer.getChunkCount() == 1)
    {
      char TestArray[25];
      size_t uiReadData = oBuffer.read(TestArray, sizeof(TestArray));
      if (uiReadData == 20)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CBufferTest::testBufferCollapsing()
{
  bool bRet = false;
  CcBufferList oBuffer;
  oBuffer.append("Test", sizeof("Test") - 1);
  oBuffer.append("Test", sizeof("Test") - 1);
  oBuffer.append("Test", sizeof("Test") - 1);
  oBuffer.append("Test", sizeof("Test"));
  if (oBuffer.getChunkCount() == 4)
  {
    char* pBufferArray = static_cast<char*>(oBuffer.getBuffer());
    if (CcStringUtil::strcmp(pBufferArray, "TestTestTestTest") == 0)
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CBufferTest::testBufferTransfering()
{
  bool bRet = false;
  char* pBuffer = new char[128];
  CcStatic::memset(pBuffer, 0, 128);
  CcBufferList oBuffer;
  oBuffer.append(pBuffer, 128);
  if (oBuffer.size() == 128)
  {
    char* pBuffer2 = new char[128];
    CcStatic::memset(pBuffer2, 0xff, 128);
    oBuffer.append(pBuffer2, 128);
    if (oBuffer.size() == 256)
    {
      char* pBufferTemp = static_cast<char*>(oBuffer.getBuffer());
      if (pBuffer != pBufferTemp)
      {
        bRet = true;
        for (int i = 0; i < 128; i++)
        {
          char uTest1 = pBufferTemp[i];
          char uTest2 = pBufferTemp[i + 128];
          if (uTest1 != 0)
          {
            bRet = false;
          }
          else if (uTest2 != -1)
          {
            bRet = false;
          }
        }
      }
    }
  }
  return bRet;
}
