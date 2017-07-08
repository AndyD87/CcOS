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
 * @brief     Implemtation of class CXmlTest
 */
#include "CXmlTest.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlDocument.h"

const char cTestXml[] = "<CcSyncClient>                          \
  <Account>                                                        \
    <Name></Name>                                                    \
    <Password Type = \"SHA256\">Test</Password>                      \
    <Server>                                                         \
      <Host>backup.adirmeier.de</Host>                                 \
      <Port>27500</Port>                                               \
      <Ssl>true</Ssl>                                                  \
    </Server>                                                        \
    <User>                                                           \
      <!--Additional Users, with different rights and credentials-->   \
    </User>                                                          \
    <Database> </Database>                                           \
    <Module>                                                         \
      <Name>Bilder</Name>                                              \
      <!--Unique ModuleName from server-->                             \
      <Location>D:\\</Location>                                        \
      <!--Location of Module on current Machine-->                     \
      <IgnoreHiddenFiles>false</IgnoreHiddenFiles>                     \
    </Module>                                                        \
    <Commands>                                                       \
      <Command>                                                        \
        <Name>                                                    \
        </Name><Executable> </Executable>                                       \
        <Parameters> </Parameters>                                       \
        <!--Use $Module : Name as relative Path-->                       \
        <WorkingDirectory> </WorkingDirectory>                           \
        <TimeMask> </TimeMask>                                           \
     </Command>                                                       \
    </Commands>                                                      \
  </Account>                                                       \
</CcSyncClient>";
const char cTestXmlCompact[] = "<CcSyncClient><Account><Name></Name><Password Type=\"SHA256\">Test</Password><Server><Host>backup.adirmeier.de</Host><Port>27500</Port><Ssl>true</Ssl></Server><User><!--Additional Users, with different rights and credentials--></User><Database></Database><Module><Name>Bilder</Name><!--Unique ModuleName from server--><Location>D:\\</Location><!--Location of Module on current Machine--><IgnoreHiddenFiles>false</IgnoreHiddenFiles></Module><Commands><Command><Name></Name><Executable></Executable><Parameters></Parameters><!--Use $Module : Name as relative Path--><WorkingDirectory></WorkingDirectory><TimeMask></TimeMask></Command></Commands></Account></CcSyncClient>";

CXmlTest::CXmlTest( void )
{
}

CXmlTest::~CXmlTest( void )
{
}

bool CXmlTest::test()
{
  bool bSuccess = true;
  bSuccess &= testRead();
  bSuccess &= testWrite();
  return bSuccess;
}

bool CXmlTest::testRead()
{
  bool bRet = true;
  CcXmlDocument oDocument(cTestXml);
  CcXmlDocument oDocument2(cTestXml);
  CcXmlNode& pRootNode = oDocument.rootNode();
  CcXmlNode& pNewRootNode = pRootNode.getNode("CcSyncClient");
  CcXmlNode& pNullNode = pRootNode.getNode("NotExisting");
  if (  pNullNode.isNull() &&
        pNewRootNode.isNotNull())
  {
    if (pNewRootNode.getName() != "CcSyncClient")
    {
      bRet = false;
    }
    else
    {
      CCDEBUG(pNewRootNode.innerText());
      CCDEBUG(pNewRootNode.innerXml());
    }
  }
  else
  {
    bRet = false;
  }
  if (bRet == true)
  {
    // test if reset node was complete
    oDocument2.parseDocument(cTestXmlCompact);
    if (oDocument.rootNode() == oDocument2.rootNode())
    {
      CcString sInnerNode1 = oDocument.getXmlDocument();
      CcString sInnerNode2 = oDocument2.getXmlDocument();
      if (sInnerNode1 != sInnerNode2)
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

bool CXmlTest::testWrite()
{
  bool bRet = false;
  CcXmlDocument oDocument1(cTestXml);
  CcXmlDocument oDocument2(cTestXmlCompact);
  CcString oDocument2Output = oDocument2.getXmlDocument(true);
  CcXmlDocument oDocument3(oDocument2Output);
  if (oDocument1.rootNode() == oDocument2.rootNode() && 
      oDocument2.rootNode() == oDocument3.rootNode())
  {
    bRet = true;
  }

  return bRet;
}
