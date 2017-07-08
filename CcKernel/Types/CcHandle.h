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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHandle
 */
#ifndef CcHandle_H_
#define CcHandle_H_

#include "CcBase.h"
#include "CcKernelBase.h"

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
  CcHandle(void)
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
  ~CcHandle(void)
  {
  }

  void copy(const CcHandle<TYPE>& oToCopy)
  {
    m_Pointer = oToCopy.m_Pointer;
  }

  void create(TYPE* oToCopy)
  {
    m_Pointer = oToCopy;
  }

  TYPE* get()
  {
    return m_Pointer;
  }

  template <class X>
  CcHandle<X> cast() const
  {
    CcHandle<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_Pointer));
    return oXRet;
  }

  TYPE* operator->() const { return m_Pointer;}
  CcHandle<TYPE>& operator=(const CcHandle<TYPE>& oToCopy)
    { copy(oToCopy); return *this;}
  CcHandle<TYPE>& operator=(TYPE* oToCopy)
    { copy(oToCopy); return *this;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcHandle<TYPE>& oToCompare) const
    { return (void*)m_Pointer == (void*)oToCompare.m_Pointer; }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcHandle<TYPE>& oToCompare) const
    { return (void*)m_Pointer != (void*)oToCompare.m_Pointer; }

  void setPointer(TYPE* pToSet)
    { m_Pointer = pToSet; }

  bool isValid()
    { return m_Pointer != nullptr; }

private:
  TYPE* m_Pointer   = nullptr;
};
#endif /* CcHandle_H_ */
