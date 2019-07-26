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
#include "Network/CcTcpProtocol.h"
#include "Network/CcIcmpProtocol.h"
#include "Network/CcIpProtocol.h"
#include "Network/CcIpSettings.h"
#include "CcConsole.h"

CNetworkTest::CNetworkTest() :
  CcTest("CNetworkTest")
{
  appendTestMethod("Test Ipv4 conversion", &CNetworkTest::testIpv4);
  appendTestMethod("Test Mac conversion", &CNetworkTest::testMac);
  appendTestMethod("Test Subnet conversion", &CNetworkTest::testSubnet);
  appendTestMethod("Test Network Stack simulation", &CNetworkTest::testNetworkStack);
  appendTestMethod("Test ICMP Checksum generation", &CNetworkTest::testIcmpChecksum);
  appendTestMethod("Test UDP Checksum generation", &CNetworkTest::testUdpChecksum);
  appendTestMethod("Test TCP Checksum generation", &CNetworkTest::testTcpChecksum);
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

bool CNetworkTest::testSubnet()
{
  bool bSuccess = false;
  CcIpSettings oIpSettings1;
  CcIpSettings oIpSettings2;
  oIpSettings1.setSubnet("255.255.255.0");
  oIpSettings2.setSubnet(24);
  if (oIpSettings1 == oIpSettings2)
  {
    oIpSettings2.setSubnet(10);
    CcIp oIp = oIpSettings2.getSubnetIp();
    CcIp oIpToVerifiy(255, 192, 0, 0);
    if (oIp == oIpToVerifiy)
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}

bool CNetworkTest::testNetworkStack()
{
  bool bRet = true;
  CcNetworkStack oNetworkStack;
  bRet = oNetworkStack.init();
  return bRet;
}

bool CNetworkTest::testIcmpChecksum()
{
  bool bRet = false;
  uint8 pBuffer[] =  { 0x08, 0x00, 0x36, 0xb8, 0x69, 0xe4, 0x00, 0x05, 0x81, 0xd0, 0xa2, 0x5c, 0x00, 0x00, 0x00, 0x00,
                       0x72, 0x5e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                       0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                       0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 };
  CcIcmpProtocol::CHeader* pHeader = CCVOIDPTRCAST(CcIcmpProtocol::CHeader*, pBuffer);
  pHeader->uiChecksum = 0;
  uint16 uiChecksum = CcIpProtocol::generateChecksum(CCVOIDPTRCAST(uint16*, pHeader), sizeof(pBuffer));
  if(uiChecksum == 0xb836)
  {
    bRet = true;
  }
  return bRet;
}

bool CNetworkTest::testUdpChecksum()
{
  bool bRet = false;
  // Test example from https://www.securitynik.com/2015/08/calculating-udp-checksum-with-taste-of.html
  // Generate a Udp packet with "Hi" as content
  uint16 uiFrameLength = sizeof(CcUdpProtocol::CHeader) + 2;
  CCNEWARRAYTYPE(uiData, uint8, uiFrameLength);
  uiData[uiFrameLength - 2] = 'H';
  uiData[uiFrameLength - 1] = 'i';
  CcUdpProtocol::CHeader* pHeader = CCVOIDPTRCAST(CcUdpProtocol::CHeader*, uiData);
  pHeader->uiChecksum = 0;
  pHeader->uiDestPort = CcStatic::swapInt16(10);
  pHeader->uiSrcPort  = CcStatic::swapInt16(20);
  pHeader->uiLength   = CcStatic::swapInt16(uiFrameLength);
  CcIp oDestIp(192, 168, 0, 30);
  CcIp oSourceIp(192, 168, 0, 31);
  pHeader->generateChecksum(oDestIp, oSourceIp);
  uint16 uiChecksum = pHeader->uiChecksum;
  if (uiChecksum == 0x35C5)
  {
    bRet = true;
  }
  CCDELETEARR(uiData);
  return bRet;
}

bool CNetworkTest::testTcpChecksum()
{
  bool bRet = false;
  // Test example from https://www.securitynik.com/2015/08/calculating-udp-checksum-with-taste-of_3.html
  // Generate a Udp packet with "Hi" as content
  uint16 uiFrameLength = sizeof(CcTcpProtocol::CHeader) + 2;
  CCNEWARRAYTYPE(uiData, uint8, uiFrameLength);
  uiData[uiFrameLength - 2] = 'H';
  uiData[uiFrameLength - 1] = 'i';
  CcTcpProtocol::CHeader* pHeader = CCVOIDPTRCAST(CcTcpProtocol::CHeader*, uiData);
  CcStatic_memsetZeroPointer(pHeader);
  pHeader->uiDestPort = CcStatic::swapInt16(10);
  pHeader->uiSrcPort = CcStatic::swapInt16(20);
  pHeader->uiSeqnum = CcStatic::swapInt16(10);
  pHeader->uiWindow = CcStatic::swapInt16(8192);
  pHeader->uiHdrLenAndFlags = CcStatic::swapInt16(0x5002);
  CcIp oDestIp(192, 168, 0, 30);
  CcIp oSourceIp(192, 168, 0, 31);
  pHeader->generateChecksum(oDestIp, oSourceIp, 2, uiData + sizeof(CcTcpProtocol::CHeader));
  uint16 uiChecksum = CcStatic::swapUint16(pHeader->uiChecksum);
  if (uiChecksum == 0xc5c1)
  {
    bRet = true;
  }
  CCDELETEARR(uiData);
  return bRet;
}
