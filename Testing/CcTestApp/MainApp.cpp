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
 * @brief     Class MainApp
 *
 *  Implementation of Main Application
 */

#include "MainApp.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcText.h"
#include "CcTaskbar.h"
#include "CcHttpClient.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonObject.h"
#include "CcFile.h"
#include "CcConsole.h"
#include "CcDateTime.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "CcPushButton.h"

MainApp::MainApp() :
  CcGuiApplication("MainApp")
{
  m_pButton =  new CcPushButton(getWindow()->getHandle());
  CCMONITORNEW(m_pButton);
  m_pButton->setText("Hallo");
  m_pButton->setBackgroundColor(CcColor(0xff, 0x00, 0x00));
}

MainApp::~MainApp() 
{
  CCDELETE(m_pButton);
}

void runGoogle()
{
  CcHttpClient oClient;
  oClient.setUrl("https://google.de");
  oClient.execGet();
  CcString sReturn = oClient.getByteArray();
  CcConsole::writeString(sReturn);
}

void runClientTest()
{
  runGoogle();
  CcStringList oCoins;
  CcHttpClient oClient;
  // Test JSON with big file described in https://yobit.net/en/api/
  oClient.setUrl("https://yobit.net/api/3/info");
  oClient.execGet();
  CcString sReturn = oClient.getByteArray();
  CcJsonDocument oDoc;
  CcDateTime oStart = CcKernel::getDateTime();
  if (oDoc.parseDocument(sReturn) &&
      oDoc.getJsonData().isObject() &&
      oDoc.getJsonData().object().contains("pairs"))
  {
    CcJsonObject& rPairs = oDoc.getJsonData().object()["pairs"].object();
    for (CcJsonData& rPair : rPairs)
    {
      CcStringList slPair = rPair.getName().split("_");
      if (slPair.size() == 2)
      {
        if (!oCoins.contains(slPair[0]))
          oCoins.append(slPair[0]);
        if (!oCoins.contains(slPair[1]))
          oCoins.append(slPair[1]);
      }
      else
      {
        CCDEBUG("Error");
      }
    }
    for (CcString& sCoin : oCoins)
    {
      CcConsole::writeString(sCoin + " ");
    }
    CcConsole::writeLine(CcString::fromNumber(oCoins.size()));
  }
  CcDateTime oEnd = CcKernel::getDateTime();
  CcConsole::writeLine(CcString::fromNumber(oEnd.getTimestampMs() - oStart.getTimestampMs()));
  CcConsole::readLine();
}