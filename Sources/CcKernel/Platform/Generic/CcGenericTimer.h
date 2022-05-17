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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Class CcGenericTimer
 */
#pragma once

#include "CcBase.h"
#include "Devices/ITimer.h"

class CcDateTime;

/**
 * @brief Linux Timer Device for triggert events
 * @todo Implementation is not yet done for timers
 */
class CcGenericTimer : public ITimer
{
public: //methods
  CcGenericTimer();
  virtual ~CcGenericTimer();

  virtual CcStatus onState(EState eState) override;
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;

private: //methods
  CCDEFINE_COPY_DENIED(CcGenericTimer)
  class CPrivate;
  CPrivate* m_pPrivate = nullptr;
};
