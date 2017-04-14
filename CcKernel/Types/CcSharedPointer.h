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
 * @page      CcSharedPointer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSharedPointer
 */
#ifndef CcSharedPointer_H_
#define CcSharedPointer_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
* @brief Communication Device for I2C
*/
template <typename TYPE>
class CcSharedPointer
{
public:
  /**
  * @brief Constructor
  */
  CcSharedPointer(void)
  {
  }

  /**
  * @brief Copy-Constructor
  */
  CcSharedPointer(const CcSharedPointer<TYPE>& oToCopy)
  {
    copy(oToCopy);
  }

  /**
  * @brief Constructor
  */
  CcSharedPointer(TYPE* oToCopy)
  {
    create(oToCopy);
  }

  /**
  * @brief Destructor
  */
  ~CcSharedPointer(void)
  {
    deleteCurrent();
  }

  void copy(const CcSharedPointer<TYPE>& oToCopy)
  {
    deleteCurrent();
    m_Pointer = oToCopy.m_Pointer;
    m_Counter = oToCopy.m_Counter;
    (*m_Counter)++;
  }

  void create(TYPE* oToCopy)
  {
    m_Pointer = oToCopy;
    m_Counter = new uint16; CCMONITORNEW(m_Counter);
    (*m_Counter) = 1;
  }

  void deleteCurrent()
  {
    if (m_Counter != NULL)
    {
      (*m_Counter)--;
      if ((*m_Counter) == 0)
      {
        if (m_Pointer != NULL)
        {
          CCMONITORDELETE(m_Pointer); 
          delete m_Pointer;
        }
        CCMONITORDELETE(m_Counter); 
        delete m_Counter;
      }
    }
  }

  inline TYPE* ptr()
  {
    return m_Pointer;
  }

  inline const TYPE* getPtr() const
  {
    return m_Pointer;
  }

  template <class X>
  CcSharedPointer<X> cast() const
  {
    CcSharedPointer<X> oXRet;
    oXRet.setPointer(static_cast<X*>(m_Pointer), m_Counter);
    return oXRet;
  }

  inline TYPE* operator->() const { return m_Pointer;}
  inline TYPE& operator*() const  { return *m_Pointer;}
  inline CcSharedPointer<TYPE>& operator=(const CcSharedPointer<TYPE>& oToCopy)
    { copy(oToCopy); return *this;}
  inline CcSharedPointer<TYPE>& operator=(TYPE* oToCopy)
    { copy(oToCopy); return *this;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcSharedPointer<TYPE>& oToCompare) const
    { return (void*)m_Pointer == (void*) oToCompare.m_Pointer; }

  /**
   * @brief Compare a given pointer with containing pointer
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator==(TYPE* pToCompare) const
    { return (void*)m_Pointer == (void*)pToCompare;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator!=(const CcSharedPointer<TYPE>& oToCompare) const
    { return (void*)m_Pointer != (void*)oToCompare.m_Pointer; }

  /**
   * @brief Compare a given pointer with containing pointer
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(TYPE* pToCompare) const
    { return (void*)m_Pointer != (void*)pToCompare;}
  void setPointer(TYPE* pToSet, uint16* uiCounter)
    { m_Pointer = pToSet; m_Counter = uiCounter; (*m_Counter)++;}

private:
  TYPE* m_Pointer   = NULL;
  uint16* m_Counter = NULL;
};
#endif /* CcSharedPointer_H_ */
