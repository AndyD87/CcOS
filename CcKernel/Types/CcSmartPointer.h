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
 * @subpage   CcSmartPointer
 *
 * @page      CcSmartPointer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSmartPointer
 */
#ifndef H_CcSmartPointer_H_
#define H_CcSmartPointer_H_

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
   * @brief Constructor
   */
  CcSmartPointer(TYPE* oToCopy) :
    CcSharedPointer(oToCopy)
  {
  }

  template <class X>
  CcSmartPointer<X> cast() const
  {
    CcSmartPointer<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_pPointer), m_pCounter);
    return oXRet;
  }

  inline CcSmartPointer<TYPE>operator=(CcSmartPointer<TYPE>&& oToMove) NOEXCEPT
  { CcSmartPointer<TYPE>::operator=(std::move(oToMove));  callUpdate();}
  inline CcSmartPointer<TYPE>operator=(const CcSmartPointer<TYPE>& oToCopy)
  { CcSmartPointer<TYPE>operator=(oToCopy); callUpdate(); return *this;}
  inline CcSmartPointer<TYPE>operator=(TYPE* oToCopy)
  { CcSmartPointer<TYPE>operator=(std::move(oToCopy)); callUpdate(); return *this;}

  inline bool operator==(const CcSmartPointer<TYPE>& oToCompare) const
  { return CcSmartPointer<TYPE>operator==(oToCompare); }
  inline bool operator==(TYPE* pToCompare) const
  { return CcSmartPointer<TYPE>operator==(pToCompare); }
  inline bool operator!=(const CcSmartPointer<TYPE>& oToCompare) const
  { return CcSmartPointer<TYPE>operator!=(oToCompare); }
  inline bool operator!=(TYPE* pToCompare) const
  { return CcSmartPointer<TYPE>operator!=(pToCompare); }

  void registerOnUpdate(const CcEvent& oEvent)
  { m_oUpdateHandler.append(oEvent); }
  void deregisterOnUpdate(CcObject* pObject)
  { m_oUpdateHandler.removeObject(pObject); }
  void callUpdate() NOEXCEPT
  { m_oUpdateHandler.call(this); }

private:
  CcEventHandler m_oUpdateHandler;
};
#endif // H_CcSmartPointer_H_
