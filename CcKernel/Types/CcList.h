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
 * @brief     Class CcList
 */
#pragma once

#include "CcBase.h"

/**
 * @brief List-class with List as base.
 */
template <typename TYPE>
class CcList
{
public:
  /**
   * @brief Each item in list will be created with additional information for next and previously element.
   */
  class CItem
  {
  public:
    /**
     * @brief Create default Item with backward and forward information.
     * @param pForward:   Next element in list
     * @param pBackward:  Previously element in list
     */
    inline CItem(CItem* pForward, CItem* pBackward) : pForward(pForward), pBackward(pBackward) {}

    /**
     * @brief Create initialized Item with backward and forward information.
     * @param pForward:   Next element in list
     * @param pBackward:  Previously element in list
     * @param oItem:      Item to initalize by copy.
     */
    inline CItem(CItem* pForward, CItem* pBackward, const TYPE& oItem) : pForward(pForward), pBackward(pBackward), oItem(oItem) {}

    /**
     * @brief Create initialized Item by move with backward and forward information.
     * @param pForward:   Next element in list
     * @param pBackward:  Previously element in list
     * @param oItem:      Item to initalize by move.
     */
    inline CItem(CItem* pForward, CItem* pBackward, TYPE&& oItem) : pForward(pForward), pBackward(pBackward), oItem(CCMOVE(oItem)) {}

    CItem* pForward;  //!< Next element in list
    CItem* pBackward; //!< Previously element in list
    TYPE oItem;       //!< Stored item
  };

