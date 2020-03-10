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
 * @page      CcKernel
 * @subpage   CcReferenceCount
 *
 * @page      CcReferenceCount
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcReferenceCount
 **/
#ifndef H_CcReferenceCount_H_
#define H_CcReferenceCount_H_

#include "CcBase.h"

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcReferenceCount 
{
public:
  /**
   * @brief Constructor
   */
  CcReferenceCount(size_t uiInitValue = 1);

  bool referenceCountIsNull() const
    { return m_uiRefCount == 0; }
  void referenceCountIncrement()
    { m_uiRefCount++; }
  bool referenceCountDecrement()
    { m_uiRefCount--; return m_uiRefCount==0; }
private:
  size_t m_uiRefCount;
};

#endif // H_CcReferenceCount_H_
