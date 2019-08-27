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
 * @subpage   CcVector
 *
 * @page      CcVector CcVector
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVector
 */
#ifndef H_CCVECTOR_H_
#define H_CCVECTOR_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Vector class with own implementation
 */
template <typename TYPE>
class CcVector
{
public:

  class iterator
  {
    typedef TYPE  value_type;
    typedef TYPE* pointer;
    typedef TYPE& reference;
  public:
    inline iterator() : m_pItem(nullptr)
    {
    }
    inline iterator(TYPE* pStart) : m_pItem(pStart)
    {
    }
    inline iterator next()
    {
      return iterator(m_pItem + 1);
    }

    inline iterator prv()
    {
      return iterator(m_pItem - 1);
    }

    TYPE& operator*() const
    {
      return *m_pItem;
    }

    TYPE* operator->() const
    {
      return m_pItem;
    }

    inline iterator operator+(size_t uiDistance) const
    {
      return m_pItem + uiDistance;
    }

    inline iterator& operator++()
    {
      m_pItem++;
      return *this;
    }

    inline iterator operator++(int i)
    {
      do { m_pItem++; } while (i-- > 0);
      return *this;
    }

    inline iterator& operator+=(size_t uiDistance)
    {
      m_pItem += uiDistance;
      return *this;
    }

    inline iterator operator-(size_t uiDistance) const
    {
      return iterator(m_pItem-uiDistance);
    }

    inline iterator& operator-=(size_t uiDistance)
    {
      m_pItem -= uiDistance;
      return *this;
    }

    inline iterator& operator--()
    {
      m_pItem--;
      return *this;
    }

    inline iterator operator--(int i)
    {
      do { m_pItem++; } while (i-- > 0);
      return *this;
    }

    inline bool operator==(const iterator& oToCompare) const
    {
      return m_pItem == oToCompare.m_pItem;
    }

    inline bool operator!=(const iterator& oToCompare) const
    {
      return m_pItem != oToCompare.m_pItem;
    }

  private:
    TYPE* m_pItem;
  };

  class const_iterator
  {
    typedef TYPE  value_type;
    typedef const TYPE* pointer;
    typedef const TYPE& reference;
  public:
    inline const_iterator() : m_pItem(nullptr)
    {
    }
    inline const_iterator(TYPE* pStart) : m_pItem(pStart)
    {
    }
    inline const_iterator next()
    {
      return const_iterator(m_pItem + 1);
    }

    inline const_iterator prv()
    {
      return const_iterator(m_pItem - 1);
    }

    TYPE& operator*() const
    {
      return *m_pItem;
    }
    TYPE* operator->() const
    {
      return m_pItem;
    }

    inline const_iterator operator+(size_t uiDistance) const
    {
      return m_pItem + uiDistance;
    }

    inline const_iterator& operator++()
    {
      m_pItem++;
      return *this;
    }

    inline const_iterator operator++(int i)
    {
      do { m_pItem++; } while (i-- > 0);
      return *this;
    }

    inline const_iterator& operator+=(size_t uiDistance)
    {
      m_pItem += uiDistance;
      return *this;
    }

    inline const_iterator operator-(size_t uiDistance) const
    {
      return const_iterator(m_pItem - uiDistance);
    }

    inline const_iterator& operator-=(size_t uiDistance)
    {
      m_pItem -= uiDistance;
      return *this;
    }

    inline const_iterator& operator--()
    {
      m_pItem--;
      return *this;
    }

    inline const_iterator operator--(int i)
    {
      do { m_pItem--; } while (i-- > 0);
      return *this;
    }

    inline bool operator==(const const_iterator& oToCompare) const
    {
      return m_pItem == oToCompare.m_pItem;
    }

    inline bool operator!=(const const_iterator& oToCompare) const
    {
      return m_pItem != oToCompare.m_pItem;
    }

  private:
    TYPE* m_pItem;
  };

  /**
   * @brief Constructor
   */
  CcVector() = default;

  /**
   * @brief Copy-Constructor
   *        Very importand, becaus m_Buffer is not allowed to copy.
   */
  CcVector(const CcVector &oToCopy)
  {
    operator=(oToCopy);
  }

  /**
   * @brief MoveConstructor
   */
  CcVector(CcVector&& oToMove) noexcept
  {
    operator=(std::move(oToMove));
  }

  /**
   * @brief Constructor
   * @param item: Item to add on load
   */
  CcVector(const TYPE& item)
  {
    append(item);
  }

  /**
   * @brief Constructor
   * @param items: Pointer to Items to add on load
   * @param number: Count of Items on Pointer
   */
  CcVector(const TYPE* items, size_t count)
  {
    append(items, count);
  }

  /**
   * @brief Constructor
   * @param number: Count of Items to reserve in vector
   */
  CcVector(size_t count)
  {
    createArray(count);
  }

