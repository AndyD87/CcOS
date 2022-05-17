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
 * @brief     Class CcSharedPointer
 */
#pragma once

#include "CcBase.h"
#include "CcHandle.h"

/**
 * @brief Smart Pointer wich will auto-delete if all instances are destroyed
 */
template <typename TYPE>
class CcSharedPointer
{
public:
  /**
   * @brief Constructor
   */
  CcSharedPointer() = default;
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcSharedPointer)

  /**
   * @brief Constructor
   */
  CcSharedPointer(TYPE* oToCopy)
  {
    if (oToCopy != nullptr)
    {
      create(oToCopy);
    }
  }

  /**
   * @brief Destructor
   */
  ~CcSharedPointer()
  {
    deleteCurrent();
  }

  /**
   * @brief Decrease reference and delete pointer if 0.
   */
  inline void clear()
  {
    deleteCurrent();
  }

  /**
   * @brief Copy another pointer to this
   * @param oToCopy: Pointer to copy
   */
  void copy(const CcSharedPointer<TYPE>& oToCopy)
  {
    deleteCurrent();
    if (oToCopy.m_pPointer != nullptr &&
        oToCopy.m_pCounter != nullptr)
    {
      m_pPointer = oToCopy.m_pPointer;
      m_pCounter = oToCopy.m_pCounter;
      (*m_pCounter)++;
    }
  }

  /**
   * @brief Create new share pointer. clear() current
   * @param oToCopy: Pointer to set
   */
  void create(TYPE* oToCopy)
  {
    deleteCurrent();
    if(oToCopy != nullptr)
    {
      m_pPointer = oToCopy;
      CCNEW(m_pCounter, uint16, 1);
    }
  }

  //! @copydoc CcSharedPointer::clear()
  void deleteCurrent()
  {
    if (m_pCounter != nullptr)
    {
      (*m_pCounter)--;
      if ((*m_pCounter) == 0)
      {
        CCDELETE(m_pPointer);
        CCDELETE(m_pCounter);
      }
    }
    m_pPointer = nullptr;
    m_pCounter = nullptr;
  }

  /**
   * @brief Check if pointer is not nullptr
   * @return True if pionter is valid
   */
  bool isValid() const
  {
    return m_pPointer != nullptr;
  }

  //! @return Get containing pointer
  inline TYPE* ptr()
  {
    return m_pPointer;
  }

  //! @return Get containing pointer
  inline const TYPE* getPtr() const
  {
    return m_pPointer;
  }

  //! @return Create handle from pointer
  inline CcHandle<TYPE> handle()
  { return CcHandle<TYPE>(m_pPointer); }


  //! @return Create casted handle from pointer
  template <class X>
  inline CcHandle<X> handleCasted()
    { return CcHandle<X>(static_cast<X*>(m_pPointer)); }

  //! @return Create casted shared pointer from this
  template <class X>
  CcSharedPointer<X> cast() const
  {
    CcSharedPointer<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer), m_pCounter);
    return oXRet;
  }

  /**
   * @brief Move pointer from another shared pointer to this
   * @param oToMove: Pointer to move
   * @return Handle to this
   */
  inline CcSharedPointer<TYPE>& operator=(CcSharedPointer<TYPE>&& oToMove) NOEXCEPT
  {
    deleteCurrent();
    m_pCounter = oToMove.m_pCounter;
    m_pPointer = oToMove.m_pPointer;
    oToMove.m_pCounter = nullptr;
    oToMove.m_pPointer = nullptr;
    return *this;
  }

  /**
   * @brief Copy pointer from another shared pointer to this
   * @param oToCopy: Pointer to copy
   * @return Handle to this
   */
  inline CcSharedPointer<TYPE>& operator=(const CcSharedPointer<TYPE>& oToCopy)
  { copy(oToCopy); return *this;}

  /**
   * @brief Assign new pointer to this.
   * @param oToCopy: Pointer to assign
   * @return Handle to this
   */
  inline CcSharedPointer<TYPE>& operator=(TYPE* oToCopy)
  { create(oToCopy); return *this;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcSharedPointer<TYPE>& oToCompare) const
    { return m_pPointer == oToCompare.m_pPointer; }

  /**
   * @brief Compare a given pointer with containing pointer
   * @param pToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator==(TYPE* pToCompare) const
    { return m_pPointer == pToCompare;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator!=(const CcSharedPointer<TYPE>& oToCompare) const
    { return m_pPointer != oToCompare.m_pPointer; }

  /**
   * @brief Compare a given pointer with containing pointer
   * @param pToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(TYPE* pToCompare) const
  { return m_pPointer != pToCompare;}

  /**
   * @brief Change pointer values by setting values directly
   * @param pToSet:     Pointer to handle with this
   * @param uiCounter:  Pointer to shared counter of pToSet
   */
  void setPointer(TYPE* pToSet, uint16* uiCounter)
  { m_pPointer = pToSet; m_pCounter = uiCounter; (*m_pCounter)++;}

  //! @return Get pointer from this to access with ->
  inline TYPE* operator->() const { return m_pPointer;}
  //! @return Get reference to object of pointer from this to access with .
  inline TYPE& operator*() const  { return *m_pPointer;}

  //! @return Get pointer from this to access with ->
  inline operator const TYPE*() const
  { return m_pPointer; }
  //! @return Get const pointer from this to access with ->
  inline operator TYPE*() const
  { return m_pPointer; }
  //! @return Get void pointer type for comparing
  inline operator void*() const
  { return static_cast<void*>(m_pPointer); }
  //! @return Get const void pointer type for comparing
  inline operator const void*() const
  { return const_cast<const void*>(static_cast<void*>(m_pPointer)); }

private:
  TYPE* m_pPointer   = nullptr;
  uint16* m_pCounter = nullptr;
};
