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
 * @page      CcUtil
 * @subpage   WindowsProcessThread
 *
 * @page      WindowsProcessThread
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsProcessThread
 **/
#ifndef WindowsProcessThread_H_
#define WindowsProcessThread_H_

#include "CcBase.h"
#include "CcProcess.h"
#include "CcThreadObject.h"
#include "CcHandle.h"

/**
 * @brief Class impelmentation
 */
class WindowsProcessThread : public CcThreadObject
{
public:
  /**
   * @brief Constructor
   */
  WindowsProcessThread(CcProcess& m_rProcess);

  /**
   * @brief Destructor
   */
  virtual ~WindowsProcessThread( void );


private:
  void run() override;
  CcHandle<CcProcess> m_hProcess;
};

#endif /* WindowsProcessThread_H_ */