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
 * @page      Driver
 * @subpage   CcDriverLoad
 *
 * @page      CcDriverLoad
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language: C++11
 * @brief     Class CcDriverLoad
 **/
#ifndef H_CcDriverLoad_H_
#define H_CcDriverLoad_H_

#include "CcBase.h"
#include "CcVector.h"
#include "IDriver.h"
#include "IDevice.h"

/**
 * @brief Class impelmentation
 */
class CcDriverLoad
{
public:
  CcVector<IDriver*>& getDriverList()
    { return m_DriverList; }
  void init(int iLoadNr)
  {
    if(iLoadNr >= 0 && s_iState < 0)
    {
      load0();
      s_iState = 0;
    }
    if(iLoadNr >= 1 && s_iState < 1)
    {
      load1();
      s_iState = 1;
    }
    if(iLoadNr >= 2 && s_iState < 2)
    {
      load2();
      s_iState = 2;
    }
    if(iLoadNr >= 3 && s_iState < 3)
    {
      load3();
      s_iState = 3;
    }
  }
  void deinit()
  {
    for (IDriver* pDriver : m_DriverList)
    {
      pDriver->unload();
      CCDELETE(pDriver);
    }
    m_DriverList.clear();
  }
  
  void load(EDeviceType eType);

private:
  void load0();
  void load1();
  void load2();
  void load3();

public: // Member
  CcVector<IDriver*> m_DriverList;
  static int s_iState;
};

#endif // H_CcDriverLoad_H_
