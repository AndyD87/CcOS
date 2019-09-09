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
 * @subpage   ISdCard
 *
 * @page      ISdCard
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ISdCard
 */

#ifndef H_CCSDCARD_H_
#define H_CCSDCARD_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IIo.h"

/**
 * @brief This class should represent SD Card Devices.
 *        It's currently not working, it's just imported from an other source.
 */
class CcKernelSHARED ISdCard
{
public:
  ISdCard();
  virtual ~ISdCard();

  bool getAddr(uint32 Address, char* cReadBuf, uint32 length);

private:
  uint32 m_uiBlockSize;
  uint32 m_uiSDSize;

  IIo *m_DeviceCom;
};

#endif /* H_CCSDCARD_H_ */
