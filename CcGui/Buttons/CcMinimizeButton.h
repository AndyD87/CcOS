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
 * @page      CcButton
 * @subpage   CcMinimizeButton
 *
 * @page      CcMinimizeButton
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMinimizeButton
 **/
#ifndef _CcMinimizeButton_H_
#define _CcMinimizeButton_H_

#include "CcBase.h"
#include "CcButton.h"

/**
 * @brief Class impelmentation
 */
class CcMinimizeButton : public CcButton
{
public:
  /**
   * @brief Constructor
   */
  CcMinimizeButton( CcWidgetHandle pParent );

  /**
   * @brief Destructor
   */
  virtual ~CcMinimizeButton( void );

  void draw() override;

private:
  void onMouseClick(const CcPoint& oPosition) override;
};

#endif /* _CcMinimizeButton_H_ */
