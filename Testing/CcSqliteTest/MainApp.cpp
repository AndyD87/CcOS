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
#include "CcSqlDatabase.h"

MainApp::MainApp() 
{
  appendTestMethod("Test sqlite3 create and open", &MainApp::testDatabaseCreate);
}

MainApp::~MainApp() 
{
}

bool  MainApp::testDatabaseCreate(void)
{
  bool bSuccess = true;
  CcSqlDatabase dbTest(ESqlDatabaseType::Sqlite);
  dbTest.setDatabase("Test.sqlite.db");
  dbTest.open();
  CcSqlResult sRes = dbTest.query("SELECT * FROM test");
  CcString str;
  CcStringList slColomnNames = sRes.getColumnNames();
  for (size_t i = 0; i < slColomnNames.size(); i++)
  {
    CCDEBUG(str + "\t");
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
  return bSuccess;
}
