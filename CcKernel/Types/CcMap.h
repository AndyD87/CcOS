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
 * @brief     Class CcMap
 **/
#ifndef H_CcMap_H_
#define H_CcMap_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcList.h"
#include "CcPair.h"
#include "CcStatic.h"

/**
 * @brief Class implementation
 */
template <typename KEY, typename VALUE>
class CcMap : public CcList<CcPair<KEY, VALUE>>
{
public:
  /**
   * @brief Constructor
   */
  CcMap() = default;

  CcMap( const CcMap& oToCopy) :
    CcList<CcPair<KEY, VALUE>>(oToCopy)
  {  }

  CcMap( CcMap&& oToMove) NOEXCEPT :
    CcList<CcPair<KEY, VALUE>>(CCMOVE(oToMove))
  {  }
  /**
   * @brief Destructor
   */
  ~CcMap() = default;

  inline VALUE& operator[](const KEY& oByKey)
    { return getValue(oByKey); }
  inline const VALUE& operator[](const KEY& oByKey) const
    { return getValue(oByKey); }
  inline CcPair<KEY, VALUE>& operator[](size_t uiIndex)
    { return CcList<CcPair<KEY, VALUE>>::at(uiIndex); }
  inline const CcPair<KEY, VALUE>& operator[](size_t uiIndex) const
    { return CcList<CcPair<KEY, VALUE>>::at(uiIndex); }

  CcMap& operator=(CcMap&& oToMove) NOEXCEPT
  {
    if (this != &oToMove)
    {
      CcList<CcPair<KEY, VALUE>>::operator=(CCMOVE(oToMove));
    }
    return *this;
  }

  CcMap& operator=(const CcMap& oToCopy)
  {
    CcList<CcPair<KEY, VALUE>>::operator=(oToCopy);
    return *this;
  }

  VALUE& getValue(const KEY& oByKey)
  {
    size_t uiSize = CcList<CcPair<KEY, VALUE>>::size();
    for (size_t i = 0; i < uiSize; i++)
      if (CcList<CcPair<KEY, VALUE>>::at(i).key() == oByKey)
        return CcList<CcPair<KEY, VALUE>>::at(i).value();
    return CcStatic::getNullRef<VALUE>();
  }

  const VALUE& getValue(const KEY& oByKey) const
  {
    size_t uiSize = CcList<CcPair<KEY, VALUE>>::size();
    for (size_t i = 0; i < uiSize; i++)
      if (CcList<CcPair<KEY, VALUE>>::at(i).key() == oByKey)
        return CcList<CcPair<KEY, VALUE>>::at(i).value();
    return CcStatic::getNullRef<VALUE>();
  }

  void append(const KEY& oKey, const VALUE& oValue)
  {
    CcList<CcPair<KEY, VALUE>>::append(CcPair<KEY, VALUE>(oKey, oValue));
  }

  void add(const KEY& oKey, const VALUE& oValue)
  {
    CcList<CcPair<KEY, VALUE>>::append(CcPair<KEY, VALUE>(oKey, oValue));
  }

  bool removeKey(const KEY& rKey)
  {
    bool bFound = false;
    for(size_t i=0; i < this->size(); i++)
    {
      const CcPair<KEY, VALUE>&rPair = this->at(i);
      if(rPair.getKey() == rKey)
      {
        bFound = true;
        this->remove(i);
        i--;
      }
    }
    return bFound;
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

  bool operator==(const CcMap& oToCompare) const
  {
    bool bSuccess = false;
    if (CcMap<KEY, VALUE>::size() == oToCompare.size())
    {
      bSuccess = true;
      typename CcMap<KEY, VALUE>::const_iterator rIterator(oToCompare.begin());
      for (const CcPair<KEY, VALUE>& rNode : *this)
      {
        if (*rIterator != rNode)
        {
          bSuccess = false;
          break;
        }
        rIterator++;
      }
    }
    return bSuccess;
  }


private:
};

#endif // H_CcMap_H_
