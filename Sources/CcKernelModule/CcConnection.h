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
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcConnection
 **/
#pragma once

#include "CcBase.h"

namespace NKernelModule
{

class IDevice;

/**
 * @brief Class impelmentation
 */
class CcConnection
{
public:
  /**
   * @brief Constructor
   */
  CcConnection(IDevice* pDevice);

  /**
   * @brief Destructor
   */
  ~CcConnection();

  //! @return Get device interface of connection
  IDevice* getDevice() const
  { return m_pDevice; }

private:
  class CContext;
  IDevice*      m_pDevice;
  CContext*     m_pContext = nullptr;
};

}
