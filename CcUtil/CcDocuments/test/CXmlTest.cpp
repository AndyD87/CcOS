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

CXmlTest::CXmlTest() :
  CcTest<CXmlTest>("CXmlTest")
{
  appendTestMethod("test xml file read", &CXmlTest::testRead);
  appendTestMethod("test xml file write", &CXmlTest::testWrite);
  appendTestMethod("test xml inner text", &CXmlTest::testInner);
  appendTestMethod("test xml outer xml of document", &CXmlTest::testDocOuter);
}

CXmlTest::~CXmlTest()
{
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
      CcString sInnerNode1 = oDocument.getDocument();
      CcString sInnerNode2 = oDocument2.getDocument();
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
  CcString oDocument2Output = oDocument2.getDocument(true);
  CcXmlDocument oDocument3(oDocument2Output);
  if (oDocument1.rootNode() == oDocument2.rootNode() && 
      oDocument2.rootNode() == oDocument3.rootNode())
  {
    bRet = true;
  }

  return bRet;
}

bool CXmlTest::testInner()
{
  bool bRet = false;
  CcString sSingleNode("<Test>Data</Test>");
  CcXmlDocument oDoc(sSingleNode);
  if(oDoc.rootNode().isNotNull() &&
     oDoc.rootNode()[0].isNotNull())
  {
    CcString sInnerSingleNode = oDoc.rootNode()[0].innerText();
    CcString sOuterSingleNode = oDoc.rootNode()[0].outerXml();
    if(sInnerSingleNode == "Data")
    {
      if(sOuterSingleNode == sSingleNode)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CXmlTest::testOuter()
{
  bool bRet = false;
  CcString sNode("<Test><Test>Data</Test></Test>");
  CcXmlDocument oDoc(sNode);
  if(oDoc.rootNode().isNotNull() &&
     oDoc.rootNode()[0].isNotNull())
  {
    CcString sInnerNode = oDoc.rootNode()[0].innerText();
    CcString sOuterNode = oDoc.rootNode()[0].outerXml();
    if(sInnerNode == "Data")
    {
      if(sOuterNode == sNode)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CXmlTest::testDocOuter()
{
  bool bRet = false;
  CcString sNode("<Test><Test>Data</Test></Test>");
  CcString sNodeInteded("<Test>\r\n  <Test>Data</Test>\r\n</Test>\r\n");
  CcXmlDocument oDoc(sNode);
  if(oDoc.rootNode().isNotNull() &&
     oDoc.rootNode()[0].isNotNull())
  {
    CcString sInnerNode = oDoc.rootNode()[0].innerText();
    CcString sOuterNode = oDoc.getDocument(false);
    CcString sOuterIntededNode = oDoc.getDocument(true);
    if(sInnerNode == "Data")
    {
      if(sOuterNode == sNode)
      {
        if(sNodeInteded == sOuterIntededNode)
        {
          bRet = true;
        }
      }
    }
  }
  return bRet;
}
