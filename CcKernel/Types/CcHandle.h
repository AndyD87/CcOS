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
 * @brief     Class CcHandle
 */
#pragma once

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

  /**
   * @brief Copy pointer from other handle to this
   * @param oToCopy: Handle to copy from
   */
  void copy(const CcHandle<TYPE>& oToCopy)
  {
    m_pPointer = oToCopy.m_pPointer;
  }

  /**
   * @brief Overwrite current pointer with new value
   * @param oToCopy: Pointer to set
   */
  void create(TYPE* oToCopy)
  {
    m_pPointer = oToCopy;
  }

  //! @return Get pointer of handle
  TYPE* ptr() const
  {
    return m_pPointer;
  }

  /**
   * @brief Cast internal pointer and generate new const handle of that type
   * @return Generated handle of template type.
   */
  template <class X>
  const CcHandle<X> cast() const
  {
    CcHandle<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer));
    return oXRet;
  }

  /**
   * @brief Cast internal pointer and generate new handle of that type
   * @return Generated handle of template type.
   */
  template <class X>
  CcHandle<X> cast()
  {
    CcHandle<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer));
    return oXRet;
  }

  /**
   * @brief Operator to get pointer like access to stored handle.
   * @return Pointer of stored handle
   */
  TYPE* operator->() const { return m_pPointer;}

  /**
   * @brief Move operator will copy pointer from another handle and delete it's value
   * @param oToMove: Handle to copy pointer from
   * @return Handle to this
   */
  CcHandle<TYPE>& operator=(CcHandle<TYPE>&& oToMove)
  { if (this != &oToMove) { m_pPointer = oToMove.m_pPointer; oToMove.m_pPointer = nullptr;} return *this; }

  //! @copydoc copy()
  //! @return Handle to this
  CcHandle<TYPE>& operator=(const CcHandle<TYPE>& oToCopy)
  { copy(oToCopy); return *this;}

  //! @copydoc copy()
  //! @return Handle to this
  CcHandle<TYPE>& operator=(TYPE* oToCopy)
  { copy(oToCopy); return *this;}

  /**
   * @brief Compare pointer of this handle with pointer of another handle
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcHandle<TYPE>& oToCompare) const
  { return static_cast<void*>(m_pPointer) == static_cast<void*>(oToCompare.m_pPointer); }

  /**
   * @brief Compare pointer of this handle with specified pointer
   * @param pToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const TYPE* pToCompare) const
  { return static_cast<const void*>(m_pPointer) == static_cast<const void*>(pToCompare); }

  /**
   * @brief Compare pointer of this handle with pointer of another handle
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcHandle<TYPE>& oToCompare) const
  { return static_cast<void*>(m_pPointer) != static_cast<void*>(oToCompare.m_pPointer); }

  /**
   * @brief Compare pointer of this handle with specified pointer
   * @param pToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const TYPE* pToCompare) const
  { return static_cast<const void*>(m_pPointer) != static_cast<const void*>(pToCompare); }

  /**
   * @brief Overwrite internal pointer to new value
   * @param pToSet: New value to set.
   */
  void setPointer(TYPE* pToSet)
  { m_pPointer = pToSet; }

  //! @return True if handle is not nullptr
  bool isValid() const
  { return static_cast<void*>(m_pPointer) != nullptr; }

  //! @return Pointer to handle
  inline operator TYPE*() const
  { return m_pPointer; }
  //! @return Pointer to handle
  inline operator const TYPE*() const
  { return m_pPointer; }
  //! @return Pointer casted to void*
  inline void* getVoidPtr() const
  { return static_cast<void*>(m_pPointer); }
private:
  TYPE* m_pPointer   = nullptr;
};
