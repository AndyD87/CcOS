/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGuiSubsystem
 */
#include "CcGuiSubsystem.h"
#include "CcKernel.h"
#include "CcWindow.h"

class CcGuiSubsystem::CPrivate
{
public:
};

CcGuiSubsystem::CcGuiSubsystem(CcWindow* hWindowHandle) : m_hWindow(hWindowHandle)
{
  if(m_pPrivate == nullptr)
    CCNEW(m_pPrivate, CPrivate);
}

CcGuiSubsystem::~CcGuiSubsystem()
{
  CCDELETE(m_pPrivate);
}

void CcGuiSubsystem::loop()
{
  bool bDoRun = true;
  while (bDoRun)
  {
    bDoRun = false;
  }
}

void CcGuiSubsystem::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CCUNUSED(oPixel);
  CCUNUSED(uiNumber);
}

bool CcGuiSubsystem::setPixelArea(const CcRectangle& oArea)
{
  CCUNUSED(oArea);
  return false;
}

void CcGuiSubsystem::draw()
{
  // @todo flush data to display
}

void CcGuiSubsystem::getMaxArea(CcRectangle& oArea)
{
  CCUNUSED(oArea);
  // @todo flush data to display
}

void CcGuiSubsystem::hide()
{

}

void CcGuiSubsystem::show()
{

}

bool CcGuiSubsystem::hasFrame()
{
  return true;
}

CcRectangle CcGuiSubsystem::getInnerRect()
{
  // @todo Get Rectange from X11 Window
  CcRectangle oRect;
  return oRect;
}

void CcGuiSubsystem::setWindowTitle(const CcString& sTitle)
{
  // @todo Write Window Title to X11
  CCUNUSED(sTitle);

}

bool CcGuiSubsystem::setWindowState(EWindowState eState)
{
  bool bRet = false;
  // @todo Windowstate to X11
  CCUNUSED(eState);
  return bRet;
}

void CcGuiSubsystem::updateSize()
{

}

void CcGuiSubsystem::updatePos()
{

}
