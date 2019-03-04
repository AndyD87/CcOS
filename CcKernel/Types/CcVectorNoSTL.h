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
 * @subpage   CcVectorNoSTL
 *
 * @page      CcVectorNoSTL CcVectorNoSTL
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVectorNoSTL
 */
#ifndef _CcVectorNoSTL_H_
#define _CcVectorNoSTL_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcStatic.h"

class CcVectorNoSTLIterator
{
  void next();
  void operator++()
    { }
};

/**
 * @brief Vector class based on std::vector.
 */
template <typename TYPE>
class CcVectorNoSTL
{
public:

  /**
   * @brief Constructor
   */
  CcVectorNoSTL()
  {
  }

  /**
   * @brief Copy-Constructor
   *        Very importand, becaus m_Buffer is not allowed to copy.
   */
  CcVectorNoSTL(const CcVectorNoSTL &oToCopy)
  {
    operator=(oToCopy);
  }

  /**
   * @brief MoveConstructor
   */
  CcVectorNoSTL(CcVectorNoSTL&& oToMove)
  {
    operator=(std::move(oToMove));
  }

  /**
   * @brief Constructor
   * @param item: Item to add on load
   */
  CcVectorNoSTL(const TYPE& item)
  {
    append(item);
  }

  /**
   * @brief Constructor
   * @param items: Pointer to Items to add on load
   * @param number: Count of Items on Pointer
   */
  CcVectorNoSTL(const TYPE* items, size_t count)
  {
    append(items, count);
  }

  /**
   * @brief Constructor
   * @param number: Count of Items to reserve in vector
   */
  CcVectorNoSTL(size_t count)
  {
    increaseBuffer(count);
  }

