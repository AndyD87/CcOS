
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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFont
 */

#include "CcFont.h"

#pragma pack(push, 4)
// Generated structures with informations from here:
// - http://stevehanov.ca/blog/?id=143
// - https://docs.microsoft.com/en-us/typography/opentype/spec/otff#font-tables
class CcFont::CPrivate
{
public:
};
#pragma pack(pop)

CcFont::CcFont()
{
}

void CcFont::loadFont(const CcString& sPath)
{
  m_sFontFile = sPath;
  m_oFontFile.setFilePath(sPath);
  if(m_oFontFile.open())
  {
    m_oFontFile.parse();
  }
}

CcString CcFont::getName()
{
  CcString sName;
  if(m_oFontFile.getName() != nullptr)
  {
    sName = m_oFontFile.getName()->getFamilyName();
  }
  return sName;
}

/*
QPixmap CcFont::drawGlyph(uint16 uiSign)
{
  int32 uiWidht = static_cast<int32>(m_oFontFile.getHead()->getXMax()) - m_oFontFile.getHead()->getXMin();
  int32 uiHeight = static_cast<int32>(m_oFontFile.getHead()->getYMax()) - m_oFontFile.getHead()->getYMin();
  QPixmap oPixMap(uiWidht, uiHeight);
  if (m_oFontFile.getMaxp() != nullptr &&
    uiSign < m_oFontFile.getMaxp()->getNumGlyphs())
  {
    //uint16 uiUnitsPerEm = m_oFontFile.getHead()->getUintsPerEm();
    QPoint oCenter(m_oFontFile.getHead()->getXMin(), m_oFontFile.getHead()->getYMin());
    if (getGlyphType(uiSign) == CcFontTableGlyph::EType::Simple)
    {
      CcFontTableGlyph::CSimple oSign = getSimpleGlyph(uiSign);
      drawGlyph(oPixMap, oCenter, oSign);
    }
    else
    {
      CcFontTableGlyph::CCompound oSign = getCompoundGlyph(uiSign);
      for (CcFontTableGlyph::CCompound::CComponent& rComponent : oSign.oComponents)
      {
        if (getGlyphType(rComponent.uiIndex) == CcFontTableGlyph::EType::Simple)
        {
          CcFontTableGlyph::CSimple oSign = getSimpleGlyph(rComponent.uiIndex);
          if (rComponent.bDoTransform == false)
          {
            oSign.movePoints(rComponent.iXOffset, rComponent.iYOffset);
          }
          else
          {
            oSign.transformPoints(rComponent.oMatrix.fA,
              rComponent.oMatrix.fB,
              rComponent.oMatrix.fC,
              rComponent.oMatrix.fD,
              rComponent.iXOffset,
              rComponent.iYOffset);
          }
          drawGlyph(oPixMap, oCenter, oSign);
        }
        else
        {
          qDebug() << "Unknown component";
        }
      }
    }
  }
  return oPixMap;
}

void CcFont::drawGlyph(QPixmap& oPixMap, QPoint& oCenter, const CcFontTableGlyph::CSimple& oGlyph)
{
  CCNEWTYPE(pPainter, QPainter, &oPixMap);
  pPainter->setBackground(QBrush(QColor(0xff,0xff,0xff)));
  pPainter->setPen(QPen(QBrush(QColor(0xff,0xff,0xff)),1,Qt::PenStyle::SolidLine));
  pPainter->fillRect(oPixMap.rect(), QBrush(QColor(0xff,0xff,0xff)));
  pPainter->setPen(QPen(QBrush(QColor(0,0,0)),20,Qt::PenStyle::SolidLine));
  QPoint oLastPoint(0,0);
  if(oGlyph.oPoints.size() > 0)
  {
    oLastPoint = oGlyph.oPoints[0].oPoint;
  }
  QPoint oFirstPoint = oLastPoint - oCenter;
  for(int i=0, c=0; i< oGlyph.oPoints.size(); i++)
  {
    // Get line coordinates
    QPoint oBegin = oLastPoint - oCenter;
    QPoint oEnd = oGlyph.oPoints[i].oPoint - oCenter;
    // Revert Y coordinates, Qt is increasing downwards
    oBegin.ry() = oPixMap.height() - oBegin.ry();
    oEnd.ry() = oPixMap.height() - oEnd.ry();

    pPainter->drawLine(oBegin, oEnd);
    oLastPoint = oGlyph.oPoints[i].oPoint;
    if(i == oGlyph.oEndpointsOfContours[c] && i+1 < oGlyph.oPoints.size())
    {
      oFirstPoint.ry() = oPixMap.height() - oFirstPoint.ry();
      pPainter->drawLine(oFirstPoint, oEnd);
      c++;
      i++;
      oLastPoint = oGlyph.oPoints[i].oPoint;
      oFirstPoint = oLastPoint - oCenter;
    }
  }
  oLastPoint -= oCenter;
  oFirstPoint.ry() = oPixMap.height() - oFirstPoint.ry();
  oLastPoint.ry() = oPixMap.height() - oLastPoint.ry();
  pPainter->drawLine(oFirstPoint, oLastPoint );
  CCDELETE(pPainter);
}
*/
