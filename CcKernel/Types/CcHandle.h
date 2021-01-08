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
 * @page      Types
 * @subpage   CcHandle
 *
 * @page      CcHandle
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHandle
 */
#ifndef H_CcHandle_H_
#define H_CcHandle_H_

#include "CcBase.h"
#include "CcBase.h"

/**
 * @brief Communication Device for I2C
 */
template <typename TYPE>
class CcHandle
{
public:
  /**
   * @brief Constructor
   */
  CcHandle()
  {
  }

  /**
   * @brief Copy-Constructor
   */
  CcHandle(const CcHandle<TYPE>& oToCopy)
  {
    copy(oToCopy);
  }

  /**
   * @brief Constructor
   */
  CcHandle(TYPE* oToCopy)
  {
    create(oToCopy);
  }

  /**
   * @brief Destructor
   */
  ~CcHandle()
  {
  }

  void copy(const CcHandle<TYPE>& oToCopy)
  {
    m_pPointer = oToCopy.m_pPointer;
  }

  void create(TYPE* oToCopy)
  {
    m_pPointer = oToCopy;
  }

  TYPE* ptr() const
  {
    return m_pPointer;
  }

  template <class X>
  const CcHandle<X> cast() const
  {
    CcHandle<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer));
    return oXRet;
  }

  template <class X>
  CcHandle<X> cast()
  {
    CcHandle<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer));
    return oXRet;
  }

  TYPE* operator->() const { return m_pPointer;}
  CcHandle<TYPE>& operator=(CcHandle<TYPE>&& oToMove)
    { if (this != &oToMove) { m_pPointer = oToMove.m_pPointer; oToMove.m_pPointer = nullptr;} return *this; }
  CcHandle<TYPE>& operator=(const CcHandle<TYPE>& oToCopy)
    { copy(oToCopy); return *this;}
  CcHandle<TYPE>& operator=(TYPE* oToCopy)
    { copy(oToCopy); return *this;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcHandle<TYPE>& oToCompare) const
    { return static_cast<void*>(m_pPointer) == static_cast<void*>(oToCompare.m_pPointer); }
  inline bool operator==(const TYPE* pToCompare) const
    { return static_cast<const void*>(m_pPointer) == static_cast<const void*>(pToCompare); }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcHandle<TYPE>& oToCompare) const
    { return static_cast<void*>(m_pPointer) != static_cast<void*>(oToCompare.m_pPointer); }
  inline bool operator!=(const TYPE* pToCompare) const
    { return static_cast<const void*>(m_pPointer) != static_cast<const void*>(pToCompare); }

  void setPointer(TYPE* pToSet)
    { m_pPointer = pToSet; }

  bool isValid() const
    { return static_cast<void*>(m_pPointer) != nullptr; }

  inline operator TYPE*() const
    { return m_pPointer; }
  inline operator const TYPE*() const
    { return m_pPointer; }
  inline void* getVoidPtr() const
    { return static_cast<void*>(m_pPointer); }
private:
  TYPE* m_pPointer   = nullptr;
};
#endif // H_CcHandle_H_
