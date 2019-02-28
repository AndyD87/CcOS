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
 * @brief     Implementation of Class CcGenericSubSystem
 */
#include "CcGenericSubSystem.h"
#include "CcKernel.h"
#include "CcWindow.h"

CcGuiSubSystem* CcGuiSubSystem::create(const CcWindowHandle& hWindow)
{
  return new CcGenericSubSystem(hWindow);
}

class CcGenericSubSystemPrivate
{
public:
  CcHandle<IDisplay> m_hDisplay;

  int16 m_DrawXStart;
  int16 m_DrawYStart;
  uint16 m_DrawXSize;
  uint16 m_DrawYSize;
  uint16 m_CursorX;
  uint16 m_CursorY;
};

CcGenericSubSystem::CcGenericSubSystem(const CcWindowHandle &hWindowHandle):
  CcGuiSubSystem(hWindowHandle)
{
  m_pPrivate = new CcGenericSubSystemPrivate();
}

CcGenericSubSystem::~CcGenericSubSystem()
{
}

CcStatus CcGenericSubSystem::open( )
{
  CcStatus oStatus;
  return oStatus;
}

CcStatus CcGenericSubSystem::close(void)
{
  return true;
}

void CcGenericSubSystem::loop(void)
{
  bool bDoRun = true;
  while (bDoRun)
  {
    bDoRun = false;
  }
}

void CcGenericSubSystem::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CCUNUSED(oPixel);
  CCUNUSED(uiNumber);
  uint32 uiTemp = (m_pPrivate->m_CursorY + m_pPrivate->m_DrawYStart) * getWindowHandle()->getWidth();
  uiTemp += m_pPrivate->m_CursorX + m_pPrivate->m_DrawXStart;
}

bool CcGenericSubSystem::setPixelArea(const CcRectangle& oArea)
{
  CCUNUSED(oArea);
  return false;
}

void CcGenericSubSystem::draw()
{
  // @todo flush data to display
}

void CcGenericSubSystem::getMaxArea(CcRectangle& oArea)
{
  CCUNUSED(oArea);
  // @todo flush data to display
}

void CcGenericSubSystem::hide()
{

}

void CcGenericSubSystem::show()
{

}

bool CcGenericSubSystem::hasFrame()
{
  return true;
}

CcRectangle CcGenericSubSystem::getInnerRect()
{
  // @todo Get Rectange from X11 Window
  CcRectangle oRect;
  return oRect;
}

void CcGenericSubSystem::setWindowTitle(const CcString& sTitle)
{
  // @todo Write Window Title to X11
  CCUNUSED(sTitle);

}

bool CcGenericSubSystem::setWindowState(EWindowState eState)
{
  bool bRet = false;
  // @todo Windowstate to X11
  CCUNUSED(eState);
  return bRet;
}

CcSubSysHandle CcGenericSubSystem::getHandle()
{
  // @todo return a common X11 widget
  return CcSubSysHandle();
}
void CcGenericSubSystem::updateSize()
{

}

void CcGenericSubSystem::updatePos()
{

}
