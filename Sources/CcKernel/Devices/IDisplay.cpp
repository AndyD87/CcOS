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
 * @brief     Class IDisplay
 */
#include "Devices/IDisplay.h"

int32 IDisplay::writeLine(size_t uiLine, const CcString& sText, const SFontRectangle** pFont)
{
  int32 iLinesWritten = 1;
  int32 uiX = 0;
  for(const char& pcSign : sText)
  {
    if(pcSign > 0)
    {
      const SFontRectangle* pSign = pFont[static_cast<uint32>(pcSign)];
      int32 uiHeight = pSign->uiHeight + pSign->uiHeight/4;
      int32 uiWidth  = pSign->uiWidth  + pSign->uiWidth /2;
      if(uiX + uiWidth > getWidth())
      {
        uiLine++;
        iLinesWritten++;
        uiX=0;
      }
      for(uint8_t i=0;i<pSign->uiHeight;i++)
      {
        for(uint8_t j=0;j<pSign->uiWidth;j++)
        {
            setPixel(uiX+ j, (uiLine*uiHeight)+i, pSign->getPixel(j,i));
        }
      }
      uiX += uiWidth;
    }
  }
  return iLinesWritten;
}