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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcProcess
 */
#include "CcProcess.h"
#include "CcKernel.h"

CcProcess::CcProcess(void) :
  m_Input(0),
  m_Output(0)
{
}

CcProcess::CcProcess( const CcString& sApplication) :
  m_sApplication(sApplication),
  m_Input(0),
  m_Output(0)
{
}

CcProcess::~CcProcess( void )
{
}

void CcProcess::start()
{
  CcKernel::createProcess(*this);
}

void CcProcess::setApplication(const CcString& sApplication)
{
  m_sApplication = sApplication;
}

void CcProcess::setArguments(const CcString& sArguments)
{
  m_Arguments.clear();
  m_Arguments.parseArguments(sArguments);
}

void CcProcess::setArguments(const CcStringList& slArguments)
{
  m_Arguments = slArguments;
}


const CcString& CcProcess::getApplication(void)
{
  return m_sApplication;
}

const CcStringList& CcProcess::getArguments(void)
{
  return m_Arguments;
}

void CcProcess::addArgument(const CcString& sArgument)
{
  m_Arguments.add(sArgument);
}

void CcProcess::clearArguments(void)
{
  m_Arguments.clear();
}

void CcProcess::setInput(CcIODevice *pInput)
{
  m_Input = pInput;
}

void CcProcess::setOutput(CcIODevice *pOutput)
{
  m_Output = pOutput;
}

CcIODevice* CcProcess::getInput(void)
{
  return m_Input;
}

CcIODevice* CcProcess::getOutput(void)
{
  return m_Output;
}
