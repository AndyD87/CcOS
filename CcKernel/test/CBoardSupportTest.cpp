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
 * @brief     Implementation of Class CBoardSupportTest
 */

#include "CBoardSupportTest.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "Devices/IBoardSupport.h"
#include "Devices/Simulations/CcBoardSupportSimulation.h"

CBoardSupportTest::CBoardSupportTest() :
  CcTest("CBoardSupportTest")
{
  appendTestMethod("Setup board simulation", &CBoardSupportTest::testSimulationSetup);
  appendTestMethod("Verify board simulation", &CBoardSupportTest::testVerifySetup);
  appendTestMethod("Create some devices", &CBoardSupportTest::testCreateDevices);
  appendTestMethod("Remove board simulation", &CBoardSupportTest::testSimulationRemoving);
}

CBoardSupportTest::~CBoardSupportTest()
{
}

bool CBoardSupportTest::testSimulationSetup()
{
  bool bRet = false;
  CCNEW(m_pSimulation, CcBoardSupportSimulation);
  bRet = m_pSimulation != nullptr;
  return bRet;
}

bool CBoardSupportTest::testVerifySetup()
{
  bool bRet = false;
  if(m_pSimulation->getDeviceSize() == 5 &&
     m_pSimulation->getDeviceSize(EDeviceType::GpioPort) == 2 &&
     m_pSimulation->getDeviceSize(EDeviceType::Spi) == 1 &&
     m_pSimulation->getDeviceSize(EDeviceType::I2C) == 1 &&
     m_pSimulation->getDeviceSize(EDeviceType::Uart) == 1 &&
     m_pSimulation->getDeviceSize(EDeviceType::Camera) == 0
     )
    bRet = true;
  return bRet;
}

bool CBoardSupportTest::testCreateDevices()
{
  bool bRet = false;
  CcDevice oSpiDevice = m_pSimulation->createDevice(EDeviceType::Spi, 0);
  CcDevice oI2CDevice = m_pSimulation->createDevice(EDeviceType::I2C, 0);
  if(oSpiDevice.isValid())
  {
    // I2C shouldn't be created because it's pins are crossing spi.
    if(!oI2CDevice.isValid())
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CBoardSupportTest::testSimulationRemoving()
{
  bool bRet = false;
  CCDELETE(m_pSimulation);
  bRet = m_pSimulation == nullptr;
  return bRet;
}

