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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CStyleSheet
 **/
#pragma once

#include "CcBase.h"
#include "CcMapCommon.h"

class CcColor;

namespace NQt
{

/**
 * @brief Class impelmentation
 */
class CStyleSheet : private CcStringMap
{
public:
  /**
   * @brief Constructor
   */
  CStyleSheet();

  /**
   * @brief Destructor
   */
  virtual ~CStyleSheet();

  //! @return Generate css stylesheet
  CcString getStyleSheet();
  //! @return Generate css stylesheet with curly bracets
  CcString getStyleSheet(const CcString& sBaseType);

  //! @param sPath: Target path to Background image
  void setBackgroundImage(const CcString& sPath);
  //! @param oColor: Target color for Background
  void setBackgroundColor(const CcColor& oColor);
  //! @param oColor: Target color for Foreground
  void setForegroundColor(const CcColor& oColor);
  //! @param oColor: Target color for Border
  void setBorderColor(const CcColor& oColor);
  //! @param uiSize: Target border size
  void setBorderSize(uint16 uiSize);
};

}