  /**
   * @brief Destructor
   */
  ~CcVector()
  {
    clear();
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param oToAppend: Object to add
   */
  CcVector<TYPE>& append(const CcVector<TYPE> &oToAppend)
  {
    if (oToAppend.m_pArray != nullptr)
    {
      append(oToAppend.m_pArray, oToAppend.m_uiSize);
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param oToAppend: Object to add
   */
  CcVector<TYPE>& append(const CcVector<TYPE>& oToAppend, size_t count)
  {
    if (oToAppend.m_pArray != nullptr)
    {
      append(oToAppend.m_pArray, count);
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param oToAppend: Object to add
   */
  CcVector<TYPE>& append(const TYPE& oToAppend)
  {
    resize(size() + 1);
    last() = oToAppend;
    return *this;
  }

  CcVector<TYPE>& append(const TYPE* pToAppend, size_t count)
  {
    size_t uiOldPosition = size();
    resize(uiOldPosition + count);
    TYPE* pCurrentPos = m_pArray + uiOldPosition;
    while (count > 0)
    {
      *pCurrentPos = *pToAppend;
      pCurrentPos++;
      pToAppend++;
      count--;
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param oToAppend: Object to add
   */
  CcVector<TYPE>& add(const CcVector<TYPE> &oToAppend)
  {
    return append(oToAppend);
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param oToAppend: Object to add
   */
  CcVector<TYPE>& add(const TYPE& oToAppend)
  {
    return append(oToAppend);
  }

  /**
   * @brief Add an Array of Object at the end of list
   * @param oToAppend: Object to add
   * @param count: Object to add
   */
  CcVector<TYPE>& add(const TYPE* oToAppend, size_t count)
  {
    return append(oToAppend, count);
  }

  /**
   * @brief Add an Object at the end of vector
   *
   * @param oToAppend: Object to add
   */
  CcVector<TYPE>& prepend(const TYPE& oToAppend)
  {
    insert(0, oToAppend);
    return *this;
  }

  /**
   * @brief Get the number of items containing in list.
   *
   * @return Number of Items
   */
  size_t size() const
  {
    return m_uiSize;
  }

  /**
   * @brief Get the Object stored at requested position
   *
   * @param uiPos: position of requested Object, must be lower than size()
   * @return requested Object
   */
  TYPE& at(size_t uiPos) const
  {
    return m_pArray[uiPos];
  }

  /**
   * @brief Get last Item in List
   * @param return the last item in list. It must be save that at least one item is inserted bevor.
   */
  TYPE& last()
  {
    return m_pArray[m_uiSize-1];
  }

  /**
   * @brief Deletes all entries in list.
   */
  void clear()
  {
    CCDELETEARR(m_pArray);
    m_uiSize = 0;
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  inline CcVector<TYPE>& remove(size_t uiPos)
  {
    return remove(uiPos, 1);
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcVector<TYPE>& remove(size_t uiPos, size_t uiLen)
  {
    TYPE* pOldArray = m_pArray;
    size_t uiOldSize = size();
    createArray(uiOldSize - uiLen);
    moveItems(m_pArray, pOldArray, uiPos);
    moveItems(m_pArray + uiPos, pOldArray + uiPos + uiLen, uiOldSize - (uiLen + uiPos));
    CCDELETEARR(pOldArray);
    return *this;
  }

  /**
   * @brief Delete a specific Item in List
   * @param item: item to delete
   */
  CcVector<TYPE>& removeItem(const TYPE& item)
  {
    for (uint32 i = 0; i < size(); i++)
    {
      if (at(i) == item)
        remove(i);
    }
    return *this;
  }

  /**
   * @brief Insert a Item at a defined Position.
   * @param uiPos: Position to store at
   * @param item: Item to store
   */
  iterator insert(size_t uiPos, const TYPE& oItem)
  {
    TYPE* pOldArray = m_pArray;
    size_t uiOldSize = size();
    createArray(uiOldSize + 1);
    if (pOldArray)
    {
      moveItems(m_pArray, pOldArray, uiPos);
      moveItems(m_pArray + uiPos + 1, pOldArray + uiPos, uiOldSize - uiPos);
      CCDELETEARR(pOldArray);
    }
    m_pArray[uiPos] = oItem;
    return iterator(m_pArray + uiPos);
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator begin(void) const
  {
    return iterator(m_pArray);
  }

  void transfer(TYPE* pData, size_t uiCount)
  {
    clear();
    m_pArray = pData;
    m_uiSize = uiCount;
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator end(void) const
  {
    return iterator(m_pArray+size());
  }

  inline void resize(size_t uiNewSize)
  {
    if (m_uiSize != uiNewSize)
    {
      TYPE* pOldArray = m_pArray;
      size_t uiOldSize = m_uiSize;
      createArray(uiNewSize);
      moveItems(m_pArray, pOldArray, CCMIN(uiNewSize, uiOldSize));
      CCDELETEARR(pOldArray);
    }
  }

  inline void resize(size_t uiNewSize, const TYPE& rItem)
  {
    size_t uiOldSize = m_uiSize;
    resize(uiNewSize);
    while(uiOldSize < uiNewSize)
    {
      m_pArray[uiOldSize] = rItem;
      uiOldSize++;
    }
  }

  /**
   * @brief check if item is allready added to List
   * @return true if list contains item, otherwise false
   */
  size_t find(const TYPE& item) const
  {
    size_t i;
    for (i = 0; i < size(); i++)
    {
      if (item == at(i))
        return i;
    }
    return SIZE_MAX;
  }

  inline bool contains(const TYPE& item) const
    { return find(item) != SIZE_MAX; }

  /**
   * @brief check if item is allready added to List
   * @return true if list contains item, otherwise false
   */
  size_t find(CcVector<TYPE>& list) const
  {
    size_t iRet = SIZE_MAX;
    bool bFound(false);
    if (size() >= list.size())
    {
      size_t length = size() - (list.size() - 1);
      for (size_t i = 0; i < length && bFound == false; i++)
      {
        if (at(i) == list.at(0))
        {
          size_t j = 0;
          for (; j < list.size(); j++)
          {
            if (list.at(j) != at(i + j))
              break;
          }
          if (j == list.size())
          {
            bFound = true;
            iRet = i;
          }
        }
      }
    }
    return iRet;
  }

  /**
   * @brief Get an Array of Content All or just a Part of All Values
   * @param pos: beginning position of content to return
   * @param len: length of Content to return
   * @return Pointer to allocated Array
   */
  TYPE *getArray(size_t pos = 0) const
  {
    return m_pArray + pos;
  }

  /**
   * @brief Return value at Position
   * @param uiPos: position to return
   * @return Item at postion
   */
  inline const TYPE& operator[](size_t uiPos) const { return at(uiPos); }

  /**
   * @brief Return value at Position
   * @param uiPos: position to return
   * @return Item at postion
   */
  inline TYPE& operator[](size_t uiPos) { return at(uiPos); }

  /**
   * @brief Copy Vector to new one and append Item to it.
   * @param Item to add
   * @return new Vector
   */
  CcVector<TYPE> operator+(const TYPE& item)
  {
    CcVector<TYPE> newVector = *this;
    newVector.append(item);
    return newVector;
  }

  /**
   * @brief Append Item to Vector
   * @param Item to add
   * @return Reference to this Vector
   */
  CcVector<TYPE>& operator+=(const TYPE& item)
  {
    return append(item);
  }

  /**
   * @brief Copy Vector to new one but delete a specific item;
   * @param Item to add
   * @return new Vector
   */
  CcVector<TYPE> operator-(const TYPE& item)
  {
    CcVector<TYPE> newVector = *this;
    newVector.removeItem(item);
    return newVector;
  }

  /**
   * @brief Delete Specific Item from Vector
   * @param Item to delete
   * @return Reference to this Vector
   */
  CcVector<TYPE>& operator-=(const TYPE& item)
  {
    return removeItem(item);
  }

  /**
   * @brief Move assignment Operator
   * @param oToMove: Object to move to this
   * @return this
   */
  CcVector& operator=(CcVector&& oToMove) noexcept
  {
    if (this != &oToMove)
    {
      clear();
      m_pArray = oToMove.m_pArray;
      m_uiSize = oToMove.m_uiSize;
      oToMove.m_pArray = nullptr;
      oToMove.m_uiSize = 0;
    }
    return *this;
  }

  /**
   * @brief Copy Vector to new one but delete a specific item;
   * @param Item to add
   * @return new Vector
   */
  CcVector<TYPE>& operator=(const CcVector& oToCopy)
  {
    clear();
    return append(oToCopy);
  }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcVector& oToCompare) const
  { 
    bool bSame = true;
    if (size() == oToCompare.size())
    {
      for (size_t uiPos = 0; uiPos < size(); uiPos++)
      {
        if (m_pArray[uiPos] != oToCompare.m_pArray[uiPos])
        {
          bSame = false;
          break;
        }
      }
    }
    else
    {
      bSame = false;
    }
    return bSame;
  }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator!=(const CcVector& oToCompare) const
    { return !operator==(oToCompare); }

  CcVector<TYPE>& operator+=(const CcVector& oToAppend)
    { return append(oToAppend); }
  
  CcVector<TYPE> operator+(const CcVector& oToAppend)
    { CcVector<TYPE> oData(*this); return oData.append(oToAppend); }

private:
  inline void moveItems(TYPE* pTarget, TYPE* pSource, size_t uiCount)
  {
    while (uiCount)
    {
      *pTarget = std::move(*pSource);
      uiCount--;
      pTarget++;
      pSource++;
    }
  }

  inline void createArray(size_t uiSize)
  {
    if(uiSize > 0)
    {
      CCNEWARRAY(m_pArray, TYPE, uiSize);
    }
    else
    {
      m_pArray = nullptr;
    }
    m_uiSize = uiSize;
  }

private:
  TYPE* m_pArray = nullptr;
  size_t m_uiSize = 0;
};

#endif /* H_CCVECTOR_H_ */
