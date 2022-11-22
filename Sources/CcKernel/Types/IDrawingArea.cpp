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
 * @brief     Class IDrawingArea
 */

#include "IDrawingArea.h"
#include "CcRectangle.h"

class CcSubDrawingArea : 
  public IDrawingArea
{
public:
  CcSubDrawingArea(IDrawingArea* pParent, const CcRectangle& oArea) :
    IDrawingArea(oArea.getSize()),
    m_pParent(pParent),
    m_oOffset(oArea.getPoint())
  {

  }

  virtual void setPixel(int32 iX, int32 iY)
  {
    if (m_pParent)
    {
      m_pParent->setPixel(iX + m_oOffset.getX(), iY + m_oOffset.getY());
    }
  }

private:
  IDrawingArea* m_pParent;
  CcPoint       m_oOffset;
};

 /**
  * @brief Get an Drawing object wich is able to draw an a region in Display.
  * @param oArea: Location in Display to query for Painting object
  * @return Painting object or null if an error occured.
  */
IDrawingArea* IDrawingArea::createDrawingArea(const CcRectangle& oArea)
{
  CCNEWTYPE(pArea, CcSubDrawingArea, this, oArea);
  return pArea;
}

void IDrawingArea::removeDrawingArea(IDrawingArea* pArea)
{
  CCDELETE(pArea);
}