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
 * @subpage   CcCalibrationButton
 *
 * @page      CcCalibrationButton
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcCalibrationButton
 */
#ifndef H_CCCALIBRATIONBUTTON_H_
#define H_CCCALIBRATIONBUTTON_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcButton.h"
#include "CcCross.h"
#include "Types/CcPoint.h"
#include "Devices/ITouch.h"
#include "CcText.h"

/**
 * @brief Structure of Data defined for calculating a 3-Point Calibration
 */
typedef struct{
  struct {
    int32 X1;  //!< X value from first Cross on Display for calibration
    int32 Y1;  //!< Y value from first Cross on Display for calibration
    int32 X2;  //!< X value from second Cross on Display for calibration
    int32 Y2;  //!< Y value from second Cross on Display for calibration
    int32 X3;  //!< X value from third Cross on Display for calibration
    int32 Y3;  //!< Y value from third Cross on Display for calibration
  } display;
  struct {
    int32 X1;  //!< X value from first Cross on Display for calibration
    int32 Y1;  //!< Y value from first Cross on Display for calibration
    int32 X2;  //!< X value from second Cross on Display for calibratio
    int32 Y2;  //!< Y value from second Cross on Display for calibratio
    int32 X3;  //!< X value from third Cross on Display for calibration
    int32 Y3;  //!< Y value from third Cross on Display for calibration
  } touch;
} STouchCalibrationData;

class CcGuiSHARED CcCalibrationButton : public CcButton
{
public:
  CcCalibrationButton(const CcWidgetHandle& rParent);
  virtual ~CcCalibrationButton() override;

  virtual void draw(bool bDoFlush = true) override;
  void drawButton();
  void drawText();
  void fillCalibData();
  void calcCalibration();
  void registerOnDone(CcObject& oObject, uint8 nr);
  void onClick(const CcPoint& pos);
  CcPoint simulateCalibration(CcPoint input);

private:
  CcText  m_TextWidget;
  CcCross m_cross;
  uint8   m_buttonNr;
  CcPoint   m_PosAbsolute;
  CcPoint   m_PosRelative;
  CcPoint   Pos1;
  CcPoint   Pos2;
  CcPoint   Pos3;
  STouchCalibrationData m_calibData;
  STouchMatrix m_CalibMatrix;
  bool    m_Done;
};

#endif // H_CcCALIBRATIONBUTTON_H_
