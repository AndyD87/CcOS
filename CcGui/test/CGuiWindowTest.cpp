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
 * @brief     Implemtation of class CGuiWindowTest
 */
#include "CGuiWindowTest.h"
#include "CcKernel.h"
#include "CcWindow.h"
#include "CcTimer.h"
#include "CcGuiApplication.h"

class CGuiWindowTestApp : public CcGuiApplication
{
public:
  CGuiWindowTestApp() :
    CcGuiApplication("CGuiWindowTest")
  {}
};

CGuiWindowTest::CGuiWindowTest() :
  CcTest("CGuiWindowTest")
{
  appendTestMethod("Test create a Window", &CGuiWindowTest::testCreateWindow);
}

CGuiWindowTest::~CGuiWindowTest()
{
}

bool CGuiWindowTest::testCreateWindow()
{
  CcStatus oStatus = false;
  CcTimer oTimer;
  CGuiWindowTestApp oApplication;
  oTimer.setTimeout(CcDateTimeFromSeconds(5));
  oTimer.setRepeates(1);
  oTimer.registerOnTimeout(NewCcEvent(CGuiWindowTestApp, void, CGuiWindowTestApp::stop, &oApplication));
  oTimer.setState(IDevice::EState::Start);
  oApplication.exec();
  return oStatus;
}
