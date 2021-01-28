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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxGpioPort
 */
#include "CcLinuxGpioPort.h"
#include "CcFile.h"

IGpioPin* CcLinuxGpioPort::getPin(uint8 uiNr)
{
  IGpioPin* cRet = NULL;
  for(size_t i=0; i < m_lcPins.size(); i++)
  {
    if(m_lcPins.at(i).uiNr == uiNr)
    {
      return m_lcPins.at(i).cPin;
    }
  }
  CcFile fGpioFile("/sys/class/gpio/gpio" + CcString::fromNumber(uiNr));
  CcString sGpioExport("/sys/class/gpio/export");
  if(fGpioFile.isDir())
  {
    CGpioPinItem stPin;
    stPin.uiNr = uiNr;
    CCNEW(stPin.cPin, CcLinuxGpioPin, uiNr);
    cRet = stPin.cPin;
    m_lcPins.append(stPin);
  }
  else if(CcFile::exists(sGpioExport))
  {
    CcFile cExportFile(sGpioExport);
    if(cExportFile.open(EOpenFlags::Write))
    {
      CcString sExportNr = CcString::fromNumber(uiNr);
      cExportFile.write(sExportNr.getCharString(), sExportNr.length());
      cExportFile.close();
      if(fGpioFile.isDir())
      {
        CGpioPinItem stPin;
        stPin.uiNr = uiNr;
        CCNEW(stPin.cPin, CcLinuxGpioPin, uiNr);
        cRet = stPin.cPin;
        m_lcPins.append(stPin);
      }
    }
  }
  return cRet;
}

bool CcLinuxGpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
{
  CCUNUSED(uiValue);
  bool bSuccess = true;
  for(int i = 0; i < count(); i++)
  {
    if((1 << i) & uiPinMask)
    {
      getPin(i)->setDirection(eDirection);
    }
  }
  return bSuccess;
}

bool CcLinuxGpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
{
  getPin(uiPin)->setDirection(eDirection);
  CCUNUSED(uiValue);
  return true;
}

IGpioPin::EDirection CcLinuxGpioPort::getDirection(size_t uiPin)
{
  return getPin(uiPin)->getDirection();
}

bool CcLinuxGpioPort::setValue(size_t uiPin, bool bValue)
{
  if(uiPin < count())
  {
    getPin(uiPin)->setValue(bValue);
    return true;
  }
  return false;
}

bool CcLinuxGpioPort::getValue(size_t uiPin)
{
  return getPin(uiPin)->getValue();
}
