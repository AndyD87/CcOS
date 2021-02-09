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
 * @brief     Class CcCoReferenceCount
 **/
#pragma once

#include "CcBase.h"
#include "CcReferenceCount.h"

/**
 * @brief This class workes like CcReferenceCount but it is copy save.
 *        Use this class parallel to the countable object to count it's references.
 *
 *        If the countable object is editable and can be changed, the common CcReferenceCount
 *        is more efficent than this.
 */
class CcKernelSHARED CcCoReferenceCount
{
public:
  /**
   * @brief Constructor
   */
  CcCoReferenceCount(size_t uiInitValue = 1);
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcCoReferenceCount)
  ~CcCoReferenceCount();

  /**
   * @brief Copy value from other reference to this and increase.
   * @param rToCopy: Reference to copy
   * @return Handle to this
   */
  CcCoReferenceCount& operator=(const CcCoReferenceCount& rToCopy);

  //! @return Get current reference count
  size_t referenceCount() const
  { return m_pRefCount->referenceCount(); }
  //! @return True if reference count reached 0
  bool referenceCountIsNull() const
  { return m_pRefCount->referenceCountIsNull(); }

  //! @brief Increment reference count
  void referenceCountIncrement()
  { m_pRefCount->referenceCountIncrement(); }
  //! @brief Decrement reference count
  bool referenceCountDecrement()
  { m_pRefCount->referenceCountDecrement(); return referenceCountIsNull(); }
private:
  CcReferenceCount* m_pRefCount = nullptr;
};
