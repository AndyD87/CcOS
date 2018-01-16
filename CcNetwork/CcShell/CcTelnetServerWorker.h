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
 * @page      CcShell
 * @subpage   CcTelnetServerWorker
 *
 * @page      CcTelnetServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTelnetServerWorker
 */
#ifndef _CcTelnetServerWorker_H_
#define _CcTelnetServerWorker_H_

#include "CcBase.h"
#include "CcShell.h"
#include "CcWorker.h"
#include "Network/CcSocket.h"

/**
 * @brief Button for GUI Applications
 */
class CcShellSHARED CcTelnetServerWorker : public CcWorker
{
public:
  /**
   * @brief Constructor
   */
  CcTelnetServerWorker( CcSocket Socket );

  /**
   * @brief Destructor
   */
  virtual ~CcTelnetServerWorker( void );

  void run(void) override;

private:
  CcSocket  m_Socket;
  CcShell   m_Shell;
};

#endif /* _CcTelnetServerWorker_H_ */
