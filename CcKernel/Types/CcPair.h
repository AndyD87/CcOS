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
 * @page      CcPair
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcPair
 **/
#ifndef CcPair_H_
#define CcPair_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcList.h"

/**
 * @brief Class impelmentation
 */
template <typename KEY, typename VALUE>
class CcPair
{
public:
  /**
   * @brief Constructor
   */
  CcPair( void )
  {}

  /**
   * @brief Constructor with Key and Value
   */
  CcPair(const KEY& oKey, const VALUE& oValue) : m_oKey(oKey), m_oValue(oValue)
  {}

  CcPair( const CcPair& oToCopy)
  { operator=(oToCopy); }

  CcPair( CcPair&& oToMove)
  {
    operator=(std::move(oToMove));
  }

  /**
   * @brief Destructor
   */
  virtual ~CcPair(void)
    {  }

  KEY& key() { return m_oKey; }
  VALUE& value() { return m_oValue; }
  
  const KEY& getKey() const { return m_oKey; }
  const VALUE& getValue() const { return m_oValue; }

  CcPair& operator=(const CcPair& oToCopy)
  {
    m_oKey = oToCopy.m_oKey;
    m_oValue = oToCopy.m_oValue;
    return *this;
  }

  CcPair& operator=(CcPair&& oToMove)
  {
    if (&oToMove != this)
    {
      m_oKey = std::move(oToMove.m_oKey);
      m_oValue = std::move(oToMove.m_oValue);
    }
    return *this;
  }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
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
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcPair<KEY, VALUE>& oToCompare) const
  {
    bool bRet = oToCompare.m_oKey != m_oKey;
    if (bRet)
    {
      bRet = oToCompare.m_oValue != m_oValue;
    }
    return bRet;
  }
private:
  KEY m_oKey;
  VALUE m_oValue;
};

#endif /* CcPair_H_ */
