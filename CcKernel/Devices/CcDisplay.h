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
 * @page      Devices
 * @subpage   CcDisplay
 *
 * @page      CcDisplay
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDisplay
 */
#ifndef CCDISPLAY_H_
#define CCDISPLAY_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "Types/CcRectangle.h"

class CcDisplayArea;

class CcKernelSHARED CcDisplay : public CcIODevice {
public:
  CcDisplay(const CcSize& oSize);
  virtual ~CcDisplay();

  virtual size_t read(void*, size_t size) override
    { return size; }
  virtual size_t write(const void*, size_t size) override
    { return size; }
  virtual CcStatus open(EOpenFlags flags) = 0;
  virtual void setBackgroundLed( bool bState ) = 0;

  virtual CcDisplayArea* getDisplayArea(const CcRectangle& oArea) = 0;

  uint16 getWidth( void ) const
    {return m_oSize.getWidth();}
  uint16 getHeight( void ) const
    {return m_oSize.getHeight();}
  void nextCursor(void);

protected: //member
  CcSize m_oSize;
  CcPoint m_oCursor;
  uint16 m_DrawStartX = 0;
  uint16 m_DrawStartY = 0;
  uint16 m_DrawSizeX = 0;
  uint16 m_DrawSizeY = 0;
};

#endif /* CCDISPLAY_H_ */
