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
 * @brief     Implemtation of class CNetworkTest
 */
#include "CNetworkTest.h"
#include "CcIp.h"
#include "Network/CcMacAddress.h"
#include "Network/CcNetworkStack.h"
#include "Network/CcUdpProtocol.h"
#include "CcConsole.h"

CNetworkTest::CNetworkTest() :
  CcTest("CNetworkTest")
{
  appendTestMethod("Test Ipv4 conversion", &CNetworkTest::testIpv4);
  appendTestMethod("Test Mac conversion", &CNetworkTest::testMac);
  appendTestMethod("Test Network Stack simulation", &CNetworkTest::testNetworkStack);
  appendTestMethod("Test UdpChecksum generation", &CNetworkTest::testUdpChecksum);
}

CNetworkTest::~CNetworkTest()
{
}

bool CNetworkTest::testIpv4()
{
  bool bRet = false;
  uint8 pIpTest[4] = { 1, 0, 168, 192 };
  CcIp oIpString("192.168.0.1");
  CcIp oIpInteger(192, 168, 0, 1);
  CcIp oIpArray(pIpTest);
  if (oIpString == oIpInteger &&
      oIpInteger == oIpArray)
  {
    bRet = true;
  }
  if (bRet == true)
  {
    bRet = false;
    CcIp oIp1 = CcIp(pIpTest);
    CcIp oIp2("192.168.0.230");
    CcIp oIp3("192.168.1.1");
    if (oIp1 < oIp2 &&
        oIp2 < oIp3)
    {
      if (oIp2 > oIp1 &&
          oIp3 > oIp1)
      {
        bRet = true;
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    CcIp oIp1 = CcIp(pIpTest);
    oIp1++;
    CcIp oIp2("192.168.0.2");
    CcIp oIp3("192.168.1.1");
    if (oIp1 == oIp2)
    {
      oIp1 += 255;
      if (oIp1 == oIp3)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CNetworkTest::testMac()
{
  bool bRet = false;
  uint8 pMacTest[6] = { 0xdc, 0xfe, 1, 0, 168, 192 };
  CcMacAddress oMacString("c0:a8:00:01:fe:dc");
  CcMacAddress oMacInteger(192, 168, 0, 1, 0xfe, 0xdc);
  CcMacAddress oMacArray(pMacTest);
  if (oMacString == oMacInteger &&
      oMacInteger == oMacArray)
  {
    CcMacAddress oMacString2("c0-a8-00-01-fe-dc");
    CcMacAddress oMacStringNo("C0A80001FEDC");
    if (oMacString == oMacString2 &&
        oMacString == oMacStringNo)
    {
      if (oMacString.getString("", true) == "C0A80001FEDC" &&
          oMacString.getString("-") == "c0-a8-00-01-fe-dc" &&
          oMacString.getString() == "c0:a8:00:01:fe:dc")
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CNetworkTest::testNetworkStack()
{
  bool bRet = true;
  CcNetworkStack oNetworkStack;
  return bRet;
}

bool CNetworkTest::testUdpChecksum()
{
  bool bRet = false;
  // Test example from https://www.securitynik.com/2015/08/calculating-udp-checksum-with-taste-of.html
  // Generate a Udp packet with "Hi" as content
  uint16 uiFrameLength = sizeof(CcUdpProtocol::CHeader) + 2;
  uint8* uiData = new uint8[uiFrameLength];
  uiData[uiFrameLength - 2] = 'H';
  uiData[uiFrameLength - 1] = 'i';
  CcUdpProtocol::CHeader* pHeader = CCVOIDPTRCAST(CcUdpProtocol::CHeader*, uiData);
  pHeader->uiChecksum = 0;
  pHeader->uiDestPort = CcStatic::swapInt16(10);
  pHeader->uiSrcPort  = CcStatic::swapInt16(20);
  pHeader->uiLength   = CcStatic::swapInt16(uiFrameLength);
  CcIp oDestIp(192, 168, 0, 30);
  CcIp oSourceIp(192, 168, 0, 31);
  uint16 uiChecksum = CcUdpProtocol::generateChecksum(pHeader, oDestIp, oSourceIp);
  if (uiChecksum == 0x35C5)
  {
    bRet = true;
  }
  return bRet;
}