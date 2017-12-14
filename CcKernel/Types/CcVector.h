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
#ifndef CCVECTOR_H_
#define CCVECTOR_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include <vector>

/**
 * @brief Vector class based on std::vector.
 */
template <typename TYPE>
class CcVector
{
public:
  typedef typename std::vector<TYPE>::iterator iterator;

  /**
   * @brief Constructor
   */
  CcVector()
  {
    m_pVector = new std::vector<TYPE>(); 
    CCMONITORNEW(m_pVector);
  }

  /**
   * @brief Copy-Constructor
   *        Very importand, becaus m_Buffer is not allowed to copy.
   */
  CcVector(const CcVector &oToCopy)
  {
    m_pVector = new std::vector<TYPE>(); 
    CCMONITORNEW(m_pVector);
    operator=(oToCopy);
  }

  /**
   * @brief MoveConstructor
   */
  CcVector(CcVector&& oToMove)
  {
    operator=(std::move(oToMove));
  }

  /**
   * @brief Constructor
   * @param item: Item to add on load
   */
  CcVector(const TYPE& item)
  {
    m_pVector = new std::vector<TYPE>(1);
    CCMONITORNEW(m_pVector);
    append(item);
  }

  /**
   * @brief Constructor
   * @param items: Pointer to Items to add on load
   * @param number: Count of Items on Pointer
   */
  CcVector(const TYPE* items, size_t count)
  {
    m_pVector = new std::vector<TYPE>(items, items + count); 
    CCMONITORNEW(m_pVector);
  }

  /**
   * @brief Constructor
   * @param number: Count of Items to reserve in vector
   */
  CcVector(size_t count)
  {
    m_pVector = new std::vector<TYPE>(count); 
    CCMONITORNEW(m_pVector);
  }

  /**
   * @brief Destructor
   */
  ~CcVector()
  {
    deleteBuffers();
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVector<TYPE>& append(const CcVector<TYPE> &toAppend)
  {
    m_pVector->insert(m_pVector->end(), toAppend.m_pVector->begin(), toAppend.m_pVector->end());
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVector<TYPE>& append(const CcVector<TYPE>& toAppend, size_t count)
  {
    m_pVector->insert(m_pVector->end(), toAppend.m_pVector->begin(), toAppend.m_pVector->begin() + count);
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVector<TYPE>& append(const TYPE& toAppend)
  {
    m_pVector->push_back(toAppend);
    return *this;
  }

  CcVector<TYPE>& append(const TYPE* toAppend, size_t count)
  {
    m_pVector->insert(m_pVector->end(), toAppend, toAppend + count);
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVector<TYPE>& add(const CcVector<TYPE> &toAppend)
  {
    return append(toAppend);
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVector<TYPE>& add(const TYPE& toAppend)
  {
    return append(toAppend);
  }

  /**
   * @brief Add an Array of Object at the end of list
   * @param toAppend: Object to add
   * @param count: Object to add
   */
  CcVector<TYPE>& add(const TYPE* toAppend, size_t count)
  {
    return append(toAppend, count);
  }

  /**
   * @brief Get the number of items containing in list.
   *
   * @return Number of Items
   */
  size_t size(void) const
  {
    return m_pVector->size();
  }

  /**
   * @brief Get the Object stored at requested position
   *
   * @param uiPos: position of requested Object, must be lower than size()
   * @return requested Object
   */
  TYPE& at(size_t uiPos) const
  {
    return m_pVector->at(uiPos);
  }

  /**
   * @brief Get last Item in List
   * @param return the last item in list. It must be save that at least one item is inserted bevor.
   */
  TYPE& last(void)
  {
    return m_pVector->back();
  }

  /**
   * @brief Deletes all entries in list.
   */
  void clear(void)
  {
    m_pVector->clear();
  }

  /**
   * @brief Deletes all entries in list and execute a delete on all items.
   *
   */
  void clearForce(void)
  {
    while (size() > 0)
    {
      //delete at(0);
      remove(0);
    }
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcVector<TYPE>& remove(size_t uiPos)
  {
    m_pVector->erase(m_pVector->begin() + uiPos);
    return *this;
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcVector<TYPE>& remove(size_t uiPos, size_t len)
  {
    m_pVector->erase(m_pVector->begin() + uiPos, m_pVector->begin() + uiPos + len);
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
  void insertAt(size_t uiPos, const TYPE& item)
  {
    m_pVector->insert(m_pVector->begin() + uiPos, item);
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator begin(void) const
  {
    return m_pVector->begin();
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  iterator end(void) const
  {
    return m_pVector->end();
  }

  void resize(size_t uiNewSize)
  {
    m_pVector->resize(uiNewSize);
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
    return (*m_pVector).data() + pos;
  }

  /**
   * @brief Return value at Position
   * @param uiPos: position to return
   * @return Item at postion
   */
  TYPE& operator[](size_t uiPos) const
  {
    return at(uiPos);
  }

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
  CcVector& operator=(CcVector&& oToMove)
  {
    if (this != &oToMove)
    {
      CCDELETE(m_pVector);
      m_pVector = oToMove.m_pVector;
      oToMove.m_pVector = nullptr;
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
    m_pVector->clear();
    return append(oToCopy);
  }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcVector& oToCompare) const
    { return *m_pVector == *oToCompare.m_pVector; }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator!=(const CcVector& oToCompare) const
    { return *m_pVector != *oToCompare.m_pVector; }

  CcVector<TYPE>& operator+=(const CcVector& oToAppend)
    { return append(oToAppend); }
  
  CcVector<TYPE> operator+(const CcVector& oToAppend)
    { CcVector<TYPE> oData(*this); return oData.append(oToAppend); }

private:
  void deleteBuffers(void)
  {
    CCDELETE(m_pVector);
  }

private:
  std::vector<TYPE> *m_pVector = nullptr;         //!< vector with saved Items
};

#endif /* CCVECTOR_H_ */
