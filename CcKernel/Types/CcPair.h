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
 * @brief     Class CcPair
 **/
#pragma once

#include "CcBase.h"
#include "CcList.h"

/**
 * @brief Class implementation
 */
template <typename KEY, typename VALUE>
class CcPair
{
public:
  CcPair() = default;
  ~CcPair() = default;

  /**
   * @brief Create pair and init with key and value
   * @param oKey:   Init key
   * @param oValue: Init value
   */
  CcPair(const KEY& oKey, const VALUE& oValue) : m_oKey(oKey), m_oValue(oValue)
    {}
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcPair)

  //! @return Get key
  KEY& getKey()
  { return m_oKey; }
  //! @return Get value
  VALUE& getValue()
  { return m_oValue; }
  //! @return Get key
  const KEY& getKey() const
  { return m_oKey; }
  //! @return Get value
  const VALUE& getValue() const
  { return m_oValue; }

  //! @param oKey: Set key
  void setKey(const KEY& oKey)
  { m_oKey = oKey; }
  //! @param oValue: Set value
  void setValue(const VALUE& oValue)
  { m_oValue = oValue; }

  /**
   * @brief Move content of another pair to this
   * @param oToMove: Object to move from
   * @return Handle to this
   */
  CcPair& operator=(CcPair&& oToMove) NOEXCEPT
  {
    if (&oToMove != this)
    {
      m_oKey = CCMOVE(oToMove.m_oKey);
      m_oValue = CCMOVE(oToMove.m_oValue);
    }
    return *this;
  }

  /**
   * @brief Copy content of another pair to this
   * @param oToCopy: Object to copy from
   * @return Handle to this
   */
  CcPair& operator=(const CcPair& oToCopy)
  {
    m_oKey = oToCopy.m_oKey;
    m_oValue = oToCopy.m_oValue;
    return *this;
  }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcPair<KEY, VALUE>& oToCompare) const
  {
    bool bRet = oToCompare.m_oKey == m_oKey;
    if (bRet)
    {
      bRet = oToCompare.m_oValue == m_oValue;
    }
    return bRet;
  }

  /**
   * @brief Compare two items, check if they are even and negotate result
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcPair<KEY, VALUE>& oToCompare) const
  { return !operator==(oToCompare); }

private:
  KEY m_oKey;
  VALUE m_oValue;
};
