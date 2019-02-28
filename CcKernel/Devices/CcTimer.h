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
 * @page      Devices
 * @subpage   CcTimer
 *
 * @page      CcTimer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTimer
 */

#ifndef _CCTIMER_H_
#define _CCTIMER_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcDevice.h"
#include "CcEventHandler.h"

class CcDateTime;

/**
 * @brief Abstract Timer Device for triggered events
 * @todo Implementation is not yet done for timers
 */
class CcKernelSHARED CcTimer : public CcDevice
{
public: //methods
  CcTimer() = default;
  virtual ~CcTimer() = default;

  virtual CcStatus setTimeout(const CcDateTime& oTimeout) = 0;
  virtual CcStatus setRepeates(size_t uiRepeates);

  virtual CcStatus start() = 0;
  virtual CcStatus stop() = 0;

  void onTimeout(CcEventHandle hEventHandle)
    { m_oEventHandler.append(hEventHandle); }

  size_t getRepeates() const
    { return m_uiRepeates; }
  size_t getCurrentRepeates()
    { return m_uiRepeates; }
protected:
  /**
   * @brief Call this method if timeout is reached.
   *        If this method return true, last queried event is reached.
   * @return True if last event was reached.
   */
  bool timeout();
private:
  CcEventHandler m_oEventHandler;
  size_t m_uiRepeates     =0;
  size_t m_uiRepeatesCount=0;
};

#endif /* _CCTIMER_H_ */
