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
 * @brief     Class CcTimer
 */
#pragma once

#include "CcBase.h"
#include "Devices/ITimer.h"

#ifdef LINUX
  #include "Platform/Linux/CcLinuxTimer.h"
  typedef CcLinuxTimer CcTimer;

#elif defined(WINDOWS)
  #include "Platform/Windows/CcWindowsTimer.h"
  typedef CcWindowsTimer CcTimer;
#elif defined(GENERIC)
  #include "Platform/Generic/CcGenericTimer.h"
  typedef CcGenericTimer CcTimer;
#else // Further and unkown Timer definition
  class CcDateTime;

  /**
   * @brief Timer for triggered events
   */
  class CcKernelSHARED CcTimer : public ITimer
  {
  public: //methods
    CcTimer() = default;
    virtual ~CcTimer();

    virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;
  };
#endif
