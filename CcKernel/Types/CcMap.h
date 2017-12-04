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
 * @subpage   CcMap
 *
 * @page      CcMap
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMap
 **/
#ifndef CcMap_H_
#define CcMap_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcList.h"
#include "CcPair.h"

/**
 * @brief Class impelmentation
 */
template <typename KEY, typename VALUE>
class CcMap : public CcList<CcPair<KEY, VALUE>>
{
public:
  /**
   * @brief Constructor
   */
  CcMap( void )
  {}

  CcMap( const CcMap& oToCopy) :
    CcList<CcPair<KEY, VALUE>>(oToCopy)
  {  }

  CcMap( CcMap&& oToMove) :
    CcList<CcPair<KEY, VALUE>>(std::move(oToMove))
  {  }
  /**
   * @brief Destructor
   */
  ~CcMap(void) {}

  inline VALUE& operator[](const KEY& oByKey)
    { return getValue(oByKey); }

  CcMap& operator=(const CcMap& oToCopy)
  {
    CcList<CcPair<KEY, VALUE>>::operator=(oToCopy);
    return *this;
  }

  CcMap& operator=(CcMap&& oToMove)
  {
    if (this != &oToMove)
    {
      CcList<CcPair<KEY, VALUE>>::operator=(std::move(oToMove));
    }
    return *this;
  }

  VALUE& getValue(const KEY& oByKey)
  {
    size_t uiSize = CcList<CcPair<KEY, VALUE>>::size();
    for (size_t i = 0; i < uiSize; i++)
      if (CcList<CcPair<KEY, VALUE>>::at(i).key() == oByKey)
        return CcList<CcPair<KEY, VALUE>>::at(i).value();
    return *(VALUE*)(NULL);
  }

  CcList<VALUE> getValues(const KEY& oByKey) const
  {
    size_t uiSize = CcList<CcPair<KEY, VALUE>>::size();
    CcList<VALUE> m_oReturnList;
    for (size_t i = 0; i < uiSize; i++)
      if (CcList<CcPair<KEY, VALUE>>::at(i).key() == oByKey)
        m_oReturnList.add(CcList<CcPair<KEY, VALUE>>::at(i).value());
    return m_oReturnList;
  }

  void append(const KEY& oKey, const VALUE& oValue)
  {
    CcList<CcPair<KEY, VALUE>>::append(CcPair<KEY, VALUE>(oKey, oValue));
  }

  void add(const KEY& oKey, const VALUE& oValue)
  {
    CcList<CcPair<KEY, VALUE>>::append(CcPair<KEY, VALUE>(oKey, oValue));
  }

  void removeKey(const KEY& rKey)
  {
    for(size_t i=0; i < this->size(); i++)
    {
      const CcPair<KEY, VALUE>&rPair = this->at(i);
      if(rPair.getKey() == rKey)
      {
        this->remove(i);
        i--;
      }
    }
  }

  bool containsKey(const KEY& rKey)
  {
    for(const CcPair<KEY, VALUE>&rPair : *this)
    {
      if(rPair.getKey() == rKey)
        return true;
    }
    return false;
  }

private:
};

#endif /* CcMap_H_ */
