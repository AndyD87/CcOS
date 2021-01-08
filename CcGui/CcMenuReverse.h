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
 * @page      CcGui
 * @subpage   CcMenuReverse
 *
 * @page      CcMenuReverse
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMenuReverse
 */
#ifndef H_CCMENUREVERSE_H_
#define H_CCMENUREVERSE_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcVector.h"

class CcMenuItem;

#ifdef _MSC_VER
template class CcGuiSHARED CcVector<CcMenuItem*>;
#endif

/**
 * @brief Reverse Tree of Menue to get positions and following the route.
 * @todo Possibly obsolete.
 */
class CcGuiSHARED CcMenuReverse : public CcVector<CcMenuItem*>
{
public:
  /**
   * @brief Constructor
   */
  CcMenuReverse();

  /**
   * @brief Destructor
   */
  virtual ~CcMenuReverse();

  /**
   * @brief set position to next Item
   */
  void nextPos();

  /**
   * @brief get item from actual position
   * @return actual item
   */
  CcMenuItem* getPos();

  /**
   * @brief reset position to first element in list
   */
  void resetPos();

private:
  uint16 m_Pos;
};

#endif // H_CcMENUREVERSE_H_
