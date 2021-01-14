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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxProcessThread
 **/
#ifndef H_CcLinuxProcessThread_H_
#define H_CcLinuxProcessThread_H_

#include "CcBase.h"
#include "CcProcess.h"
#include "CcHandle.h"

/**
 * @brief Class impelmentation
 */
class CcLinuxProcessThread : public CcProcessThread
{
public:
  /**
   * @brief Constructor
   */
  CcLinuxProcessThread(CcProcess& m_rProcess);

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxProcessThread();

private:

private:
  virtual void run() override;
  virtual void onStop() override;
  void kill();
  void term();
  CcHandle<CcProcess> m_hProcess;
  int                 m_iChildId = -1;
};

#endif // H_CcLinuxProcessThread_H_
