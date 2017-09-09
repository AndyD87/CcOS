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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: 
 * @version    0.01
 * @date       2016-04
 * @par        Language   C++ ANSI V3
 * @file     main.cpp
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CcSqlDatabase.h"
#include "CcString.h"
#include "Devices/CcGPIOPort.h"
//#include "CcDism.h"

// Application entry point. 
int main(int argc, char **argv)
{
  CcKernel::setArg(argc, argv);
  CcKernel::initCLI();
  CcHandle<CcGPIOPort>cPort = CcKernel::getDevice(EDeviceType::GPIOPort, "System").cast<CcGPIOPort>();
  if(cPort != nullptr)
  {
    CcGPIOPin* cPin = cPort->getPin(21);
    if(cPin != nullptr)
    {
      cPin->setDirection(CcGPIOPin::EDirection::Output);
      for(int i=0; i< 100000; i++)
      {
        cPin->setValue(1);
        CcKernel::delayS(1);
        cPin->setValue(0);
        CcKernel::delayS(1);
      }
      cPin->setDirection(CcGPIOPin::EDirection::Input);
    }
  }
  CcSqlDatabase dbTest(ESqlDatabaseType::Sqlite);
  dbTest.setDatabase("Test.sqlite.db");
  dbTest.open();
  CcSqlResult sRes = dbTest.query("SELECT * FROM test");
  CcString str;
  CcStringList slColomnNames = sRes.getColumnNames();
  for (size_t i = 0; i < slColomnNames.size(); i++)
  {
    CCDEBUG(str+"\t");
  }
  for (size_t j = 0; j < sRes.size(); j++)
  {
    CcTableRow row(sRes.at(j));
    for (size_t k = 0; k < row.size(); k++)
    {
      CCDEBUG(row.at(k).getString() + "\t");
    }
  }
  dbTest.close();
  return 0;
}
