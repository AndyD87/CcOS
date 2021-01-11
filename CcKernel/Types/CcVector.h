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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVector
 */
#ifndef H_CcVector_H_
#define H_CcVector_H_

#include "CcBase.h"

/**
 * @brief Vector class with own implementation
 */
template <typename TYPE>
class CcVector
{
public:
  /**
   * @brief Iterator object to walk through objects wihin thin vector
   */
  class iterator
  {
  public:
    /**
     * @brief Initialize default iterator with nullptr
     */
    inline iterator() : m_pItem(nullptr)
    {
    }

    /**
     * @brief Initialize iterator with pointer to an array item
     * @param pStart: Pointer to item to initialize with
     */
    inline iterator(TYPE* pStart) : m_pItem(pStart)
    {
    }

    /**
     * @brief Create iterator to next element in list.
     * @return Generated iterator.
     */
    inline iterator next()
    {
      return m_pItem + 1;
    }

    /**
     * @brief Create iterator to previously element in list.
     * @return Generated iterator.
     */
    inline iterator prv()
    {
      return m_pItem - 1;
    }

    /**
     * @brief Referenced access to item on iterator
     * @return
     */
    TYPE& operator*() const
    {
      return *m_pItem;
    }

    /**
     * @brief Pointer access to item in iterator
     * @return
     */
    TYPE* operator->() const
    {
      return m_pItem;
    }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline iterator operator+(size_t uiDistance) const
    {
      return m_pItem + uiDistance;
    }

    /**
     * @brief Update this iterator by 1
     * @return Reference to this
     */
    inline iterator& operator++()
    {
      m_pItem++;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator++(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) m_pItem++; }
      return *this;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator+=(size_t uiDistance)
    {
      m_pItem += uiDistance;
      return *this;
    }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline iterator operator-(size_t uiDistance) const
    {
      return m_pItem - uiDistance;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator-=(size_t uiDistance)
    {
      m_pItem -= uiDistance;
      return *this;
    }

    /**
     * @brief Update this iterator by reducing vector by one
     * @return Reference to this
     */
    inline iterator& operator--()
    {
      m_pItem--;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator--(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) m_pItem--; }
      return *this;
    }

    /**
     * @brief Compare if two iterators points to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is the same object
     */
    inline bool operator==(const iterator& oToCompare) const
    {
      return m_pItem == oToCompare.m_pItem;
    }

    /**
     * @brief Compare if two iterators does not point to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is not the same object
     */
    inline bool operator!=(const iterator& oToCompare) const
    {
      return m_pItem != oToCompare.m_pItem;
    }

  private:
    TYPE* m_pItem; //!< Current pointed item in vector array
  };

  /**
   * @brief Iterator object to walk through objects wihin thin vector without
   *        the option to make changes on current item in iterator.
   */
  class const_iterator
  {
  public:
    /**
     * @brief Initialize default iterator with nullptr
     */
    inline const_iterator() : m_pItem(nullptr)
    {
    }

    /**
     * @brief Initialize iterator with pointer to an array item
     * @param pStart: Pointer to item to initialize with
     */
    inline const_iterator(TYPE* pStart) : m_pItem(pStart)
    {
    }

    /**
     * @brief Create iterator to next element in list.
     * @return Generated iterator.
     */
    inline const_iterator next()
    {
      return m_pItem + 1;
    }

    /**
     * @brief Create iterator to previously element in list.
     * @return Generated iterator.
     */
    inline const_iterator prv()
    {
      return m_pItem - 1;
    }

    /**
     * @brief Referenced access to item on iterator
     * @return
     */
    TYPE& operator*() const
    {
      return *m_pItem;
    }

    /**
     * @brief Pointer access to item in iterator
     * @return
     */
    TYPE* operator->() const
    {
      return m_pItem;
    }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline const_iterator operator+(size_t uiDistance) const
    {
      return m_pItem + uiDistance;
    }

    /**
     * @brief Update this iterator by 1
     * @return Reference to this
     */
    inline const_iterator& operator++()
    {
      m_pItem++;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator++(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) m_pItem++; }
      return *this;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator+=(size_t uiDistance)
    {
      m_pItem += uiDistance;
      return *this;
    }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline const_iterator operator-(size_t uiDistance) const
    {
      return const_iterator(m_pItem - uiDistance);
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator-=(size_t uiDistance)
    {
      m_pItem -= uiDistance;
      return *this;
    }


    /**
     * @brief Update this iterator by reducing by one
     * @return Reference to this
     */
    inline const_iterator& operator--()
    {
      m_pItem--;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator--(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) m_pItem--; }
      return *this;
    }

    /**
     * @brief Compare if two iterators points to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is the same object
     */
    inline bool operator==(const const_iterator& oToCompare) const
    {
      return m_pItem == oToCompare.m_pItem;
    }

