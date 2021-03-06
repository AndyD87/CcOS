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
 * @page      CcKernel
 * @subpage   CcTimer
 *
 * @page      CcTimer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTimer
 */

#ifndef H_CCTIMER_H_
#define H_CCTIMER_H_

#include "CcBase.h"
#ifdef LINUX
#include "Platform/Linux/CcLinuxTimer.h"
typedef CcLinuxTimer CcTimer;

#elif defined(WINDOWS)
#include "Platform/Windows/CcWindowsTimer.h"
typedef CcWindowsTimer CcTimer;

#elif defined(GENERIC)

#include "IDevice.h"
#include "Platform/Generic/CcGenericTimer.h"
typedef CcGenericTimer CcTimer;

#else
class CcDateTime;

/**
 * @brief Abstract Timer Device for triggered events
 * @todo Implementation is not yet done for timers
 */
class CcKernelSHARED CcTimer : public CcObject
{
public: //methods
  CcTimer();
  virtual ~CcTimer();

  CcStatus setTimeout(const CcDateTime& oTimeout);
  CcStatus setRepeates(size_t uiRepeates);

  void registerOnTimeout(const CcEvent& hEventHandle)
    { m_oEventHandler.append(hEventHandle); }

  size_t getRepeates() const
    { return m_uiRepeates; }
  size_t getCurrentRepeates()
    { return m_uiRepeates; }
  /**
   * @brief Call this method if timeout is reached.
   *        If this method return true, last queried event is reached.
   * @return True if last event was reached.
   */
  virtual bool timeout();
private:
  CcEventHandler m_oEventHandler;
  size_t m_uiRepeates     =0;
  size_t m_uiRepeatesCount=0;
};
#endif
#endif // H_CcTIMER_H_
