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
 * @brief     Implemtation of class CUrlTest
 */
#include "CUrlTest.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcUrl.h"
#include "CcConsole.h"

#define TESTURL "https://username:password@hostname.dom:80/Path/to"
const CcString sTestUrl(TESTURL);

CUrlTest::CUrlTest( void )
{
}

CUrlTest::~CUrlTest( void )
{
}

bool CUrlTest::test()
{
  bool bSuccess = true;
  bSuccess &= testConstructors();
  bSuccess &= testExampleUrl();
  if(!bSuccess)
  {
    CcConsole::writeLine("CUrlTest failed");
  }
  return bSuccess;
}

bool CUrlTest::testConstructors()
{
  bool bSuccess = false;
  CcUrl oUrl1(TESTURL);
  CcUrl oUrl2(sTestUrl);
  if (TESTURL == sTestUrl &&  // Make a short string test
      oUrl1 == oUrl2 )
  {
    if(oUrl1.getHostname() == "hostname.dom" &&
       oUrl1.getPassword() == "password" &&
       oUrl1.getPath()     == "/Path/to" &&
       oUrl1.getPort()     == 80 &&
       oUrl1.getProtocol() ==  "https" &&
       oUrl1.getUsername() == "username")
    {
      bSuccess = true;
    }

  }
  return bSuccess;
}

bool CUrlTest::testExampleUrl()
{
  bool bSuccess = true;
  
  return bSuccess;
}
