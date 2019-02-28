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
 * @brief     Implementation of Class CcLinuxLed
 */

#include "CcLinuxLed.h"
#include "CcFile.h"
#include "stdio.h"

CcLinuxLed::CcLinuxLed(const CcString &Path) :
  m_Path(Path)
{
  CcString filename("max_brightness");
  CcFile file(m_Path.appendPath(filename));
  if(file.open(EOpenFlags::Read)){
    CcString Temp;
    char buf[20];
    size_t read;
    read = file.read(buf, 20);
    file.close();
    Temp.append(buf, read);
    m_MaxBrightness = Temp.toUint16();
  }
}

CcLinuxLed::~CcLinuxLed()
{
  // TODO Auto-generated destructor stub
}

CcStatus CcLinuxLed::setMaxBirghtness(uint16 brightness)
{
  CcStatus oSuccess = false;
  CcString filename("brightness");
  CcFile file(m_Path.appendPath(filename));
  if(file.open(EOpenFlags::Write))
  {
    oSuccess = true;
    CcString Temp;
    Temp.appendNumber(brightness);
    file.write(Temp.getCharString(), Temp.length());
    file.close();
  }
  return oSuccess;
}

CcStatus CcLinuxLed::setToggleTime(uint16 onTime, uint16 offTime)
{
  CcStatus oSuccess = false;
  CcString filename("delay_on");
  CcFile fileOn(m_Path.appendPath(filename));
  if(fileOn.open(EOpenFlags::Write))
  {
    CcString Temp;
    Temp.appendNumber(onTime);
    fileOn.write(Temp.getCharString(), Temp.length());
    fileOn.close();
    filename = "delay_off";
    CcFile fileOff(m_Path.appendPath(filename));
    if(fileOff.open(EOpenFlags::Write))
    {
      oSuccess = true;
      CcString Temp;
      Temp.appendNumber(offTime);
      fileOff.write(Temp.getCharString(), Temp.length());
      fileOff.close();
    }
  }
  return oSuccess;
}

CcStatus CcLinuxLed::on(uint16 brightness)
{
  CcStatus oSuccess = false;
  CcString filename("brightness");
  if(brightness == 0)
  {
    CcFile file(m_Path.appendPath(filename));
    if(file.open(EOpenFlags::Write))
    {
      oSuccess = true;
      CcString Temp;
      Temp.appendNumber(brightness);
      file.write(Temp.getCharString(), Temp.length());
      file.close();
    }
  }
  else
  {
    CcFile file(m_Path.appendPath(filename));
    if(file.open(EOpenFlags::Write))
    {
      oSuccess = true;
      CcString Temp;
      Temp.appendNumber(brightness);
      file.write(Temp.getCharString(), Temp.length());
      file.close();
    }
  }
  return oSuccess;
}

CcStatus CcLinuxLed::off(void)
{
  CcStatus oSuccess = false;
  CcString filename("brightness");
  CcFile file(m_Path.appendPath(filename));
  if(file.open(EOpenFlags::Write))
  {
    oSuccess = true;
    CcString Temp("0");
    file.write(Temp.getCharString(), Temp.length());
    file.close();
  }
  return oSuccess;
}

CcStatus CcLinuxLed::toggle()
{
  CcStatus oSuccess = false;
  CcString filename("toggle");
  CcFile file(m_Path.appendPath(filename));
  if(file.open(EOpenFlags::Write))
  {
    oSuccess = true;
    CcString Temp("1");
    file.write(Temp.getCharString(), Temp.length());
    file.close();
  }
  return oSuccess;
}
