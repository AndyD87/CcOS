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
#ifndef H_CcLinuxTimer_H_
#define H_CcLinuxTimer_H_

#include "CcBase.h"
#include "Devices/ITimer.h"

class CcDateTime;

/**
 * @brief Linux Timer Device for triggert events
 * @todo Implementation is not yet done for timers
 */
class CcLinuxTimer : public ITimer
{
public: //methods
  CcLinuxTimer();
  virtual ~CcLinuxTimer();

  virtual CcStatus setState(EState eState) override;
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;

private: //methods
  class CPrivate;
  CPrivate* m_pPrivate;
};

#endif // H_CcLinuxTimer_H_
