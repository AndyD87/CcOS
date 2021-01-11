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
 * @subpage   CcList
 *
 * @page      CcList  CcList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcList
 */
#ifndef H_CcList_H_
#define H_CcList_H_

#include "CcBase.h"
// search for m_List

/**
 * @brief List-class with List as base.
 */
template <typename TYPE>
class CcList
{
public:
  class CItem
  {
  public:
    inline CItem(CItem* pForward, CItem* pBackward) : pForward(pForward), pBackward(pBackward) {}
    inline CItem(CItem* pForward, CItem* pBackward, const TYPE& oItem) : oItem(oItem), pForward(pForward), pBackward(pBackward) {}
    inline CItem(CItem* pForward, CItem* pBackward, TYPE&& oItem) : oItem(CCMOVE(oItem)), pForward(pForward), pBackward(pBackward) {}
    TYPE oItem;
    CItem* pForward;
    CItem* pBackward;
  };

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
    inline iterator(CItem* pStart) : m_pItem(pStart)
    {
    }

    /**
     * @brief Create iterator to next element in list.
     * @return Generated iterator.
     */
    inline iterator next()
    {
      return iterator(m_pItem->pForward);
    }

    /**
     * @brief Create iterator to previously element in list.
     * @return Generated iterator.
     */
    inline iterator prv()
    {
      return iterator(m_pItem->pBackward);
    }

    /**
     * @brief Referenced access to item on iterator
     * @return
     */
    TYPE& operator*() const { return m_pItem->oItem; }

    /**
     * @brief Pointer access to item in iterator
     * @return
     */
    TYPE* operator->() const { return &m_pItem->oItem; }

