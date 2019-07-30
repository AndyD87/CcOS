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
 * @subpage   CcWindowsTimer
 *
 * @page      CcWindowsTimer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsTimer
 **/
#ifndef H_CcWindowsTimer_H_
#define H_CcWindowsTimer_H_

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "Devices/ITimer.h"
#include "CcDateTime.h"

/**
 * @brief Windows Timer Device for triggert events
 * @todo Implementation is not yet done for timers
 */
class CcWindowsTimer : public ITimer 
{
public: //methods
  CcWindowsTimer();
  virtual ~CcWindowsTimer();

  virtual CcStatus setTimeout(const CcDateTime& oTimeout)
    { CCUNUSED(oTimeout); return EStatus::NotSupported; }
  virtual CcStatus setRepeates(size_t uiRepeates)
    { CCUNUSED(uiRepeates); return EStatus::NotSupported; }
  virtual CcStatus start()
    { return EStatus::NotSupported; }
  virtual CcStatus stop()
    { return EStatus::NotSupported; }
};

#endif /* H_CcWindowsTimer_H_ */
