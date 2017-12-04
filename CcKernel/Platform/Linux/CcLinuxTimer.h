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
 * @page      Linux
 * @subpage   CcLinuxTimer
 *
 * @page      CcLinuxTimer
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Class CcLinuxTimer
 */
#ifndef CcLinuxTimer_H_
#define CcLinuxTimer_H_

#include "CcBase.h"
#include "Devices/CcTimer.h"

/**
 * @brief Linux Timer Device for triggert events
 * @todo Implementation is not yet done for timers
 */
class CcLinuxTimer : public CcTimer
{
public: //methods
  CcLinuxTimer();
  virtual ~CcLinuxTimer();
private: //methods
};

#endif /* CcLinuxTimer_H_ */
