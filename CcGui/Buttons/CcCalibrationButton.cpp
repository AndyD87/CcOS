/*
 * @copyright  Andreas Dirmeier (C) 2017
 *
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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Implementation of Class CcSha256
 */
#include "Buttons/CcCalibrationButton.h"
#include "CcPainter.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcStatic.h"
#include "CcGlobalStrings.h"

CcCalibrationButton::CcCalibrationButton(const CcWidgetHandle& rParent) :
  CcButton(rParent),
  m_TextWidget(this),
  m_cross(21, 21, 3)
{
  fillCalibData();
  m_buttonNr = 0;
  m_TextWidget.setFontColor(0x00, 0xff, 0x00);
  m_TextWidget.setPos(CcPoint(30, 10));
  m_TextWidget.setSize(CcSize(200, 200));
  m_PosAbsolute.setPoint(0, 0);
  m_PosRelative.setPoint(0, 0);
  m_Done=false;
}

CcCalibrationButton::~CcCalibrationButton()
{
}

void CcCalibrationButton::draw(bool bDoFlush)
{
  CCUNUSED(bDoFlush);
  drawBackground(getStyle()->oBackgroundColor);
  drawText();
  drawButton();
}

void CcCalibrationButton::drawButton()
{
  CcPainter Painter(getHandle());
  Painter.setColor(0xff, 0, 0);
  Painter.drawCross(Pos1, m_cross);
}

void CcCalibrationButton::drawText()
{
  CcString sDisplayText("Calibrate the Touchpanel\n");
  sDisplayText.append("Press on Position where crosses\n");
  sDisplayText.append("are getting Displayed\n\n");
  sDisplayText.append("Pos1: ( ");
  sDisplayText.appendNumber(m_calibData.touch.X1);
  sDisplayText.append(", ");
  sDisplayText.appendNumber(m_calibData.touch.Y1);
  sDisplayText.append(" )\n");
  sDisplayText.append("Pos2: ( ");
  sDisplayText.appendNumber(m_calibData.touch.X2);
  sDisplayText.append(", ");
  sDisplayText.appendNumber(m_calibData.touch.Y2);
  sDisplayText.append(" )\n");
  sDisplayText.append("Pos3: ( ");
  sDisplayText.appendNumber(m_calibData.touch.X3);
  sDisplayText.append(", ");
  sDisplayText.appendNumber(m_calibData.touch.Y3);
  sDisplayText.append(" )\n");
  if(m_Done)
  {
    sDisplayText.append("\n\n\n");
    sDisplayText.append("Matrix->A:   ");
    sDisplayText.appendNumber(m_CalibMatrix.A);
    sDisplayText.append(CcGlobalStrings::EolShort);
    sDisplayText.append("Matrix->B:   ");
    sDisplayText.appendNumber(m_CalibMatrix.B);
    sDisplayText.append(CcGlobalStrings::EolShort);
    sDisplayText.append("Matrix->C:   ");
    sDisplayText.appendNumber(m_CalibMatrix.C);
    sDisplayText.append(CcGlobalStrings::EolShort);
    sDisplayText.append("Matrix->D:   ");
    sDisplayText.appendNumber(m_CalibMatrix.D);
    sDisplayText.append(CcGlobalStrings::EolShort);
    sDisplayText.append("Matrix->E:   ");
    sDisplayText.appendNumber(m_CalibMatrix.E);
    sDisplayText.append(CcGlobalStrings::EolShort);
    sDisplayText.append("Matrix->F:   ");
    sDisplayText.appendNumber(m_CalibMatrix.F);
    sDisplayText.append(CcGlobalStrings::EolShort);
    sDisplayText.append("Matrix->Div: ");
    sDisplayText.appendNumber(m_CalibMatrix.Div);
    if (m_PosAbsolute.getX() > 0 || m_PosAbsolute.getY() > 0)
    {
      sDisplayText.append("\n\n");
      sDisplayText.append("Absolute: ( ");
      sDisplayText.appendNumber(m_PosAbsolute.getX());
      sDisplayText.append(", ");
      sDisplayText.appendNumber(m_PosAbsolute.getY());
      sDisplayText.append(" )\n");
      sDisplayText.append("Relative: ( ");
      sDisplayText.appendNumber(m_PosRelative.getX());
      sDisplayText.append(", ");
      sDisplayText.appendNumber(m_PosRelative.getY());
      sDisplayText.append(" )\n");
    }
  }
  m_TextWidget.setString(sDisplayText);
  m_TextWidget.drawString();
}

void CcCalibrationButton::onClick(const CcPoint& pos)
{
  CcPainter Painter(getHandle());
  Painter.setColor(0xff, 0, 0);
  if(m_buttonNr == 0)
  {
    m_calibData.touch.X1 = pos.getX();
    m_calibData.touch.Y1 = pos.getY();
    drawBackground(getStyle()->oBackgroundColor);
    drawText();
    Painter.drawCross(Pos2, m_cross);
    m_buttonNr++;
  }
  else if(m_buttonNr == 1)
  {
    m_calibData.touch.X2 = pos.getX();
    m_calibData.touch.Y2 = pos.getY();
    drawBackground(getStyle()->oBackgroundColor);
    drawText();
    Painter.drawCross(Pos3, m_cross);
    m_buttonNr++;
  }
  else if(m_buttonNr == 2)
  {
    m_buttonNr++;
    m_Done=true;
    m_calibData.touch.X3 = pos.getX();
    m_calibData.touch.Y3 = pos.getY();
    // All values connected start calculation
    calcCalibration();
    /*m_onDone.call();
    m_buttonNr++;*/
    drawBackground(getStyle()->oBackgroundColor);
    drawText();
  }
  else{
    m_PosAbsolute.setPoint(pos);
    CcPoint sim = simulateCalibration(pos);
    m_PosRelative.setPoint(sim);
    drawBackground(getStyle()->oBackgroundColor);
    drawText();
  }
}

