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
#pragma once

#include "CcBase.h"
#include "CcObject.h"
#include "IIo.h"
#include "CcStringList.h"
#include "Devices/CcDeviceDisplay.h"

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED CcGenericConsole : public IIo
{
public:
  /**
   * @brief Create an async interface for specific io device
   * @param pIoDevice: Target io device to make async
   */
  CcGenericConsole(const CcDeviceDisplay& oDisplay, const SFontRectangle* pFont) : 
    m_oDisplay(oDisplay),
    m_pFont(pFont)
  {}

  /**
   * @brief Destructor
   */
  virtual ~CcGenericConsole();

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t readHidden(void* pBuffer, size_t uSize);
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  virtual CcStatus ioControl(uint32 cmd, const void *pInArg = nullptr, size_t uiInSize = 0, void *pOutArg = nullptr, size_t uiOutSize = 0, size_t* puiWritten = nullptr) override;
  virtual void* getStdFile() override;
  virtual CcStatus flush() override;

private:
  int32 writeLine(size_t uiLine, const CcString& sText, const SFontRectangle** pFont);
  int32 linesRequired(const CcString& sText, const SFontRectangle** pFont);
private:
  CcDeviceDisplay       m_oDisplay;
  const SFontRectangle* m_pFont;
  CcStringList          m_oOutputBuffer;

  int32                 m_iLineHeight;
  int32                 m_iMaxLines;
  int32                 m_iFirstLineOffset;
};
