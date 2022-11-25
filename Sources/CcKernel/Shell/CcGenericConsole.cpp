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
 * @brief     Class CcGenericConsole
 */

#include "CcGenericConsole.h"

#include "IIo.h"
#include "CcConsole.h"

CcGenericConsole::~CcGenericConsole()
{
  close();
}

size_t CcGenericConsole::read(void* CCUNUSED_PARAM(pBuffer), size_t CCUNUSED_PARAM(uSize))
{
  size_t uiRet = SIZE_MAX;
  return uiRet;
}

size_t CcGenericConsole::readHidden(void* pBuffer, size_t uSize)
{
  size_t uiRet = read(pBuffer, uSize);
  return uiRet;
}

size_t CcGenericConsole::write(const void* pBuffer, size_t uSize)
{
  return CcConsole::write(pBuffer, uSize);
}

CcStatus CcGenericConsole::open(EOpenFlags)
{
  CcStatus oRet = false;
  if(m_pFont)
  {
    m_iLineHeight = m_pFont[0]->uiHeight + m_pFont[0]->uiHeight/4;
    m_iMaxLines = m_oDisplay.getDevice()->getHeight() / m_iLineHeight;
    m_iFirstLineOffset = m_oDisplay.getDevice()->getHeight() - (m_iLineHeight * m_iMaxLines);
    oRet = true;
  }
  return oRet;
}

CcStatus CcGenericConsole::close()
{
  CcStatus oRet = false;
  return oRet;
}

CcStatus CcGenericConsole::cancel()
{
  CcStatus oRet = EStatus::AllOk;
  return oRet;
}

CcStatus CcGenericConsole::ioControl(uint32 , const void *, size_t , void *, size_t , size_t* )
{
  CcStatus oRet = EStatus::NotSupported;
  return oRet;
}

void* CcGenericConsole::getStdFile()
{
  return this;
}

CcStatus CcGenericConsole::flush()
{
  CcStatus oRet = EStatus::NotSupported;
  return oRet;
}


void CcGenericConsole::writeLine(const CcString& sLine)
{
  int32 uiX=0;
  size_t uiBegin = 0;
  size_t uiNext = 0;
  for(const char& pcSign : sLine)
  { 
    if(pcSign > 0)
    {
      const SFontRectangle* pSign = m_pFont[static_cast<uint32>(pcSign)];
      int32 uiWidth  = pSign->uiWidth  + pSign->uiWidth /2;
      if(uiX + uiWidth > m_oDisplay.getDevice()->getWidth())
      {
        if(m_oOutputBuffer.size() == m_iMaxLines)
        {
          m_oOutputBuffer.remove(0);
        }
        m_oOutputBuffer.append(sLine.substr(uiBegin, uiNext));
        uiBegin = uiNext;
        uiX=0;
      }
      uiX += uiWidth;
    }
    uiNext++;
  }
  if(uiNext > uiBegin)
  {
    if(m_oOutputBuffer.size() == m_iMaxLines)
    {
      m_oOutputBuffer.remove(0);
    }
    m_oOutputBuffer.append(sLine.substr(uiBegin, uiNext));
  }
  drawLines();
}

void CcGenericConsole::drawLines()
{
  m_oDisplay.getDevice()->fill(false);
  int32 iLine = 0;
  for(const CcString& sLine : m_oOutputBuffer)
  {
    int32 uiX = 0;
    int32 uiLineOffset = m_iFirstLineOffset + (iLine * m_iLineHeight);
    for(const char& pcSign : sLine)
    {
      if(pcSign > 0)
      {
        const SFontRectangle* pSign = m_pFont[static_cast<uint32>(pcSign)];
        int32 uiWidth  = pSign->uiWidth  + pSign->uiWidth /2;
        for(uint8_t i=0;i<pSign->uiHeight;i++)
        {
          for(uint8_t j=0;j<pSign->uiWidth;j++)
          {
              m_oDisplay.getDevice()->setPixel(uiX+ j, uiLineOffset + i, pSign->getPixel(j,i));
          }
        }
        uiX += uiWidth;
      }
    }
    iLine++;
  }
  m_oDisplay.getDevice()->draw();
}
