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
 * @page      Types
 * @subpage   CcEventActionLoop
 *
 * @page      CcEventActionLoop
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEventActionLoop
 */
#ifndef H_CcEventActionLoop_H_
#define H_CcEventActionLoop_H_

//! Forward Declaration
#include "CcBase.h"
#include "CcMutex.h"
#include "CcEventActionList.h"

class CcEventAction;

class CcKernelSHARED CcEventActionLoop : CcMutex
{
public:
  void loop();
  void appendAction(CcEventAction* pAction);
  virtual bool onLoop() = 0;
  
private:
  CcEventActionList m_oEvents;
};

#endif // H_CcEventActionLoop_H_
