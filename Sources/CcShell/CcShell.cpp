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
 * @brief     Implementation of Class CcShell
 */

#include "CcShell.h"
#include "CcKernel.h"
#include "IThread.h"
#include "CcByteArray.h"
#include "CcKernel.h"
#include "CcProcess.h"

CcShell::CcShell() :
  CcApp("CcShell"),
  m_pInput(nullptr),
  m_pOutput(nullptr)
{
}


CcShell::CcShell(IIo *pIn, IIo *pOut) :
  m_pInput(pIn),
  m_pOutput(pOut)
{
}

CcShell::~CcShell() {

}

void CcShell::run()
{
  char inBuf[256];
  size_t readSize;
  CcByteArray inData;
  // If no working directory set, get working directory from system
  if (m_sWorkingDirectory == "")
    setWorkingDir(CcKernel::getWorkingDir());
  /*m_pOutput->write("Login: ", 7);
  if (SIZE_MAX != (readSize = m_pInput->read(inBuf, 256)))
  {
    inData.append(inBuf, readSize);
    m_pOutput->write("Password: ", 10);
    if (SIZE_MAX != (readSize = m_pInput->read(inBuf, 256)))
    {
      inData.append(inBuf, readSize);
    }
  }*/
  m_pOutput->write(m_sWorkingDirectory.getCharString(), m_sWorkingDirectory.length());
  while (getThreadState() == EThreadState::Running)
  {
      CcString line;
      readSize = m_pInput->read(inBuf, 256);
      while (readSize != SIZE_MAX)
      {
        line.append(inBuf, readSize);
        if (line.find('\n'))
        {
          parseLine(line.trim());
          line.clear();
          m_pOutput->write(m_sWorkingDirectory.getCharString(), m_sWorkingDirectory.length());
        }
        readSize = m_pInput->read(inBuf, 256);
      }
  }
}

void CcShell::parseLine(const CcString& sLine)
{
  CcProcess process;
  CcStringList slArguments;
  CcString sCommand(slArguments.parseArguments(sLine));
  process.setApplication(sCommand);
  process.setArguments(slArguments);
  //process.setInput(*m_pInput);
  //process.setOutput(*m_pOutput);
  process.start();
}

void CcShell::addApp(CcApp* pApp, const CcString& sName)
{
  CCUNUSED(pApp);
  CCUNUSED(sName);
}

void CcShell::setWorkingDir(const CcString& sPath)
{
  m_sWorkingDirectory = sPath + " $ ";
}

void CcShell::setInput(IIo *pInput)
{
  m_pInput = pInput;
}

void CcShell::setOutput(IIo *pOutput)
{
  m_pOutput = pOutput;
}