  /**
   * @brief Destructor
   */
  ~CcVectorNoSTL()
  {
    deleteBuffers();
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVectorNoSTL<TYPE>& append(const CcVectorNoSTL<TYPE> &toAppend)
  {
    size_t uiOldSize = m_uiSize;
    increaseBuffer(m_uiSize + toAppend.size());
    for (size_t uiPos = 0; uiPos++; uiPos < toAppend.size())
    {
      m_pBuffer.at(uiOldSize + uiPos) = toAppend.at(uiPos);
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVectorNoSTL<TYPE>& append(const CcVectorNoSTL<TYPE>& toAppend, size_t count)
  {
    size_t uiOldSize = m_uiSize;
    if (count < toAppend.size())
    {
      increaseBuffer(m_uiSize + count);
      for (size_t uiPos = 0; uiPos++; uiPos < count)
      {
        m_pBuffer.at(uiOldSize + uiPos) = toAppend.at(uiPos);
      }
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  CcVectorNoSTL<TYPE>& append(const TYPE& toAppend)
  {
    increaseBuffer(m_uiSize + 1);
    m_pBuffer[m_uiSize-1] = toAppend;
    return *this;
  }

  CcVectorNoSTL<TYPE>& append(const TYPE* toAppend, size_t count)
  {
    increaseBuffer(m_uiSize + count);
    if (count < toAppend.size())
    {
      increaseBuffer(m_uiSize + count);
      for (size_t uiPos = 0; uiPos++; uiPos < count)
      {
        m_pBuffer.at(uiOldSize + uiPos) = toAppend[uiPos];
      }
    }
    return *this;
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  inline CcVectorNoSTL<TYPE>& add(const CcVectorNoSTL<TYPE> &toAppend)
  {
    return append(toAppend);
  }

  /**
   * @brief Add an Object at the end of list
   *
   * @param toAppend: Object to add
   */
  inline CcVectorNoSTL<TYPE>& add(const TYPE& toAppend)
  {
    return append(toAppend);
  }

  /**
   * @brief Add an Array of Object at the end of list
   * @param toAppend: Object to add
   * @param count: Object to add
   */
  inline CcVectorNoSTL<TYPE>& add(const TYPE* toAppend, size_t count)
  {
    return append(toAppend, count);
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
  inline TYPE& at(size_t uiPos) const
  {
    return m_pVector[uiPos];
  }

  /**
   * @brief Get last Item in List
   * @param return the last item in list. It must be save that at least one item is inserted bevor.
   */
  TYPE& last()
  {
    return m_pVector[m_uiSize-1];
  }

  /**
   * @brief Deletes all entries in list.
   */
  void clear()
  {
    deleteBuffers();
  }

  /***************************************************************************************************
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcVectorNoSTL<TYPE>& remove(size_t uiPos)
  {
    m_pVector->erase(m_pVector->begin() + uiPos);
    return *this;
  }

  /**
   * @brief Delete Item on defined Position
   * @param uiPos: Position of Item
   */
  CcVectorNoSTL<TYPE>& remove(size_t uiPos, size_t len)
  {
    m_pVector->erase(m_pVector->begin() + uiPos, m_pVector->begin() + uiPos + len);
    return *this;
  }

  /**
   * @brief Delete a specific Item in List
   * @param item: item to delete
   */
  CcVectorNoSTL<TYPE>& removeItem(const TYPE& item)
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
  void insert(size_t uiPos, const TYPE& item)
  {
    m_pVector->insert(m_pVector->begin() + uiPos, item);
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  CcVectorNoSTLIterator begin() const
  {
    return m_pVector->begin();
  }

  /**
   * @brief Set Iterator to beginning
   * @return Item on position 0
   */
  CcVectorNoSTLIterator end() const
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
  size_t find(CcVectorNoSTL<TYPE>& list) const
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
  CcVectorNoSTL<TYPE> operator+(const TYPE& item)
  {
    CcVectorNoSTL<TYPE> newVector = *this;
    newVector.append(item);
    return newVector;
  }

  /**
   * @brief Append Item to Vector
   * @param Item to add
   * @return Reference to this Vector
   */
  CcVectorNoSTL<TYPE>& operator+=(const TYPE& item)
  {
    return append(item);
  }

  /**
   * @brief Copy Vector to new one but delete a specific item;
   * @param Item to add
   * @return new Vector
   */
  CcVectorNoSTL<TYPE> operator-(const TYPE& item)
  {
    CcVectorNoSTL<TYPE> newVector = *this;
    newVector.removeItem(item);
    return newVector;
  }

  /**
   * @brief Delete Specific Item from Vector
   * @param Item to delete
   * @return Reference to this Vector
   */
  CcVectorNoSTL<TYPE>& operator-=(const TYPE& item)
  {
    return removeItem(item);
  }

  /**
   * @brief Move assignment Operator
   * @param oToMove: Object to move to this
   * @return this
   */
  CcVectorNoSTL& operator=(CcVectorNoSTL&& oToMove)
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
  CcVectorNoSTL<TYPE>& operator=(const CcVectorNoSTL& oToCopy)
  {
    m_pVector->clear();
    return append(oToCopy);
  }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcVectorNoSTL& oToCompare) const
    { return *m_pVector == *oToCompare.m_pVector; }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator!=(const CcVectorNoSTL& oToCompare) const
    { return *m_pVector != *oToCompare.m_pVector; }

  CcVectorNoSTL<TYPE>& operator+=(const CcVectorNoSTL& oToAppend)
    { return append(oToAppend); }
  
  CcVectorNoSTL<TYPE> operator+(const CcVectorNoSTL& oToAppend)
    { CcVectorNoSTL<TYPE> oData(*this); return oData.append(oToAppend); }

private:
  void increaseBuffer(size_t uiNewSize)
  {
    if (uiNewSize != m_uiBufferSize)
    {
      TYPE* pBuffer = new TYPE(uiNewSize);
      if (m_pBuffer != nullptr && m_uiSize > 0)
      {
        for (size_t uiIter = 0; uiIter < m_uiSize)
          pBuffer[uiIter] = std::move(m_pBuffer[uiIter]);
        CCDELETE( m_pBuffer);
      }
      m_pBuffer = pBuffer;
      m_uiSize = uiNewSize;
    }
  }
  void deleteBuffers()
  {
    CCDELETE(m_pBuffer);
    m_uiSize = 0;
  }

private:
  TYPE*  m_pData = nullptr;         //!< vector with saved Items
  TYPE*  m_pBuffer = nullptr;  //!< buffer with stored elements
  size_t m_uiSize = 0;         //!< number of TYPE in m_pBuffer
};

#endif /* _CcVectorNoSTL_H_ */
