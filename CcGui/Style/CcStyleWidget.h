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
 * @page      Style
 * @subpage   CcStyleWidget
 *
 * @page      CcStyleWidget
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStyleWidget
 **/
#ifndef _CcStyleWidget_H_
#define _CcStyleWidget_H_

#include "CcBase.h"
#include "CcStyle.h"

/**
 * @brief Class impelmentation
 */
class CcGuiSHARED CcStyleWidget 
{
public:
  CcStyleWidget()
    { }

  CcStyleWidget(const CcColor& oBackgroundColor,
                const CcColor& oForegroundColor,
                const CcColor& oBorderColor,
                const uint16  uBorderSize) :
    oBackgroundColor(oBackgroundColor),
    oForegroundColor(oForegroundColor),
    oBorderColor    (oBorderColor    ),
    uBorderSize     (uBorderSize     )
    {}
  virtual ~CcStyleWidget()
    { }

  CcColor oBackgroundColor;
  CcColor oForegroundColor;
  CcColor oBorderColor;
  uint16  uBorderSize;

public:
  static CcStyleWidget Default;
};

#endif /* _CcStyleWidget_H_ */