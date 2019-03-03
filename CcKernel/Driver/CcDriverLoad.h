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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language: C++11
 * @brief     Class CcDriverLoad
 **/
#ifndef _CcDriverLoad_H_
#define _CcDriverLoad_H_

#include "CcBase.h"
#include "CcVector.h"
#include "CcDriver.h"

/**
 * @brief Class impelmentation
 */
class CcDriverLoad
{
public:
  CcVector<CcDriver*>& getDriverList()
    { return m_DriverList; }
  void init()
    { if(s_bInitDone==false){s_bInitDone=true;bootLoad();} }

private:
  void bootLoad();

public: // Member
  CcVector<CcDriver*> m_DriverList;
  static bool s_bInitDone;
};

#endif /* _CcDriverLoad_H_ */
