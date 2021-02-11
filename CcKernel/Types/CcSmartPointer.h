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
 * @brief     Class CcSmartPointer
 */
#pragma once

#include "CcBase.h"
#include "CcSharedPointer.h"

/**
 * @brief Smart Pointer wich will auto-delete if all instances are destroyed
 */
template <typename TYPE>
class CcSmartPointer : public CcSharedPointer
{
public:
  /**
   * @brief Constructor
   */
  CcSmartPointer() = default;
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcSmartPointer);

  /**
   * @brief Constructor with pointer
   * @param oToCopy: Copy Pointer from
   */
  CcSmartPointer(TYPE* oToCopy) :
    CcSharedPointer(oToCopy)
  {
  }

  //! @return Create copy and casted smart pointer from this
  template <class X>
  CcSmartPointer<X> cast() const
  {
    CcSmartPointer<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer), m_pCounter);
    return oXRet;
  }

  /**
   * @brief Move content of another smart pointer to this.
   *        Update will be called.
   * @param oToMove: Pointer to move data from
   * @return Handle to this
   */
  inline CcSmartPointer<TYPE>operator=(CcSmartPointer<TYPE>&& oToMove) NOEXCEPT
  { CcSmartPointer<TYPE>::operator=(CCMOVE(oToMove));  callUpdate();}

  /**
   * @brief Copy content of another smart pointer to this.
   *        Update will be called.
   * @param oToCopy: Pointer to copy
   * @return Handle to this
   */
  inline CcSmartPointer<TYPE>operator=(const CcSmartPointer<TYPE>& oToCopy)
  { CcSmartPointer<TYPE>operator=(oToCopy); callUpdate(); return *this;}

  /**
   * @brief Assign new pointer to this
   *        Update will be called.
   * @param oToCopy: Pointer to set
   * @return Handle to this
   */
  inline CcSmartPointer<TYPE>operator=(TYPE* oToCopy)
  { CcSmartPointer<TYPE>operator=(CCMOVE(oToCopy)); callUpdate(); return *this;}

  /**
   * @brief Compare if two smart pointers have the same pointer
   * @param oToCompare: Pointer to compare with
   * @return True if both are same
   */
  inline bool operator==(const CcSmartPointer<TYPE>& oToCompare) const
  { return CcSmartPointer<TYPE>operator==(oToCompare); }

  /**
   * @brief Compare if a pointer is the same as this member pointer
   * @param pToCompare: Pointer to compare with
   * @return True if both are same
   */
  inline bool operator==(TYPE* pToCompare) const
  { return CcSmartPointer<TYPE>operator==(pToCompare); }

  /**
   * @brief Compare if two smart pointers have not the same pointer
   * @param oToCompare: Pointer to compare with
   * @return True if both are not same
   */
  inline bool operator!=(const CcSmartPointer<TYPE>& oToCompare) const
  { return CcSmartPointer<TYPE>operator!=(oToCompare); }

  /**
   * @brief Compare if a pointer is not the same as this member pointer
   * @param pToCompare: Pointer to compare with
   * @return True if both are not same
   */
  inline bool operator!=(TYPE* pToCompare) const
  { return CcSmartPointer<TYPE>operator!=(pToCompare); }

  /**
   * @brief Register to get notification if pointer has changed
   * @param oEvent: Target event to call
   */
  void registerOnUpdate(const CcEvent& oEvent)
  { m_oUpdateHandler.append(oEvent); }

  /**
   * @brief Remove object from register
   * @param pObject: Target object to remove
   */
  void deregisterOnUpdate(CcObject* pObject)
  { m_oUpdateHandler.removeObject(pObject); }
  /**
   * @brief Call all registered objects with this as paramter
   */
  void callUpdate() NOEXCEPT
  { m_oUpdateHandler.call(this); }

private:
  CcEventHandler m_oUpdateHandler;
};
