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
 * @page      Windows
 * @subpage   CcWindowsProcessThread
 *
 * @page      CcWindowsProcessThread
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsProcessThread
 **/
#ifndef _CcWindowsProcessThread_H_
#define _CcWindowsProcessThread_H_

#include "CcBase.h"
#include "CcProcess.h"
#include "CcHandle.h"

class CcWindowsProcessThreadPrivate;

/**
 * @brief Class impelmentation
 */
class CcWindowsProcessThread : public CcProcessThread
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsProcessThread(CcProcess& m_rProcess);

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsProcessThread( void );

private:
  virtual void run() override;
  virtual void onStop() override
    { term(); }
  void kill();
  void term();

private:
  CcWindowsProcessThreadPrivate * m_pPrivate = nullptr;
  CcHandle<CcProcess> m_hProcess;
};

#endif /* _CcWindowsProcessThread_H_ */
