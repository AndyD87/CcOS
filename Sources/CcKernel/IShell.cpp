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
 * @brief     Class IShell
 */
#include "IShell.h"
#include "CcKernel.h"
#include "IIo.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"
#include "CcDirectory.h"

IShell::IShell() :
  m_sWorkingDir(CcGlobalStrings::Seperators::Slash)
{
  m_oTransferBuffer.resize(256);
}

IShell::~IShell()
{
}

void IShell::run()
{
  while(isRunning())
  {
    if(m_pIoStream)
    {
      readLine();
      // Handle the line
      CcStringList oArguments = CcStringUtil::getArguments(m_sRead);
      if(oArguments.size() > 0)
      {
        if(oArguments[0] == "cd")
        {
          if(oArguments.size() > 1)
          {
            if(changeDirectory(oArguments[1]))
            {
              m_pIoStream->writeLine("Path successfully changed", IIo::ELineEnding::CRNL);
            }
            else
            {
              m_pIoStream->writeLine("Path not found", IIo::ELineEnding::CRNL);
            }
          }
          else
          {
            m_pIoStream->writeLine("Additional parameter required", IIo::ELineEnding::CRNL);
          }
        }
        else
        {
          m_pIoStream->writeLine("Unknown command: " + oArguments[0], IIo::ELineEnding::CRNL);
        }
      }
      
      // Start with next line
      updatePrefix();
      m_sRead.clear();
      m_pIoStream->writeString(m_sPrefix);
    }
  }
}

void IShell::init(IIo* pIoStream)
{
  m_pIoStream = pIoStream;
}

CcStatus IShell::changeDirectory(const CcString& sPath)
{
  CcStatus oStatus;
  CcString sNewPath = m_sWorkingDir;
  CcStringUtil::setPath(sNewPath, sPath);
  if(CcDirectory::exists(sNewPath))
  {
    m_sWorkingDir = sNewPath;
  }
  else
  {
    oStatus = EStatus::FSFileNotFound;
  }
  return oStatus;
}

void IShell::readLine()
{
  size_t uiReceived = 0;
  if(m_oReadLineBuffer.size())
  {
    uiReceived = CCMIN(m_oReadLineBuffer.size(), m_oTransferBuffer.size());
    m_oTransferBuffer.write(m_oReadLineBuffer.getArray(), uiReceived);
    m_oReadLineBuffer.remove(0, uiReceived);
  }
  else
  {
    uiReceived = m_pIoStream->read(m_oTransferBuffer.getArray(), m_oTransferBuffer.size());
  }
  while( uiReceived < SIZE_MAX)
  {
    // echo read data

    // Interpret read data
    for(size_t uiPos =0 ; uiPos < uiReceived && uiReceived < SIZE_MAX; uiPos++)
    {
      switch (static_cast<uint8>(m_oTransferBuffer[uiPos]))
      {
        case 127: // del pressed
          if(m_sRead.size() > 0)
          {
            m_sRead.remove(m_sRead.size()-1);
            m_pIoStream->write(&m_oTransferBuffer[uiPos], 1);
          }
          break;
        case '\r':
          CCFALLTHROUGH;
        case '\n':
          uiPos++; // Ignore this char
          if(uiReceived > uiPos)
          {
            m_oReadLineBuffer.append(m_oTransferBuffer.getArray(uiPos), uiReceived - uiPos);
            uiPos = uiReceived;
          }
          else
          {
            uiReceived  = SIZE_MAX;
          }
          m_pIoStream->writeString(CcGlobalStrings::EolLong);
          break;
        default:
          m_sRead.append(m_oTransferBuffer[uiPos]);
          m_pIoStream->write(&m_oTransferBuffer[uiPos], 1);
          break;
      }
    }
    if(uiReceived != SIZE_MAX)
    {
      uiReceived = m_pIoStream->read(m_oTransferBuffer.getArray(), m_oTransferBuffer.size());
      CcKernel::sleep(10);
    }
  }
}

void IShell::updatePrefix()
{
  m_sPrefix = m_sWorkingDir;
  m_sPrefix << CcGlobalStrings::Seperators::Colon << CcGlobalStrings::Seperators::Space;
}
