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
 * @page      CcThreadManager
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcThreadManager
 */
#ifndef CCTHREADMANAGER_H_
#define CCTHREADMANAGER_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcThreadObject.h"
#include "CcString.h"
#include "CcVector.h"

#ifdef WIN32
template class CcKernelSHARED CcVector<CcThreadObject*>;
#endif

/**
 * @brief Default Class to create a Application
 */
class CcKernelSHARED CcThreadManager {
public:
  CcThreadManager(void);

  virtual ~CcThreadManager();

  void addThread(CcThreadObject &oThread);
  void closeAll(void);

public:
  CcVector<CcThreadObject*> m_ThreadList;
  static const int c_iThreadWaitingTime = 10;
  static const int c_iThreadDelayTime = 1000 / c_iThreadWaitingTime;
};

#endif /* CCTHREADMANAGER_H_ */