    inline iterator operator+(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pForward;
      }
      return pReturn;
    }

    /**
     * @brief Update this iterator by 1
     * @return Reference to this
     */
    inline iterator& operator++()
    {
      m_pItem = m_pItem->pForward;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator++(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) operator++(); }
      return *this;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator+=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pForward;
      }
      return *this;
    }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline iterator operator-(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pBackward;
      }
      return pReturn;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator-=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pBackward;
      }
      return *this;
    }

    /**
     * @brief Update this iterator by reducing vector by one
     * @return Reference to this
     */
    inline iterator& operator--()
    {
      m_pItem = m_pItem->pBackward;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline iterator& operator--(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) operator--(); }
      return *this;
    }

    /**
     * @brief Compare if two iterators points to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is the same object
     */
    inline bool operator==(const iterator& oToCompare) const
    {
      return oToCompare.m_pItem == m_pItem;
    }

    /**
     * @brief Compare if two iterators does not point to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is not the same object
     */
    inline bool operator!=(const iterator& oToCompare) const
    {
      return oToCompare.m_pItem != m_pItem;
    }

    CItem* m_pItem;
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
    inline const_iterator(CItem* pStart) : m_pItem(pStart)
    {
    }

    /**
     * @brief Create iterator to next element in list.
     * @return Generated iterator.
     */
    inline const_iterator next()
    {
      return m_pItem->pForward;
    }

    /**
     * @brief Create iterator to previously element in list.
     * @return Generated iterator.
     */
    inline const_iterator prv()
    {
      return m_pItem->pBackward;
    }

    /**
     * @brief Referenced access to item on iterator
     * @return
     */
    TYPE& operator*() const
    {
      return m_pItem->oItem;
    }

    /**
     * @brief Pointer access to item in iterator
     * @return
     */
    TYPE* operator->() const
    {
      return &m_pItem->oItem;
    }

    TYPE& getItem()
      {return m_pItem->oItem; }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline const_iterator operator+(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pForward;
      }
      return pReturn;
    }

    /**
     * @brief Update this iterator by 1
     * @return Reference to this
     */
    inline const_iterator& operator++()
    {
      m_pItem = m_pItem->pForward;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator++(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) operator++(); }
      return *this;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator+=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pForward;
      }
      return *this;
    }

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
    inline const_iterator operator-(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pBackward;
      }
      return pReturn;
    }

    /**
     * @brief Update this iterator by removing varuios count
     * @param uiDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator-=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pBackward;
      }
      return *this;
    }


    /**
     * @brief Update this iterator by reducing by one
     * @return Reference to this
     */
    inline const_iterator& operator--()
    {
      m_pItem = m_pItem->pBackward;
      return *this;
    }

    /**
     * @brief Update this iterator by varuios count
     * @param iDistance: Number of distance to update
     * @return Reference to this
     */
    inline const_iterator& operator--(int iDistance)
    {
      if(iDistance >= 0){ while (iDistance-- >= 0) operator--(); }
      return *this;
    }

    /**
     * @brief Compare if two iterators points to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is the same object
     */
    inline bool operator==(const const_iterator& oToCompare) const
    {
      if (oToCompare.m_pItem == nullptr && m_pItem == nullptr)
        return true;
      else if (oToCompare.m_pItem == nullptr || m_pItem == nullptr)
        return false;
      else
        return oToCompare.m_pItem == m_pItem;
    }

    /**
     * @brief Compare if two iterators does not point to same object.
     * @param oToCompare: iterator to compare to
     * @return true if it is not the same object
     */
    inline bool operator!=(const const_iterator& oToCompare) const
    {
      return !operator==(oToCompare);
    }

  private:
    CItem* m_pItem;
  };

  /**
   * @brief Constructor with null elements
   */
  CcList() = default;

  /**
   * @brief Copy-Constructor
   *        Very importand, because m_oBuffer is not allowed to copy.
   * @param oToCopy: Vector to copy from
   */
  CcList(const CcList &oToCopy)
  {
    operator=(oToCopy);
  }

  /**
   * @brief Move List content from a list to another.
   * @param oToMove: Vector to move from
   */
  CcList(CcList&& oToMove) NOEXCEPT
  {
    operator=(CCMOVE(oToMove));
  }

  /**
   * @brief Constructor
   * @param item: Item to add on load
   */
  CcList(const TYPE& item)
  {
    append(item); 
  }

  /**
   * @brief Constructor
   * @param items: Pointer to Items to add on load
   * @param uiCount: Count of Items on Pointer
   */
  CcList(const TYPE* items, size_t uiCount)
  {
    append(items, uiCount);
  }

  /**
   * @brief Constructor
   * @param uiCount: Count of Items to reserve in vector
   */
  CcList(size_t uiCount)
  {
    while(uiCount > 0)
    {
      uiCount--;
      append(TYPE());
    }
  }

  /**
   * @brief Destructor
   */
  ~CcList()
  {
    clear();
  }

  /**
   * @brief Add an Object at the end of list
   * @param oToAppend: Object to add
   * @return Reference to this Vector
   */
  CcList<TYPE>& append(const CcList<TYPE> &toAppend)
  {
    for(const TYPE& rItem : toAppend)
    {
      append(rItem);
    }
    return *this;
  }
  
  /**
   * @brief Add an array of objects at the end of list
   * @param oToAppend: Array of objects to add
   * @param uiCount:   Number of item in oToAppend to add.
   * @return Reference to this Vector
   */
  CcList<TYPE>& append(const TYPE* toAppend, size_t count)
  {
    while (count > 0)
    {
      append(*toAppend);
      toAppend++;
      count--;
    }
    return *this;
  }
  
  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   * @return Reference to this Vector
   */
  CcList<TYPE>& append(CcList<TYPE> &&toAppend)
  {
    while (toAppend.size() > 0)
    {
      append(CCMOVE(toAppend.at(0)));
      toAppend.remove(0);
    }
    return *this;
  } 

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& append(TYPE &&toAppend)
  {
    CCNEWTYPE(pItem, CItem, nullptr, m_pListEnd, CCMOVE(toAppend));
    if (m_pListEnd != nullptr)
    {
      m_pListEnd->pForward = pItem;
      m_pListEnd = pItem;
    }
    else
    {
      m_pListEnd   = pItem;
      m_pListBegin = pItem;
    }
    m_uiSize++;
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   * @return Reference to this Vector
   */
  CcList<TYPE>& append(const TYPE& toAppend)
  {
    CCNEWTYPE(pItem, CItem, nullptr, m_pListEnd, toAppend);
    if (m_pListEnd != nullptr)
    {
      m_pListEnd->pForward = pItem;
      m_pListEnd = pItem;
    }
    else
    {
      m_pListEnd = pItem;
      m_pListBegin = pItem;
    }
    m_uiSize++;
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& append(iterator& toAppend)
  {
    toAppend.m_pItem->pForward = nullptr;
    toAppend.m_pItem->pBackward = m_pListEnd;
    if (m_pListEnd != nullptr)
    {
      m_pListEnd->pForward = toAppend.m_pItem;
      m_pListEnd = toAppend.m_pItem;
    }
    else
    {
      m_pListEnd = toAppend.m_pItem;
      m_pListBegin = toAppend.m_pItem;
    }
    m_uiSize++;
    return *this;
  }

  /**
   * @brief Add an empty Object at the end of list
   * @return Return reference to newly added item at the end.
   */
  CcList<TYPE>& appendDefault()
  {
    CCNEWTYPE(pItem, CItem, nullptr, m_pListEnd);
    if (m_pListEnd != nullptr)
    {
      m_pListEnd->pForward = pItem;
      m_pListEnd = pItem;
    }
    else
    {
      m_pListEnd = pItem;
      m_pListBegin = pItem;
    }
    m_uiSize++;
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  inline CcList<TYPE>& add(const CcList<TYPE> &toAppend)
  { return append(toAppend); }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  inline CcList<TYPE>& add(const TYPE& toAppend)
  { return append(toAppend); }

  /**
   * @brief Add an Array of Object at the end of list
   * @param toAppend: Object to add
   * @param count: Object to add
   */
  inline CcList<TYPE>& add(const TYPE* toAppend, size_t count)
  { return append(toAppend, count); }

  iterator dequeue(size_t uiPos)
  {
    CItem* pItem = prvtItemAt(uiPos);
    if(pItem != nullptr)
    {
      prvtRemoveItem(pItem);
      m_uiSize--;
      pItem->pForward = nullptr;
      pItem->pBackward = nullptr;
    }
    return iterator(pItem);
  }

  inline iterator dequeueFirst()
  { return dequeue(0); }

  inline iterator dequeueLast()
  { return dequeue(size()-1); }

  /**
   * @brief Add an Object at the beginning
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& prepend(const TYPE &toAppend)
  {
    insert(0, toAppend);
    return *this;
  }

  /**
   * @brief Add an Object at the beginning
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& prepend(TYPE&& toAppend)
  {
    insert(0, toAppend);
    return *this;
  }

  /**
   * @brief Add an Object at the beginning
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& prepend(iterator& oItem)
  {
    insert(0, oItem);
    return *this;
  }

  iterator dequeue(iterator& oItem)
  {
    CItem* pItem = oItem.m_pItem;
    iterator oReturn = (oItem++);
    prvtRemoveItem(pItem);
    m_uiSize--;
    pItem->pForward = nullptr;
    pItem->pBackward = nullptr;
    return oReturn;
  }

  /**
   * @brief Get the number of items containing in list.
   *
   * @return Number of Items
   */
  size_t size(void) const
    { return m_uiSize; }

  /**
   * @brief Get the Object stored at requested position
   *
   * @param uiPos: position of requested Object, must be lower than size()
   * @return requested Object
   */
  TYPE& at(size_t uiPos) const
  {
    return prvtItemAt(uiPos)->oItem;
  }

  /**
   * @brief Get last Item in List
   * @param return the last item in list. It must be save that at least one item is inserted bevor.
   */
  TYPE& last(void)
  { return m_pListEnd->oItem; }

  /**
   * @brief Deletes all entries in list.
   */
  void clear(void)
  {
    CItem* pTemp = nullptr;
    while (m_uiSize > 0)
    {
      pTemp = m_pListBegin->pForward;
      CCDELETE(m_pListBegin);
      m_pListBegin = pTemp;
      m_uiSize--;
    }
    m_pListBegin = nullptr;
    m_pListEnd = nullptr;
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcList<TYPE>& remove(size_t uiPos)
  {
    CItem* pItemToDelete = prvtItemAt(uiPos);
    prvtRemoveItem(pItemToDelete);
    CCDELETE(pItemToDelete);
    m_uiSize--;
    return *this;
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcList<TYPE>& remove(size_t uiPos, size_t len)
  {
    m_uiSize -= len;
    CItem* pItemToDelete = prvtItemAt(uiPos);
    CItem* pTemp;
    while (len)
    {
      pTemp = pItemToDelete->pForward;
      prvtRemoveItem(pItemToDelete);
      m_uiSize--;
      CCDELETE(pItemToDelete);
      pItemToDelete = pTemp;
      len--;
    }
    return *this;
  }

  /**
   * @brief Delete all items in List wich will equal the given item
   * @param item: item to delete
   * @return true if item found and removed, otherwise false;
   */
  bool removeItem(const TYPE& item)
  {
    bool bFound = false;
    CItem* pCurrent = m_pListBegin;
    while (pCurrent != nullptr)
    {
      if (pCurrent->oItem == item)
      {
        prvtRemoveItem(pCurrent);
        m_uiSize--;
        CCDELETE(pCurrent);
        bFound = true;
      }
      else
      {
        pCurrent = pCurrent->pForward;
      }
    }
    return bFound;
  }

  /**
   * @brief Delete Item wich was previously dequeued
   * @param uiPos: Position of Item
   */
  iterator removeIterator(iterator& oItem)
  {
    CItem* pItem = oItem.m_pItem;
    iterator oReturn = (oItem++);
    prvtRemoveItem(pItem);
    m_uiSize--;
    CCDELETE(pItem);
    return oReturn;
  }

  /**
   * @brief Insert a Item at a defined Position.
   * @param uiPos: Position to store at
   * @param item: Item to store
   */
  iterator insert(size_t uiPos, TYPE&& oToAppend)
  {
    CItem* pItemNext = prvtItemAt(uiPos);
    CItem* pItemPrv = nullptr;
    if (pItemNext != nullptr) pItemPrv = pItemNext->pBackward;
    CCNEWTYPE(pItem, CItem, pItemNext, pItemPrv, CCMOVE(oToAppend));
    if (pItemPrv)
    {
      pItemPrv->pForward = pItem;
    }
    else
    {
      m_pListBegin = pItem;
    }
    if (pItemNext)
    {
      pItemNext->pBackward = pItem;
    }
    else
    {
      m_pListEnd = pItem;
    }
    m_uiSize++;
    return iterator(pItem);
  }

  /**
   * @brief Insert a Item at a defined Position.
   * @param uiPos: Position to store at
   * @param item: Item to store
   */
  iterator insert(size_t uiPos, const TYPE& oToAppend)
  {
    CItem* pItemNext = prvtItemAt(uiPos);
    CItem* pItemPrv = nullptr;
    if (pItemNext != nullptr) pItemPrv = pItemNext->pBackward;
    CCNEWTYPE(pItem, CItem, pItemNext, pItemPrv, oToAppend);
    if (pItemPrv)
    {
      pItemPrv->pForward = pItem;
    }
    else
    {
      m_pListBegin = pItem;
    }
    if (pItemNext)
    {
      pItemNext->pBackward = pItem;
    }
    else
    {
      m_pListEnd = pItem;
    }
    m_uiSize++;
    return iterator(pItem);
  }

  /**
   * @brief Insert a Item at a defined Position.
   * @param uiPos: Position to store at
   * @param item: Item to store
   */
  iterator insert(size_t uiPos, iterator& oItem)
  {
    if (uiPos > size())
    {
      // nothing to do
    }
    else if (uiPos > 0 && uiPos < size())
    {
      CItem* pItemNext = prvtItemAt(uiPos);
      oItem.m_pItem->pForward = pItemNext;
      oItem.m_pItem->pBackward = pItemNext->pBackward;
      if (pItemNext->pBackward)
      {
        pItemNext->pBackward->pForward = oItem.m_pItem;
      }
      pItemNext->pBackward = oItem.m_pItem;
      m_uiSize++;
    }
    else if(uiPos == 0)
    {
      oItem.m_pItem->pForward = m_pListBegin;
      oItem.m_pItem->pBackward = nullptr;
      m_pListBegin = oItem.m_pItem;
      m_uiSize++;
    }
    else
    {
      oItem.m_pItem->pForward = nullptr;
      oItem.m_pItem->pBackward = m_pListEnd;
      m_pListEnd = oItem.m_pItem;
      if (oItem.m_pItem->pBackward)
        oItem.m_pItem->pBackward->pForward = oItem.m_pItem;
      m_uiSize++;
    }
    return oItem;
  }
  
  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator begin()
  {
    return iterator(m_pListBegin);
  }
  
  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator end()
  {
    return iterator(nullptr);
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  const_iterator begin(void) const
  {
    return m_pListBegin;
  }
  
  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  const_iterator end(void) const
  {
    return nullptr;
  }

  /**
   * @brief check if item is allready added to List
   * @return true if list contains item, otherwise false
   */
  size_t find(const TYPE& rCompareItem) const
  {
    size_t i = 0;
    for (const TYPE& rItem : *this)
    {
      if (rCompareItem == rItem)
        return i;
      i++;
    }
    return SIZE_MAX;
  }

  /**
   * @brief Check if item is placed in list.
   * @return true if list contains item, otherwise false
   */
  inline bool contains(const TYPE& item) const
  {
    return find(item) != SIZE_MAX;
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
   * @brief Copy List to new one and append Item to it.
   * @param rItem: Item to append to new list.
   * @return new List
   */
  CcList<TYPE> operator+(const TYPE& rItem)
  { 
    CcList<TYPE> newList = *this;
    newList.append(rItem);
    return newList;
  }

  /**
   * @brief Append Item to List
   * @param rItem: Item to append
   * @return Reference to this List
   */
  inline CcList<TYPE>& operator+=(const TYPE& rItem)
  { return append(rItem); }

  /**
   * @brief Copy List to new one but delete a specific item;
   * @param Item to add
   * @return new List
   */
  CcList<TYPE> operator-(const TYPE& item)
  {
    CcList<TYPE> newList(*this);
    newList.removeItem(item);
    return newList;
  }
  
  /**
   * @brief Move assignment Operator
   * @param oToMove: Object to move to this
   * @return this
   */
  CcList& operator=(CcList&& oToMove) NOEXCEPT
  {
    if (this != &oToMove)
    {
      clear();
      m_pListBegin = oToMove.m_pListBegin;
      m_pListEnd = oToMove.m_pListEnd;
      m_uiSize = oToMove.m_uiSize;
      oToMove.m_pListBegin = nullptr;
      oToMove.m_pListEnd = nullptr;
      oToMove.m_uiSize = 0;
    }
    return *this;
  }

  /**
   * @brief Replace current content of CcList with new;
   * @param oToSet: List to replace with current list
   * @return new List
   */
  CcList<TYPE>& operator=(const CcList &oToSet)
  {
    clear();
    return append(oToSet);
  }

  /**
   * @brief Delete Specific Item from Vector
   * @param Item to delete
   * @return Reference to this List
   */
  CcList<TYPE>& operator-=(const TYPE& oToDelete)
  {
    removeItem(oToDelete);
    return *this;
  }

private:
  CItem* prvtItemAt(size_t uiPos) const
  {
    CItem* pCurrent = m_pListBegin;
    while (uiPos > 0)
    {
      pCurrent = pCurrent->pForward;
      uiPos--;
    }
    return pCurrent;
  }

  void prvtRemoveItem(CItem* pItemToRemove)
  {
    if (pItemToRemove->pForward != nullptr)
    {
      pItemToRemove->pForward->pBackward = pItemToRemove->pBackward;
    }
    else
    {
      m_pListEnd = pItemToRemove->pBackward;
    }
    if (pItemToRemove->pBackward != nullptr)
    {
      pItemToRemove->pBackward->pForward = pItemToRemove->pForward;
    }
    else
    {
      m_pListBegin = pItemToRemove->pForward;
    }
  }

private:
  CItem* m_pListBegin = nullptr;
  CItem* m_pListEnd = nullptr;
  size_t m_uiSize = 0;
};

#endif // H_CcList_H_