void CcCalibrationButton::fillCalibData()
{
  uint32 xSize = getWindow()->getWidth();
  uint32 ySize = getWindow()->getHeight();
  uint32 temp32;
  uint32 temp16X, temp16Y;
  CcStatic::memset(&m_calibData, 0, sizeof(m_calibData));
  //generate a Point up left
  Pos1.setPoint(30, 30);
  m_calibData.display.X1 = 30 + (m_cross.m_width  / 2) + getParent()->getPos().getX();
  m_calibData.display.Y1 = 30 + (m_cross.m_height / 2) + getParent()->getPos().getY();
  //generate a Point right middle
  temp32 = (xSize * 48);
  temp16X = (temp32 / 64) & 0xffff;
  temp16Y = (ySize / 2);
  Pos2.setPoint(temp16X, temp16Y);
  m_calibData.display.X2 = temp16X + (m_cross.m_width  / 2) + getParent()->getPos().getX();
  m_calibData.display.Y2 = temp16Y + (m_cross.m_height / 2) + getParent()->getPos().getY();
  //generate a Point down middle
  temp16X = (xSize / 2);
  temp32 = (ySize * 48);
  temp16Y = (temp32 / 64) & 0xffff;
  Pos3.setPoint(temp16X, temp16Y);
  m_calibData.display.X3 = temp16X + (m_cross.m_width  / 2) + getParent()->getPos().getX();
  m_calibData.display.Y3 = temp16Y + (m_cross.m_height / 2) + getParent()->getPos().getY();
}

void CcCalibrationButton::calcCalibration()
{
  int32 temp1 = ((m_calibData.touch.X1 - m_calibData.touch.X3) * (m_calibData.touch.Y2 - m_calibData.touch.Y3));
  int32 temp2 = ((m_calibData.touch.X2 - m_calibData.touch.X3) * (m_calibData.touch.Y1 - m_calibData.touch.Y3));
  m_CalibMatrix.Div = temp1-temp2;

  if(m_CalibMatrix.Div != 0)
  {
    m_CalibMatrix.A = ((m_calibData.display.X1 - m_calibData.display.X3) * (m_calibData.touch.Y2   - m_calibData.touch.Y3   )) -
                      ((m_calibData.display.X2 - m_calibData.display.X3) * (m_calibData.touch.Y1   - m_calibData.touch.Y3   ));

    m_CalibMatrix.B = ((m_calibData.touch.X1   - m_calibData.touch.X3  ) * (m_calibData.display.X2 - m_calibData.display.X3 )) -
                      ((m_calibData.display.X1 - m_calibData.display.X3) * (m_calibData.touch.X2   - m_calibData.touch.X3   ));

    m_CalibMatrix.C = (((m_calibData.touch.X3 * m_calibData.display.X2) - (m_calibData.touch.X2 * m_calibData.display.X3)) * m_calibData.touch.Y1) +
                      (((m_calibData.touch.X1 * m_calibData.display.X3) - (m_calibData.touch.X3 * m_calibData.display.X1)) * m_calibData.touch.Y2) +
                      (((m_calibData.touch.X2 * m_calibData.display.X1) - (m_calibData.touch.X1 * m_calibData.display.X2)) * m_calibData.touch.Y3) ;

    m_CalibMatrix.D = ((m_calibData.display.Y1 - m_calibData.display.Y3) * (m_calibData.touch.Y2   - m_calibData.touch.Y3)) -
                      ((m_calibData.display.Y2 - m_calibData.display.Y3) * (m_calibData.touch.Y1   - m_calibData.touch.Y3)) ;

    m_CalibMatrix.E = ((m_calibData.touch.X1   - m_calibData.touch.X3  ) * (m_calibData.display.Y2 - m_calibData.display.Y3)) -
                      ((m_calibData.display.Y1 - m_calibData.display.Y3) * (m_calibData.touch.X2   - m_calibData.touch.X3  )) ;

    m_CalibMatrix.F = (((m_calibData.touch.X3   * m_calibData.display.Y2) - (m_calibData.touch.X2 * m_calibData.display.Y3)) * m_calibData.touch.Y1) +
                      (((m_calibData.touch.X1   * m_calibData.display.Y3) - (m_calibData.touch.X3 * m_calibData.display.Y1)) * m_calibData.touch.Y2) +
                      (((m_calibData.touch.X2   * m_calibData.display.Y1) - (m_calibData.touch.X1 * m_calibData.display.Y2)) * m_calibData.touch.Y3) ;
  }
}

void CcCalibrationButton::registerOnDone(CcObject& oObject, uint8 nr)
{
  CCUNUSED(oObject);
  CCUNUSED(nr);
}

CcPoint CcCalibrationButton::simulateCalibration(CcPoint input)
{
  CcPoint Ret;
  uint32 x, y;
  x = (uint32)(((m_CalibMatrix.A * input.getX()) +
                (m_CalibMatrix.B * input.getY()) +
                 m_CalibMatrix.C) / m_CalibMatrix.Div);
  y = (uint32)(((m_CalibMatrix.D * input.getX()) +
                (m_CalibMatrix.E * input.getY()) +
                 m_CalibMatrix.F) / m_CalibMatrix.Div);
  Ret.setPoint(x, y);
  return Ret;
}