    /**
     * @brief Compare if two iterators does not point to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is not the same object
     */
    inline bool operator!=(const const_iterator& oToCompare) const
    {
      return m_pItem != oToCompare.m_pItem;
    }

  private:
    TYPE* m_pItem; //!< Current pointed item in vector array
  };

  /**
   * @brief Constructor
   */
  CcVector() = default;

  /**
   * @brief Copy-Constructor
   *        Very importand, because m_oBuffer is not allowed to copy.
   * @param oToCopy: Vector to copy from
   */
  CcVector(const CcVector &oToCopy)
  {
    operator=(oToCopy);
  }

  /**
   * @brief Move Vector content from on vector to another.
   * @param oToMove: Vector to move from
   */
  CcVector(CcVector&& oToMove) NOEXCEPT
  {
    operator=(CCMOVE(oToMove));
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
   * @param uiCount: Count of Items on Pointer
   */
  CcVector(const TYPE* items, size_t uiCount)
  {
    append(items, uiCount);
  }

  /**
   * @brief Constructor
   * @param uiCount: Count of Items to reserve in vector
   */
  CcVector(size_t uiCount)
  {
    createArray(uiCount);
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
   * @param oToAppend: Object to add
   * @return Reference to this Vector
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
   * @brief Add an array of objects at the end of list
   * @param oToAppend: Array of objects to add
   * @param uiCount:   Number of item in oToAppend to add.
   * @return Reference to this Vector
   */
  CcVector<TYPE>& append(const CcVector<TYPE>& oToAppend, size_t uiCount)
  {
    if (oToAppend.m_pArray != nullptr)
    {
      append(oToAppend.m_pArray, uiCount);
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   * @param oToAppend: Object to add
   * @return Reference to this Vector
   */
  CcVector<TYPE>& append(const TYPE& oToAppend)
  {
    resize(size() + 1);
    last() = oToAppend;
    return *this;
  }

  /**
   * @brief Add an array of objects at the end of list
   * @param pToAppend: Array of objects to add
   * @param uiCount: Number of object sin pToAppend
   * @return Reference to this Vector
   */
  CcVector<TYPE>& append(const TYPE* pToAppend, size_t uiCount)
  {
    size_t uiOldPosition = size();
    resize(uiOldPosition + uiCount);
    TYPE* pCurrentPos = m_pArray + uiOldPosition;
    while (uiCount > 0)
    {
      *pCurrentPos = *pToAppend;
      pCurrentPos++;
      pToAppend++;
      uiCount--;
    }
    return *this;
  }
  
  /**
   * @brief Add an Object at the end of list
   * @return Return reference to newly added item at the end.
   */
  CcVector<TYPE>& appendDefault()
  {
    resize(size() + 1);
    return *this;
  }

  /**
   * @brief Append an vector of object at the end of list
   * @param oToAppend: Objects to add
   * @return Reference to this Vector
   */
  inline CcVector<TYPE>& add(const CcVector<TYPE> &oToAppend)
  {
    return append(oToAppend);
  }

  /**
   * @brief Append an object at the end of list
   * @param oToAppend: Object to add
   * @return Reference to this Vector
   */
  inline CcVector<TYPE>& add(const TYPE& oToAppend)
  {
    return append(oToAppend);
  }

  /**
   * @brief Add an Array of Object at the end of list
   * @param oToAppend: Object to add
   * @param uiCount: Object to add
   * @return Reference to this Vector
   */
  inline CcVector<TYPE>& add(const TYPE* oToAppend, size_t uiCount)
  {
    return append(oToAppend, uiCount);
  }

  /**
   * @brief Add an Object at the begining
   * @param oToAdd: Object to add
   * @return Reference to this Vector
   */
  CcVector<TYPE>& prepend(const TYPE& oToAdd)
  {
    insert(0, oToAdd);
    return *this;
  }

  /**
   * @brief Get the number of items containing in list.
   * @return Number of Items
   */
  size_t size() const
  {
    return m_uiSize;
  }

  /**
   * @brief Get the Object stored at requested position
   * @param uiPos: position of requested Object, must be lower than size()
   * @return requested Object
   */
  TYPE& at(size_t uiPos) const
  {
    return m_pArray[uiPos];
  }

  /**
   * @brief Get last Item in List
   * @return The last item in list. It must be save that at least one item is inserted bevor.
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
   * @return Reference to this Vector
   */
  inline CcVector<TYPE>& remove(size_t uiPos)
  {
    return remove(uiPos, 1);
  }

  /**
   * @brief Remove an range of objects from list.
   * @param uiPos: Position of first item to remove
   * @param uiLen: Number of items to remove
   * @return Reference to this Vector
   */
  CcVector<TYPE>& remove(size_t uiPos, size_t uiLen)
  {
    TYPE* pOldArray = m_pArray;
    size_t uiOldSize = size();
    createArray(uiOldSize - uiLen);
    move(m_pArray, pOldArray, uiPos);
    move(m_pArray + uiPos, pOldArray + uiPos + uiLen, uiOldSize - (uiLen + uiPos));
    if (uiOldSize - uiLen != size())
    {
      CCDELETEARR(pOldArray);
    }
    else
      CCDELETEARR(pOldArray);
    return *this;
  }

  /**
   * @brief Delete a specific Item in List
   * @param oItem: item to delete
   * @return True if item was found and removed
   */
  bool removeItem(const TYPE& oItem)
  {
    bool bFound = false;
    for (uint32 i = 0; i < size(); i++)
    {
      if (at(i) == oItem)
        remove(i);
    }
    return bFound;
  }

  /**
   * @brief Internaly move number of objects from.
   *
   *        std::move will be executed on all objects.
   * @param uiOffsetTo:   Position to start move to
   * @param uiOffsetFrom: Position to start move from
   * @param uiLength:     Number of elements to move.
   * @return Reference to this Vector
   */
  CcVector<TYPE>& move(size_t uiOffsetTo, size_t uiOffsetFrom, size_t uiLength)
  {
    if (uiOffsetTo < uiOffsetFrom)
    {
      return move(m_pArray + uiOffsetTo, m_pArray + uiOffsetFrom, uiLength);
    }
    else
    {
      return moveReverse(m_pArray + uiOffsetTo, m_pArray + uiOffsetFrom, uiLength);
    }
  }

  /**
   * @brief Move an array of objects from this vector to new vector.
   * @param uiBegin:    Position of element to start cut.
   * @param uiDistance: Number of items to remove to new vector
   * @return Reference to this Vector
   */
  CcVector<TYPE>& cut(size_t uiBegin, size_t uiDistance = SIZE_MAX)
  {
    size_t uiLength = CCMIN(size() - uiBegin, uiDistance);
    TYPE* pOldArray = m_pArray;
    createArray(uiLength);
    move(m_pArray, pOldArray + uiBegin, uiLength);
    CCDELETEARR(pOldArray);
    return *this;
  }

  /**
   * @brief Insert a Item at a defined Position.
   * @param uiPos: Position to store at
   * @param oItem: Item to insert
   * @return Generated iterator of inserted item.
   */
  iterator insert(size_t uiPos, const TYPE& oItem)
  {
    TYPE* pOldArray = m_pArray;
    size_t uiOldSize = size();
    createArray(uiOldSize + 1);
    if (pOldArray)
    {
      move(m_pArray, pOldArray, uiPos);
      move(m_pArray + uiPos + 1, pOldArray + uiPos, uiOldSize - uiPos);
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

  /**
   * @brief Remove data from internal array and set this one.
   *        The vector must be the only owner of pData after this call to.
   *        pData will be deleted from the vector.
   * @param pData:    Pointer to array to overtake.
   * @param uiCount:  Number of items in pData.
   */
  void transfer(TYPE* pData, size_t uiCount)
  {
    clear();
    m_pArray = pData;
    m_uiSize = uiCount;
  }

  /**
   * @brief Get internal array and it's count from this vector and clear the vector.
   *        After this call, pData must be deleted from external.
   * @param[out] pData:    Pointer to array to overtake.
   * @param[out] uiCount:  Number of items in pData.
   */
  void extract(TYPE*& pData, size_t& uiCount)
  {
    pData = m_pArray;
    uiCount = m_uiSize;
    m_pArray = nullptr;
    m_uiSize = 0;
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator end(void) const
  {
    return iterator(m_pArray+size());
  }

  /**
   * @brief Resize vector. Remove items they are to much,
   *        or create a default one by its default consturctor.
   * @param uiNewSize: New size of vector to set.
   */
  inline void resize(size_t uiNewSize)
  {
    if (m_uiSize != uiNewSize)
    {
      TYPE* pOldArray = m_pArray;
      size_t uiOldSize = m_uiSize;
      createArray(uiNewSize);
      move(m_pArray, pOldArray, CCMIN(uiNewSize, uiOldSize));
      CCDELETEARR(pOldArray);
    }
  }

  /**
   * @brief Resize vector. Remove items they are to much,
   *        or create a default one by copy from rItem.
   * @param uiNewSize: New size of vector to set.
   * @param rItem:     Default item to set if vector increases.
   */
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
   * @brief check if item is already added to List
   * @param oItem:      Item to search for in vector.
   * @param uiBegin:    Position in vector to start search at
   * @param uiDistance: Maximum number of items to search from uiBegin.
   *                    Default SIZE_MAX, to search until end of vector.
   * @return Position found item in list or SIZE_MAX if not found.
   */
  size_t find(const TYPE& oItem, size_t uiBegin = 0, size_t uiDistance = SIZE_MAX) const
  {
    size_t i;
    size_t uiMax = size();
    if (uiDistance < size() - uiBegin)
    {
      uiMax = uiBegin + uiDistance;
    }
    for (i = uiBegin; i < uiMax; i++)
    {
      if (oItem == at(i))
        return i;
    }
    return SIZE_MAX;
  }

  /**
   * @brief Check if a list of items is already added to List.
   *        The items must be stored in the same order too.
   * @param oList:      Vector of item to search for in vector.
   * @param uiBegin:    Position in vector to start search at
   * @param uiDistance: Maximum number of items to search from uiBegin.
   *                    Default SIZE_MAX, to search until end of vector.
   * @return Position found item in list or SIZE_MAX if not found.
   */
  size_t find(CcVector<TYPE>& oList, size_t uiBegin = 0, size_t uiDistance = SIZE_MAX) const
  {
    size_t iRet = SIZE_MAX;
    size_t uiMax = size();
    if (uiDistance < size() - uiBegin)
    {
      uiMax = uiBegin + uiDistance;
    }
    bool bFound(false);
    if (uiMax >= oList.size())
    {
      size_t length = uiMax - (oList.size() - 1);
      for (size_t i = uiBegin; i < length && bFound == false; i++)
      {
        if (at(i) == oList.at(0))
        {
          size_t j = 0;
          for (; j < oList.size(); j++)
          {
            if (oList.at(j) != at(i + j))
              break;
          }
          if (j == oList.size())
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
   * @brief Check if item exists at least one time in this vector.
   * @param oItem: Object to search for
   * @return true if found.
   */
  inline bool contains(const TYPE& oItem) const
  { return find(oItem) != SIZE_MAX; }

  /**
   * @brief Get an Array of Content All or just a Part of All Values
   * @param uiPosition: Position within vector
   * @return Pointer to position in vector
   */
  TYPE *getArray(size_t uiPosition = 0) const
  {
    return m_pArray + uiPosition;
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
   * @param oItem: Item to add
   * @return New Vector
   */
  CcVector<TYPE> operator+(const TYPE& oItem)
  {
    CcVector<TYPE> newVector = *this;
    newVector.append(oItem);
    return newVector;
  }

  /**
   * @brief Append Item to Vector
   * @param oItem: Item to add
   * @return Reference to this Vector
   */
  CcVector<TYPE>& operator+=(const TYPE& oItem)
  {
    return append(oItem);
  }

  /**
   * @brief Copy Vector to new one but delete a specific item;
   * @param oItem: Item to search and remove
   * @return New Vector
   */
  CcVector<TYPE> operator-(const TYPE& oItem)
  {
    CcVector<TYPE> newVector = *this;
    newVector.removeItem(oItem);
    return newVector;
  }

  /**
   * @brief Delete Specific Item from Vector
   * @param oItem: Item to delete
   * @return Reference to this Vector
   */
  CcVector<TYPE>& operator-=(const TYPE& oItem)
  {
    removeItem(oItem);
    return *this;
  }

  /**
   * @brief Move assignment Operator
   * @param oToMove: Object to move to this
   * @return this
   */
  CcVector& operator=(CcVector&& oToMove) NOEXCEPT
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
   * @param oToCopy: Item to add
   * @return New Vector
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
   * @brief Compare all items in two vectors if they differs.
   * @param oToCompare: Vector to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator!=(const CcVector& oToCompare) const
  { return !operator==(oToCompare); }

  /**
   * @brief Append vector to vector
   * @param oToAppend: Vector to add
   * @return Reference to this Vector
   */
  CcVector<TYPE>& operator+=(const CcVector& oToAppend)
  { return append(oToAppend); }

  /**
   * @brief Generate new vector from this and add additional vector to
   *        newly generated.
   * @param oToAppend: Vector to add
   * @return Newly generated Vector
   */
  CcVector<TYPE> operator+(const CcVector& oToAppend)
  { CcVector<TYPE> oData(*this); return oData.append(oToAppend); }

private:

  CcVector<TYPE>& move(TYPE* pTarget, TYPE* pSource, size_t uiCount)
  {
    while (uiCount)
    {
      *pTarget = CCMOVE(*pSource);
      uiCount--;
      pTarget++;
      pSource++;
    }
    return *this;
  }

  CcVector<TYPE>& moveReverse(TYPE* pTarget, TYPE* pSource, size_t uiCount)
  {
    pTarget += uiCount - 1;
    pSource += uiCount - 1;
    while (uiCount)
    {
      *pTarget = CCMOVE(*pSource);
      uiCount--;
      pTarget--;
      pSource--;
    }
    return *this;
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
  TYPE* m_pArray = nullptr; //!< Operating array to work on in vector
  size_t m_uiSize = 0;      //!< Number of items available in m_pArray
};

#endif // H_CcVector_H_
