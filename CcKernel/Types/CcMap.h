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
 * @brief     Class CcMap
 **/
#pragma once

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

  /**
   * @brief Copy constructor
   * @param oToCopy: Object to copy from
   */
  CcMap( const CcMap& oToCopy) :
    CcList<CcPair<KEY, VALUE>>(oToCopy)
  {  }

  /**
   * @brief Move constructor
   * @param oToMove: Object to move from
   */
  CcMap( CcMap&& oToMove) NOEXCEPT :
    CcList<CcPair<KEY, VALUE>>(CCMOVE(oToMove))
  {  }
  ~CcMap() = default;

  /**
   * @brief Get value by key
   * @param oByKey: Key to query
   * @return Value of selected key
   */
  inline VALUE& operator[](const KEY& oByKey)
  { return getValue(oByKey); }

  /**
   * @brief Get value by key
   * @param oByKey: Key to query
   * @return Value of selected key
   */
  inline const VALUE& operator[](const KEY& oByKey) const
  { return getValue(oByKey); }

  /**
   * @brief Get pair by index
   * @param uiIndex: Index
   * @return Pair at selected index
   */
  inline CcPair<KEY, VALUE>& operator[](size_t uiIndex)
  { return CcList<CcPair<KEY, VALUE>>::at(uiIndex); }

  /**
   * @brief Get pair by index
   * @param uiIndex: Index
   * @return Pair at selected index
   */
  inline const CcPair<KEY, VALUE>& operator[](size_t uiIndex) const
  { return CcList<CcPair<KEY, VALUE>>::at(uiIndex); }

  /**
   * @brief Move content from another object to this
   * @param oToMove: Object to move from
   * @return Handle to this
   */
  CcMap& operator=(CcMap&& oToMove) NOEXCEPT
  {
    if (this != &oToMove)
    {
      CcList<CcPair<KEY, VALUE>>::operator=(CCMOVE(oToMove));
    }
    return *this;
  }

  /**
   * @brief Copy content from another object to this
   * @param oToCopy: Object to copy from
   * @return Handle to this
   */
  CcMap& operator=(const CcMap& oToCopy)
  {
    CcList<CcPair<KEY, VALUE>>::operator=(oToCopy);
    return *this;
  }

  /**
   * @brief Get value by key
   * @param oByKey: Key to query
   * @return Value of selected key
   */
  VALUE& getValue(const KEY& oByKey)
  {
    size_t uiSize = CcList<CcPair<KEY, VALUE>>::size();
    for (size_t i = 0; i < uiSize; i++)
      if (CcList<CcPair<KEY, VALUE>>::at(i).getKey() == oByKey)
        return CcList<CcPair<KEY, VALUE>>::at(i).getValue();
    return CcStatic::getNullRef<VALUE>();
  }

  /**
   * @brief Get value by key
   * @param oByKey: Key to query
   * @return Value of selected key
   */
  const VALUE& getValue(const KEY& oByKey) const
  {
    size_t uiSize = CcList<CcPair<KEY, VALUE>>::size();
    for (size_t i = 0; i < uiSize; i++)
      if (CcList<CcPair<KEY, VALUE>>::at(i).getKey() == oByKey)
        return CcList<CcPair<KEY, VALUE>>::at(i).getValue();
    return CcStatic::getNullRef<VALUE>();
  }

  /**
   * @brief Append key and value as pair to map
   * @param oKey:   Key to add
   * @param oValue: Value to add
   */
  void append(const KEY& oKey, const VALUE& oValue)
  {
    CcList<CcPair<KEY, VALUE>>::append(CcPair<KEY, VALUE>(oKey, oValue));
  }

  /**
   * @brief Append key and value as pair to map
   * @param oKey:   Key to add
   * @param oValue: Value to add
   */
  void add(const KEY& oKey, const VALUE& oValue)
  {
    CcList<CcPair<KEY, VALUE>>::append(CcPair<KEY, VALUE>(oKey, oValue));
  }

  /**
   * @brief Remove Pair by it's key
   * @param rKey: Key to remove
   * @return True if key was found
   */
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

  /**
   * @brief Check if key exists in map
   * @param rKey: Key to search for
   * @return True if key was found
   */
  bool containsKey(const KEY& rKey)
  {
    for(const CcPair<KEY, VALUE>&rPair : *this)
    {
      if(rPair.getKey() == rKey)
        return true;
    }
    return false;
  }

  /**
   * @brief Compare another map with this if their contents are same
   * @param oToCompare: Map to compare with
   * @return True if contents are same
   */
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
