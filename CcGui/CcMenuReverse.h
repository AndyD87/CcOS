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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcMenuReverse
 */
#ifndef CCMENUREVERSE_H_
#define CCMENUREVERSE_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcList.h"

class CcMenuItem;

#ifdef WIN32
template class CcGuiSHARED CcList<CcMenuItem*>;
#endif

/**
 * @brief Reverse Tree of Menue to get positions and following the route.
 * @todo Possibly obsolete.
 */
class CcGuiSHARED CcMenuReverse : public CcList<CcMenuItem*>
{
public:
  /**
   * @brief Constructor
   */
  CcMenuReverse(void);

  /**
   * @brief Destructor
   */
  virtual ~CcMenuReverse(void);

  /**
   * @brief set position to next Item
   */
  void nextPos(void);

  /**
   * @brief get item from actual position
   * @return actual item
   */
  CcMenuItem* getPos(void);

  /**
  * @brief reset position to first element in list
  */
  void resetPos(void);

private:
  uint16 m_Pos;
};

#endif /* CCMENUREVERSE_H_ */
