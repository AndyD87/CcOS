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
 * @brief     Implementation of Class CcLinuxGpioPin
 */
#include "CcLinuxGpioPin.h"
#include "CcKernel.h"
#include "CcByteArray.h"

//const char c_sGpioClassPath[]         = "/sys/class/gpio/";
const char c_sGpioBasicPath[]         = "/sys/class/gpio/gpio";
//const char c_sGpioClassUnexportPath[] = "/sys/class/gpio/unexport";
const char c_sGpioClassExportPath[]   = "/sys/class/gpio/export";
const char c_sGpioAppendDirection[]   = "/direction";
const char c_sGpioAppendValue[]       = "/value";
const char c_sDirectionIn[]           = "in";
const char c_sDirectionOut[]          = "out";

CcLinuxGpioPin::CcLinuxGpioPin(uint8 nr)
{
  m_sPinNr = CcString::fromNumber(nr);
  m_sPinPath = c_sGpioBasicPath + m_sPinNr;
}

CcLinuxGpioPin::~CcLinuxGpioPin()
{

}

void CcLinuxGpioPin::init()
{

}

bool CcLinuxGpioPin::setDirection( EDirection eDirection)
{
  bool bRet =false;
  switch(eDirection)
  {
    case EDirection::Unknown:
    case EDirection::Alternate:
      {
        CcFile cFile(c_sGpioClassExportPath);
        if(cFile.exists())
        {
          if(cFile.open(EOpenFlags::Write))
          {
            if(cFile.write(m_sPinNr.getCharString(), m_sPinNr.length()) != SIZE_MAX)
              bRet = true;
            else
              CCERROR("Unable to write to gippin" + m_sPinPath);
            cFile.close();
          }
          else
          {
            CCERROR("Unable to open gpio-unexport");
          }
        }
        else
        {
          CCERROR("GPIO-Class not existing in system.");
        }
      }
      break;
    case EDirection::Input:
      {
        if(writeInput())
        {
          bRet = true;
        }
      }
      break;
    case EDirection::Output:
      {
        if(writeOutput())
        {
          bRet = true;
        }
      }
      break;
    default:
      break;
  }
  return bRet;
}

IGpioPin::EDirection CcLinuxGpioPin::getDirection()
{
  IGpioPin::EDirection eRet = IGpioPin::EDirection::Unknown;
  CcFile cFile(m_sPinPath + c_sGpioAppendValue);
  if(cFile.exists())
  {
    if(cFile.open(EOpenFlags::Read))
    {
      CcByteArray cContent = cFile.readAll();
      CcString sContent(cContent);
      if(sContent.compare("in", ESensitivity::CaseInsensitiv))
      {
        eRet = IGpioPin::EDirection::Input;
      }
      else if(sContent.compare("out", ESensitivity::CaseInsensitiv))
      {
        eRet = IGpioPin::EDirection::Output;
      }
      cFile.close();
    }
    else
    {
      CCERROR("Unable to open gpio-direction");
    }
  }
  else
  {
    CCERROR("GPIO Pin not available");
  }
  return eRet;
}

void CcLinuxGpioPin::setValue(bool bValue)
{
  CcFile cFile(m_sPinPath + c_sGpioAppendValue);
  if(cFile.exists())
  {
    if(cFile.open(EOpenFlags::Write))
    {
      char cValue = '0';
      if(bValue)
        cValue = '1';
      if(SIZE_MAX == cFile.write(&cValue, 1))
        CCERROR("Unable to write to " + m_sPinPath);
      cFile.close();
    }
    else
    {
      CCERROR("Unable to open gpio-value");
    }
  }
  else
  {
    CCERROR("GPIO Pin not available");
  }
}

bool CcLinuxGpioPin::getValue()
{
  bool bRet = false;
  CcFile cFile(m_sPinPath + c_sGpioAppendValue);
  if(cFile.exists())
  {
    if(cFile.open(EOpenFlags::Read))
    {
      char cValue = '0';
      if(SIZE_MAX != cFile.read(&cValue, 1))
      {
        if(cValue == '1')
          bRet = 1;
      }
      else
        CCERROR("Unable to read from GPIO" + m_sPinPath);
      cFile.close();
    }
    else
    {
      CCERROR("Unable to open gpio-value");
    }
  }
  else
  {
    CCERROR("GPIO Pin not available");
  }
  return bRet;
}

bool CcLinuxGpioPin::toggle()
{
  bool bReturn = true;
  setValue(!getValue());
  return bReturn;
}

bool CcLinuxGpioPin::writeOutput()
{
  bool bRet = false;
  CcFile cFile(m_sPinPath + c_sGpioAppendDirection);
  if(cFile.exists())
  {
    if(cFile.open(EOpenFlags::Write))
    {
      if(cFile.write((char*)c_sDirectionOut, sizeof(c_sDirectionOut)) != SIZE_MAX)
        bRet = true;
      else
        CCERROR("Unable to write to " + m_sPinPath);
      cFile.close();
    }
    else
    {
      CCERROR("Unable to open gpio-unexport");
    }
  }
  else
  {
    CCERROR("GPIO-Class not existing in system.");
  }
  return bRet;
}


bool CcLinuxGpioPin::writeInput()
{
  bool bRet = false;
  CcFile cFile(m_sPinPath + c_sGpioAppendDirection);
  if(cFile.exists())
  {
    if(cFile.open(EOpenFlags::Write))
    {
      if(cFile.write((char*)c_sDirectionIn, sizeof(c_sDirectionIn)) != SIZE_MAX)
        bRet = true;
      else
        CCERROR("Unable to write to " + m_sPinPath);
      cFile.close();
    }
    else
    {
      CCERROR("Unable to open gpio-unexport");
    }
  }
  else
  {
    CCERROR("GPIO-Class not existing in system.");
  }
  return bRet;
}
