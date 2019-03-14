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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcList
 */
#ifndef _CCLIST_H_
#define _CCLIST_H_

//#include "CcBase.h"
//#include "CcKernelBase.h"

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
    inline CItem(CItem* pForward, CItem* pBackward, TYPE oItem) : oItem(oItem), pForward(pForward), pBackward(pBackward) {}
    TYPE oItem;
    CItem* pForward;
    CItem* pBackward;
  };

  class iterator
  {
    typedef TYPE  value_type;
    typedef TYPE* pointer;
    typedef TYPE& reference;
  public:
    inline iterator() : m_pItem(nullptr)
    {
    }
    inline iterator(CItem* pStart) : m_pItem(pStart)
    {
    }
    inline iterator next()
    {
      return iterator(m_pItem->pForward);
    }

    inline iterator prv()
    {
      return iterator(m_pItem->pBackward);
    }

    TYPE& operator*() const { return m_pItem->oItem; }
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

    inline iterator& operator++()
    {
      m_pItem = m_pItem->pForward;
      return *this;
    }

    inline iterator operator++(int)
    {
      return iterator(m_pItem->pForward);
    }

    inline iterator& operator+=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pForward;
      }
      return *this;
    }

    inline iterator operator-(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pBackward;
      }
      return pReturn;
    }

    inline iterator& operator-=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pBackward;
      }
      return *this;
    }

    inline iterator& operator--()
    {
      m_pItem = m_pItem->pBackward;
      return *this;
    }

    inline iterator operator--(int)
    {
      return iterator(m_pItem->pBackward);
    }

    inline bool operator==(const iterator& oToCompare)
    {
      return oToCompare.m_pItem == m_pItem;
    }

    inline bool operator!=(const iterator& oToCompare)
    {
      return oToCompare.m_pItem != m_pItem;
    }

  private:
    CItem* m_pItem;
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
    inline const_iterator(CItem* pStart) : m_pItem(pStart)
    {
    }
    inline const_iterator next()
    {
      return const_iterator(m_pItem->pForward);
    }

    inline const_iterator prv()
    {
      return const_iterator(m_pItem->pBackward);
    }

    TYPE& operator*() const
    {
      return m_pItem->oItem;
    }

    TYPE* operator->() const
    {
      return &m_pItem->oItem;
    }

    TYPE& getItem()
      {return m_pItem->oItem; }

    inline const_iterator operator+(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pForward;
      }
      return pReturn;
    }

    inline const_iterator& operator++()
    {
      m_pItem = m_pItem->pForward;
      return *this;
    }

    inline const_iterator operator++(int)
    {
      return const_iterator(m_pItem->pForward);
    }

    inline const_iterator& operator+=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pForward;
      }
      return *this;
    }

    inline const_iterator operator-(size_t uiDistance) const
    {
      CItem* pReturn = m_pItem;
      for (size_t i = 0; i < uiDistance; i++)
      {
        pReturn = m_pItem->pBackward;
      }
      return pReturn;
    }

    inline const_iterator& operator-=(size_t uiDistance)
    {
      for (size_t i = 0; i < uiDistance; i++)
      {
        m_pItem = m_pItem->pBackward;
      }
      return *this;
    }

    inline const_iterator& operator--()
    {
      m_pItem = m_pItem->pBackward;
      return *this;
    }

    inline const_iterator operator--(int)
    {
      return const_iterator(m_pItem->pBackward);
    }

    inline bool operator==(const const_iterator& oToCompare)
    {
      if (oToCompare.m_pItem == nullptr && m_pItem == nullptr)
        return true;
      else if (oToCompare.m_pItem == nullptr || m_pItem == nullptr)
        return false;
      else
        return oToCompare.m_pItem->oItem == m_pItem->oItem;
    }

    inline bool operator!=(const const_iterator& oToCompare)
    {
      if (oToCompare.m_pItem == nullptr && m_pItem == nullptr)
        return false;
      else if (oToCompare.m_pItem == nullptr || m_pItem == nullptr)
        return true;
      else
        return !(oToCompare.m_pItem->oItem == m_pItem->oItem);
    }

  private:
    CItem* m_pItem;
  };


  /**
   * @brief Copy-Constructor
   *        Very importand, becaus m_Buffer is not allowed to copy.
   */
  inline CcList(const CcList &oToCopy)
  {
    operator=(oToCopy);
  }

  /**
   * @brief MoveConstructor
   */
  inline CcList(CcList&& oToMove)
  {
    operator=(std::move(oToMove));
  }

  /**
   * @brief Constructor
   * @param item: Item to add on load
   */
  CcList(TYPE item)
  {
    append(item); 
  }

  /**
   * @brief Constructor
   * @param items: Pointer to Items to add on load
   * @param number: Count of Items on Pointer
   */
  CcList(const TYPE* items, size_t count)
  {
    append(items, count);
  }

  /**
   * @brief Constructor
   * @param iCount: Count of Items to reserve in List
   * @todo
   */
  CcList(size_t uiCount = 0)
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
   *
   * @param toAppend: Object to add
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
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& append(CcList<TYPE> &&toAppend)
  {
    while (toAppend.size() > 0)
    {
      append(std::move(toAppend.at(0)));
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
    CItem* pItem = new CItem(nullptr, m_pListEnd, toAppend);
    CCMONITORNEW(pItem);
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
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& append(const TYPE& toAppend)
  {
    CItem* pItem = new CItem(nullptr, m_pListEnd, toAppend);
    CCMONITORNEW(pItem);
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
    m_uiSize--;
    CCDELETE(pItemToDelete);
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
      pItemToDelete = pTemp;
      len--;
    }
    return *this;
  }

  /**
   * @brief Delete a specific Item in List
   * @param item: item to delete
   */
  CcList<TYPE>& removeItem(const TYPE& item)
  {
    for (size_t i = 0; i<size(); i++)
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
  iterator insert(size_t uiPos, const TYPE& oToAppend)
  {
    CItem* pItemNext = prvtItemAt(uiPos);
    CItem* pItemPrv = nullptr;
    if (pItemNext != nullptr) pItemPrv = pItemNext->pBackward;
    CItem* pItem = new CItem(pItemNext, pItemPrv, oToAppend);
    CCMONITORNEW(pItem);
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
    return const_iterator(m_pListBegin);
  }
  
  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  const_iterator end(void) const
  {
    return const_iterator(nullptr);
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

  /**
   * @brief Check if item is placed in list.
   * @return true if list contains item, otherwise false
   */
  inline bool contains(const TYPE& item) const
  {
    return find(item) != SIZE_MAX;
  }

  /**
   * @brief check if item is allready added to List
   * @return true if list contains item, otherwise false
   */
  size_t find(CcList<TYPE>& list) const
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
            if (!(list.at(j) == at(i + j)))
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
  CcList& operator=(CcList&& oToMove)
  {
    if (this != &oToMove)
    {
      m_pListBegin = oToMove.m_pListBegin;
      m_pListEnd = oToMove.m_pListEnd;
      oToMove.m_pListBegin = nullptr;
      oToMove.m_pListEnd = nullptr;
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
  inline CcList<TYPE>& operator-=(const TYPE& oToDelete) 
    { return removeItem(oToDelete); }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcList& oToCompare) const
  {
    bool bSame = true;
    if (size() == oToCompare.size())
    {
      const_iterator oThisList = begin();
      const_iterator oCompareList = oToCompare.begin();
      while (oThisList != end())
      {
        if (oThisList.getItem() == oCompareList.getItem())
        {
          oThisList = oThisList++;
          oCompareList = oCompareList++;
        }
        else
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
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcList& oToCompare) const
   { return !operator==(oToCompare); }

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

#endif /* _CCLIST_H_ */