  /**
   * @brief Iterator object to walk through objects wihin thin List
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

    /**
     * @brief Create iterator and adjust it's distance to this iterator.
     * @param uiDistance: Number of Items next to this.
     * @return Generated iterator
     */
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
     * @brief Update this iterator by reducing List by one
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
      return !(oToCompare.m_pItem == m_pItem);
    }

    CItem* m_pItem; //!< Stored item to walk through list
  };

  /**
   * @brief Iterator object to walk through objects wihin thin List without
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

    /**
     * @brief Get stored item from iterator as reference
     * @return Referenced stored item
     */
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
   * @param oToCopy: List to copy from
   */
  CcList(const CcList &oToCopy)
  {
    operator=(oToCopy);
  }

  /**
   * @brief Move List content from a list to another.
   * @param oToMove: List to move from
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
   * @param uiCount: Count of Items to reserve in List
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
   * @return Reference to this List
   */
  CcList<TYPE>& append(const CcList<TYPE> &oToAppend)
  {
    for(const TYPE& rItem : oToAppend)
    {
      append(rItem);
    }
    return *this;
  }
  
  /**
   * @brief Add an array of objects at the end of list
   * @param oToAppend: Array of objects to add
   * @param uiCount:   Number of item in oToAppend to add.
   * @return Reference to this List
   */
  CcList<TYPE>& append(const TYPE* oToAppend, size_t uiCount)
  {
    while (uiCount > 0)
    {
      append(*oToAppend);
      oToAppend++;
      uiCount--;
    }
    return *this;
  }
  
  /**
   * @brief Add an Object at the end of list
   *
   * @param oToAppend: Object to add
   * @return Reference to this List
   */
  CcList<TYPE>& append(CcList<TYPE> &&oToAppend)
  {
    while (oToAppend.size() > 0)
    {
      append(CCMOVE(oToAppend.at(0)));
      oToAppend.remove(0);
    }
    return *this;
  } 

  /**
   * @brief Add an Object at the end of list
   * @param oToAppend: Object to add
   * @return Reference to this List
   */
  CcList<TYPE>& append(TYPE &&oToAppend)
  {
    CCNEWTYPE(pItem, CItem, nullptr, m_pListEnd, CCMOVE(oToAppend));
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
   * @param oToAppend: Object to add
   * @return Reference to this List
   */
  CcList<TYPE>& append(const TYPE& oToAppend)
  {
    CCNEWTYPE(pItem, CItem, nullptr, m_pListEnd, oToAppend);
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
   * @param oToAppend: Object to add
   * @return Reference to this List
   */
  CcList<TYPE>& append(iterator& oToAppend)
  {
    oToAppend.m_pItem->pForward = nullptr;
    oToAppend.m_pItem->pBackward = m_pListEnd;
    if (m_pListEnd != nullptr)
    {
      m_pListEnd->pForward = oToAppend.m_pItem;
      m_pListEnd = oToAppend.m_pItem;
    }
    else
    {
      m_pListEnd = oToAppend.m_pItem;
      m_pListBegin = oToAppend.m_pItem;
    }
    m_uiSize++;
    return *this;
  }

  /**
   * @brief Add an empty Object at the end of list
   * @return Reference to this List
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
   * @param oToAppend: Object to add
   * @return Reference to this List
   */
  inline CcList<TYPE>& add(const CcList<TYPE> &oToAppend)
  { return append(oToAppend); }

  /**
   * @brief Add an Object at the end of list
   * @param oToAppend: Object to add
   * @return Reference to this List
   */
  inline CcList<TYPE>& add(const TYPE& oToAppend)
  { return append(oToAppend); }

  /**
   * @brief Add an Array of Object at the end of list
   * @param oToAppend: Object to add
   * @param uiCount: Object to add
   * @return Reference to this List
   */
  inline CcList<TYPE>& add(const TYPE* oToAppend, size_t uiCount)
  { return append(oToAppend, uiCount); }

  /**
   * @brief Remove item from List and store in iterator.
   *        This usage is dangerous, because the stored item will not be destroyed outside list.
   * @param uiPos: Deque at specific location.
   * @return Iterator wich enqued element.
   */
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

  /**
   * @brief Dequeue an item within an iterator from this list.
   *        This usage is dangerous, because the stored item will not be destroyed outside list.
   * @param oIterator: Iterator with stored item to remove from list.
   * @return Next iterator from list.
   */
  iterator dequeue(iterator& oIterator)
  {
    CItem* pItem = oIterator.m_pItem;
    iterator oReturn = (oIterator++);
    prvtRemoveItem(pItem);
    m_uiSize--;
    pItem->pForward = nullptr;
    pItem->pBackward = nullptr;
    return oReturn;
  }

  /**
   * @brief Remove first item from List and store in iterator.
   *        This usage is dangerous, because the stored item will not be destroyed outside list.
   * @return Iterator wich enqued element.
   */
  inline iterator dequeueFirst()
  { return dequeue(0); }

  /**
   * @brief Remove last item from List and store in iterator.
   *        This usage is dangerous, because the stored item will not be destroyed outside list.
   * @return Iterator wich enqued element.
   */
  inline iterator dequeueLast()
  { return dequeue(size()-1); }

  /**
   * @brief Add an Object at the beginning
   * @param oToAppend: Object to add
   * @return Handle to this list.
   */
  CcList<TYPE>& prepend(const TYPE &oToAppend)
  {
    insert(0, oToAppend);
    return *this;
  }

  /**
   * @brief Add an Object at the beginning
   * @param oToAppend: Object to add
   * @return Handle to this list.
   */
  CcList<TYPE>& prepend(TYPE&& oToAppend)
  {
    insert(0, oToAppend);
    return *this;
  }

  /**
   * @brief Add an object, stored in iterator.
   * @param oItem: Object in iterator to add
   * @return Reference to this list.
   */
  CcList<TYPE>& prepend(iterator& oItem)
  {
    insert(0, oItem);
    return *this;
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
   *        It must be save that at least one item is available bevor.
   * @return Reference to the last item in list.
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
   * @return True if item was found and removed
   */
  bool remove(size_t uiPos)
  {
    bool bSuccess = false;
    CItem* pItemToDelete = prvtItemAt(uiPos);
    if(pItemToDelete)
    {
      prvtRemoveItem(pItemToDelete);
      CCDELETE(pItemToDelete);
      m_uiSize--;
      bSuccess = true;
    }
    return bSuccess;
  }

  /**
   * @brief Delete multiple items on defined position.
   * @param uiPos:    Position of item to remvoe
   * @param uiLength: Number of items to remove.
   * @return True if at least one item was removed
   */
  bool remove(size_t uiPos, size_t uiLength)
  {
    bool bSuccess = false;
    m_uiSize -= uiLength;
    CItem* pItemToDelete = prvtItemAt(uiPos);
    if(pItemToDelete)
    {
      bSuccess = true;
      CItem* pTemp;
      while (uiLength)
      {
        pTemp = pItemToDelete->pForward;
        prvtRemoveItem(pItemToDelete);
        m_uiSize--;
        CCDELETE(pItemToDelete);
        pItemToDelete = pTemp;
        uiLength--;
      }
    }
    return bSuccess;
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
   * @brief Delete Item of given iterator and update it to next item.
   * @param oIterator: Iterator with stored item
   * @return Next element of iterator.
   */
  iterator removeIterator(iterator& oIterator)
  {
    CItem* pItem = oIterator.m_pItem;
    iterator oReturn = (oIterator++);
    prvtRemoveItem(pItem);
    m_uiSize--;
    CCDELETE(pItem);
    return oReturn;
  }

  /**
   * @brief Insert a Item at a defined Position.
   * @param uiPos: Position to store at
   * @param oToAppend: Item to store
   * @return Created iterator of element.
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
   * @param oToAppend: Item to store
   * @return Created iterator of element.
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
   * @param oItem: Item to store
   * @return Created iterator of element.
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
   * @brief Create an iterator from begining of list.
   * @return Iterator with item on position 0.
   */
  iterator begin()
  {
    return iterator(m_pListBegin);
  }
  
  /**
   * @brief Create an iterator at the next postion of after last item in list.
   *        Because this list will mark nullptr to define end, iterator will contain a
   *        nullptr element as item.
   * @return Iterator with item behind last item.
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
   * @brief Check if item is allready added to List.
   *        If an true/false is required look at @ref contains
   * @param rCompareItem: Item to search for
   * @return Position in list if item was found, otherwise SIZE_MAX
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
   * @brief Check if item is allready added to List.
   *        If a position is required look at @ref find
   * @param rCompareItem: Item to search for
   * @return true if list contains item, otherwise false
   */
  inline bool contains(const TYPE& rCompareItem) const
  {
    return find(rCompareItem) != SIZE_MAX;
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
   * @brief Move item from list to new one.
   * @param oItem: Item to search and remove
   * @return new List
   */
  CcList<TYPE> operator-(const TYPE& oItem)
  {
    CcList<TYPE> newList(*this);
    newList.removeItem(oItem);
    return newList;
  }

  /**
   * @brief Delete Specific Item from List
   * @param oToDelete: Item to delete
   * @return Reference to this List.
   */
  CcList<TYPE>& operator-=(const TYPE& oToDelete)
  {
    removeItem(oToDelete);
    return *this;
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

private:
  CItem* prvtItemAt(size_t uiPos) const
  {
    CItem* pCurrent = m_pListBegin;
    while (uiPos > 0 && pCurrent != nullptr)
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
