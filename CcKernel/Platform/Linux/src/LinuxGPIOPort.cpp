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
 * @file      LinuxGPIOPort
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class LinuxGPIOPort
 */
#include "LinuxGPIOPort.h"
#include "CcFile.h"

LinuxGPIOPort::LinuxGPIOPort() {
  // TODO Auto-generated constructor stub

}

LinuxGPIOPort::~LinuxGPIOPort() {
  // TODO Auto-generated destructor stub
}

void LinuxGPIOPort::init( void )
{

}

CcGPIOPin* LinuxGPIOPort::getPin(uint8 uiNr)
{
  CcGPIOPin* cRet = NULL;
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
    SGPIOPinItem stPin;
    stPin.uiNr = uiNr;
    stPin.cPin = new LinuxGPIOPin(uiNr);
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
        SGPIOPinItem stPin;
        stPin.uiNr = uiNr;
        stPin.cPin = new LinuxGPIOPin(uiNr);
        cRet = stPin.cPin;
        m_lcPins.append(stPin);
      }
    }
  }
  return cRet;
}
