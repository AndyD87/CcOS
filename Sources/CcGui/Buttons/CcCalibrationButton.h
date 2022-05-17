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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcCalibrationButton
 */
#pragma once

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
  } display; //!< Display calibration
  struct {
    int32 X1;  //!< X value from first Cross on Display for calibration
    int32 Y1;  //!< Y value from first Cross on Display for calibration
    int32 X2;  //!< X value from second Cross on Display for calibratio
    int32 Y2;  //!< Y value from second Cross on Display for calibratio
    int32 X3;  //!< X value from third Cross on Display for calibration
    int32 Y3;  //!< Y value from third Cross on Display for calibration
  } touch; //!< Touch calibration
} STouchCalibrationData;

/**
 * @brief This button should overlay the whole display and by touching the crosses
 *        on touch display calculate the address transformation to calibrate the display and touch
 *        combination.
 */
class CcGuiSHARED CcCalibrationButton : public CcButton
{
public:
  /**
   * @brief Parent (Display) Widget to overlay.
   * @param pParent: Parten to overlay
   */
  CcCalibrationButton(CcWidget* pParent);
  virtual ~CcCalibrationButton() override;

  /**
   * @brief Override draw method to draw the crosses
   * @param bDoFlush: Instant write out if true
   */
  virtual void draw(bool bDoFlush = true) override;

  /**
   * @brief Draw button background on @ref draw
   */
  void drawButton();

  /**
   * @brief Draw text on @ref draw
   */
  void drawText();

  /**
   * @brief Setup calibration data by parent window
   */
  void fillCalibData();

  /**
   * @brief Calculate calibration from stored cross touch results
   */
  void calcCalibration();

  /**
   * @brief Override on click to register all pressed crosses addresses.
   *        It will step by step show all crosses and finally start the calculation.
   * @param pos
   */
  void onClick(const CcPoint& pos);

  /**
   * @brief Simulate Calibration result by translate incoming point with calculated
   *        transform table.
   * @param oInput: Point to translate
   * @return Translated point
   */
  CcPoint simulateCalibration(const CcPoint& oInput);

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
