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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Force implementations of interface only classes
 */

#include "Devices/CcDeviceEeprom.h"

size_t CcDeviceEeprom::read(void* pBuffer, size_t uSize)
{
  if (isValid()) return getDevice()->read(pBuffer, uSize);
  return SIZE_MAX;
}

size_t CcDeviceEeprom::write(const void* pBuffer, size_t uSize)
{
  if (isValid()) return getDevice()->write(pBuffer, uSize);
  return SIZE_MAX;
}

CcStatus CcDeviceEeprom::open(EOpenFlags eOpenFlags)
{
  if (isValid()) return getDevice()->open(eOpenFlags);
  return EStatus::DeviceError;
}

CcStatus CcDeviceEeprom::close()
{
  if (isValid()) return getDevice()->close();
  return EStatus::DeviceError;
}

CcStatus CcDeviceEeprom::cancel()
{
  if (isValid()) return getDevice()->cancel();
  return EStatus::DeviceError;
}

size_t CcDeviceEeprom::size() const
{
  if (isValid()) return getDevice()->size();
  return SIZE_MAX;
}

CcStatus CcDeviceEeprom::setPosition(size_t uiPosition)
{
  if (isValid()) return getDevice()->setPosition(uiPosition);
  return EStatus::DeviceError;
}
