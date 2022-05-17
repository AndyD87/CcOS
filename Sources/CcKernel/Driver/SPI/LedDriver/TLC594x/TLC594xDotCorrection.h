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
 * @brief     Class TLC594xDotCorrection
 **/
#pragma once

#include "CcBase.h"
#include "CcByteArray.h"

/**
 * @brief Class impelmentation
 */
class TLC594xDotCorrection
{
public:
  /**
   * @brief Set number of chips for dot correction size
   * @param uiNumberOfChips: Number of chips in row
   */
  void setChipCount(size_t uiNumberOfChips);

  /**
   * @brief Set brightnes of specific LED as PWM
   * @param uiLedNr:      Indes of led for brightness
   * @param uiBrightness: PWM value between 0x0 and 0xffff for brigthness
   */
  void setLedBrightness(size_t uiLedNr, uint16 uiBrightness);

private:
  size_t      m_uiChipCount = 0;
  CcByteArray m_oData;
};
