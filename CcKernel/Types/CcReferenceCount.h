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
 * @brief     Class CcReferenceCount
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief Use this class to count the number of references to an object.
 *        This can be used to avoid deletion before all references are removed.
 *
 *        By inheriting this class, the target class gets the ability to count the instances.
 *        If it is not possible to inherit this class, but the references needs to be counted,
 *        look for CcCoReferenceCount to copy and move parallel to the counting object.
 */
class CcKernelSHARED CcReferenceCount
{
public:
  /**
   * @brief Constructor
   */
  CcReferenceCount(size_t uiInitValue = 1);

  //! @return Get current reference count
  size_t referenceCount() const
  { return m_uiRefCount; }
  //! @return True if reference count reached 0
  bool referenceCountIsNull() const
  { return m_uiRefCount == 0; }

  //! @brief Increment reference count
  void referenceCountIncrement()
  { m_uiRefCount++; }
  //! @brief Decrement reference count
  bool referenceCountDecrement()
  { m_uiRefCount--; return m_uiRefCount==0; }
private:
  size_t m_uiRefCount;
};
