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
 * @brief     Class II2C
 */
#include "II2C.h"

II2C::II2C()
{
}

II2C::~II2C()
{
}

II2CSlave::~II2CSlave()
{
}

size_t II2CSlave::readRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize)
{
  size_t uiRead = write(&uiRegister, sizeof(uiRegister));
  if(uiRead == sizeof(uiRegister))
  {
    uiRead = read(pBuffer, uiSize);
  }
  return uiRead;
}

size_t II2CSlave::readRegister16(uint16 uiRegister, void* pBuffer, size_t uiSize)
{
  size_t uiRead = write(&uiRegister, sizeof(uiRegister));
  if(uiRead == sizeof(uiRegister))
  {
    uiRead = read(pBuffer, uiSize);
  }
  return uiRead;
}

size_t II2CSlave::writeRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize)
{
  size_t uiWritten = write(&uiRegister, sizeof(uiRegister));
  if(uiWritten == sizeof(uiRegister))
  {
    uiWritten = write(pBuffer, uiSize);
  }
  return uiWritten;
}

size_t II2CSlave::writeRegister16(uint16 uiRegister, void* pBuffer, size_t uiSize)
{
  size_t uiWritten = write(&uiRegister, sizeof(uiRegister));
  if(uiWritten == sizeof(uiRegister))
  {
    uiWritten = write(pBuffer, uiSize);
  }
  return uiWritten;
}
