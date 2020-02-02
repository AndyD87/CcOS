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
 * @subpage   CcText
 *
 * @page      CcText
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcText
 */

#ifndef H_CCTEXT_H_
#define H_CCTEXT_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcFont.h"
#include "CcString.h"
#include "CcGuiGlobals.h"

/**
 * @brief Class for creating an manipulating a Textbox on Display.
 */
class CcGuiSHARED CcText : public CcWidget 
{
public:
  /**
   * @brief Constructor with Font-size of displaying text
   * @param fontSize: FontSize of Displayin Text in pixles, default:6
   */
  CcText(CcWidget* rParent);
  /**
   * @brief Destructor
   */
  virtual ~CcText() override;

  /**
   * @brief Set Color of Font
   */
  void setFontColor(uchar R, uchar G, uchar B);
  /**
   * @brief Set Offset where Text has to Start on Display
   */
  void setTextOffset(uint16 x, uint16 y );

  /**
   * @brief Set String that has to be shown on Display
   */
  void setText(const CcString& sString);

  /**
   * @brief get Handle to internal String-Member
   */
  const CcString& getText();

  void setBackgroundImage(const CcString& sPath);

private:
  class CPrivate;
private: //members
  CPrivate* m_pPrivate;
};

#endif // H_CcTEXT_H_
