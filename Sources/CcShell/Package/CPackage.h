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
 * @brief     Class CPackage
 */
#pragma once

#include "CcShellBase.h"

#pragma pack(push, 1)
namespace NShell
{
/**
 * @brief Basic input transport for shell
 */
class CcShellSHARED CPackage
{
public:
  enum class EType : uint16
  {
    None  = 0,
    Login = 1,
    Input = 2,
  };
  /**
   * @brief Default shell instance with reading and writin from stdin.
   */
  CPackage() = default;

  /**
   * @brief Destructor
   */
  ~CPackage() = default;

  CPackage* create(uint16 uiPayload);
  void      remove(CPackage* pPackage);

  uint16 uiType;
  uint16 uiSize;
  char   pData[1];
};
}

#pragma pack(pop)