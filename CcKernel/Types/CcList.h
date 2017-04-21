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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcList
 */
#ifndef CCLIST_H_
#define CCLIST_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include <deque>

/**
 * @brief List-class with List as base.
 */
template <typename TYPE>
class CcList {
public:
  typedef typename std::deque<TYPE>::iterator iterator;
  /**
  * @brief Constructor
  */
  CcList()
  {
    m_List = new std::deque<TYPE>(); CCMONITORNEW(m_List);
  }
  
  /**
   * @brief Copy-Constructor
   *        Very importand, becaus m_Buffer is not allowed to copy.
   */
  CcList(const CcList &oToCopy)
  {
    m_List = new std::deque<TYPE>(); CCMONITORNEW(m_List);
    if (oToCopy.m_List->size() > 0)
    {
      m_List->insert(m_List->begin(), oToCopy.m_List->begin(), oToCopy.m_List->end());
    }
  }

  /**
   * @brief MoveConstructor
   */
  CcList(CcList&& oToMove)
  {
    operator=(std::move(oToMove));
  }

  /**
  * @brief Constructor
  * @param item: Item to add on load
  */
  CcList(TYPE item)
  {
    m_List = new std::deque<TYPE>(); 
    CCMONITORNEW(m_List);
    append(item); 
  }

  /**
  * @brief Constructor
  * @param items: Pointer to Items to add on load
  * @param number: Count of Items on Pointer
  */
  CcList(const TYPE* items, size_t count)
  {
    m_List = new std::deque<TYPE>();  
    CCMONITORNEW(m_List);
    append(items, count);
  }

  /**
  * @brief Constructor
  * @param number: Count of Items to reserve in List
  */
  CcList(size_t count)
  {
    m_List = new std::deque<TYPE>(count); CCMONITORNEW(m_List);
  }

  /**
  * @brief Destructor
  */
  virtual ~CcList()
  {
    deleteBuffers();
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& append(const CcList<TYPE> &toAppend)
  {
    m_List->insert(m_List->end(), toAppend.m_List->begin(), toAppend.m_List->end());
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
    m_List->push_back(std::move(toAppend));
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcList<TYPE>& append(const TYPE& toAppend)
  {
    m_List->push_back(toAppend);
    return *this;
  }

  CcList<TYPE>& append(const TYPE* toAppend, size_t count)
  {
    m_List->insert(m_List->end(), toAppend, toAppend + count);
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
    { return m_List->size(); }

  /**
  * @brief Get the Object stored at requested position
  *
  * @param uiPos: position of requested Object, must be lower than size()
  * @return requested Object
  */
  TYPE& at(size_t uiPos) const
    { return m_List->at(uiPos); }

  /**
   * @brief Get last Item in List
   * @param return the last item in list. It must be save that at least one item is inserted bevor.
   */
  TYPE& last(void)
  { return m_List->back(); }

  /**
  * @brief Deletes all entries in list.
  */
  void clear(void)
  { m_List->clear(); }

  /**
   * @brief Deletes all entries in list and execute a delete on all items.
   * 
   */
  void clearForce(void)
  {
    while (size() > 0)
    {
      remove(0);
    }
  }

  /**
  * @brief Delete Item on defined Position
  * @param uiPos: Position of Item
  */
  CcList<TYPE>& remove(size_t uiPos)
  {
    m_List->erase(m_List->begin() + uiPos, m_List->begin() + uiPos + 1);
    return *this;
  }

  /**
  * @brief Delete Item on defined Position
  * @param uiPos: Position of Item
  */
  CcList<TYPE>& remove(size_t uiPos, size_t len)
  {
    m_List->erase(m_List->begin() + uiPos, m_List->begin() + uiPos + len);
    return *this;
  }

  /**
  * @brief Delete a specific Item in List
  * @param item: item to delete
  */
  CcList<TYPE>& removeItem(const TYPE& item)
  {
    for (uint32 i = 0; i<size(); i++)
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
  void insertAt(size_t uiPos, const TYPE& item)
  {
    m_List->insert(m_List->begin() + uiPos, item);
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator begin(void) const
  {
    return m_List->begin();
  }
  
  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator end(void) const
  {
    return m_List->end();
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
   * @brief Get Content of List as std::deque
   * @return std::deque with content
   */
  std::deque<TYPE> getStdList()const{ return (*m_List); }

  /**
   * @brief Return value at Position
   * @param uiPos: position to return
   * @return Item at postion
   */
  inline TYPE& operator[](size_t uiPos) const { return at(uiPos); }

  /**
   * @brief Copy List to new one and append Item to it.
   * @param Item to add
   * @return new List
   */
  CcList<TYPE> operator+(const TYPE& item)
  { 
    CcList<TYPE> newList = *this;
    newList.append(item);
    return newList;
  }

  /**
   * @brief Append Item to List
   * @param Item to add
   * @return Reference to this List
   */
  inline CcList<TYPE>& operator+=(const TYPE& item)
  { return append(item); }

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
   * @brief Replace current content of CcList with new;
   * @param oToSet: List to replace with current list
   * @return new List
   */
  CcList<TYPE>& operator=(const CcList &oToSet)
  {
    m_List->clear();
    return append(oToSet);
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
      deleteBuffers();
      m_List = oToMove.m_List;
      oToMove.m_List = NULL;
    }
    return *this;
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
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcList& oToCompare) const
   { return (*m_List == *oToCompare.m_List); }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcList& oToCompare) const
   { return !operator==(oToCompare); }

private:
  void deleteBuffers(void)
  {
    if (m_List != nullptr)
    {
      m_List->clear();
      CCMONITORDELETE(m_List); 
      delete m_List;
    }
  }
  std::deque<TYPE> *m_List = nullptr; //!< List with saved Items
};

#endif /* CCLIST_H_ */
