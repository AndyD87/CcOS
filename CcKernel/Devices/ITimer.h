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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ITimer
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "IDevice.h"
#include "CcEventHandler.h"

class CcDateTime;

/**
 * @brief Interface for timer devices
 */
class CcKernelSHARED ITimer : public IDevice
{
public: //methods
  ITimer() = default;
  virtual ~ITimer() = default;

  /**
   * @brief Set timeout the timer should call
   * @param oTimeout: Target timeout in date time format
   * @return Status of operation.
   */
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) = 0;

  /**
   * @brief Set number of repeates of timeout.
   *        After that, timer will stop.
   * @param uiRepeates: Number of repeates
   * @return Status of operation.
   */
  virtual CcStatus setRepeates(size_t uiRepeates);

  /**
   * @brief Register timout event to receive the timeout signal
   * @param hEventHandle: Eventhandle to call
   */
  void registerOnTimeout(const CcEvent& hEventHandle)
  { m_oEventHandler.append(hEventHandle); }

  //! @return Get number of timeouts left
  size_t getRepeates() const
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
