/*
 * This file is part of CcShellWorker.
 *
 * CcShellWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcShellWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcShellWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcShellWorker
 */
#include "CcShellWorker.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"
#include "CcShell.h"
#include "CcBufferList.h"
#include "Package/CPackage.h"
#include "CcShellApp.h"

CcShellWorker::CcShellWorker(CcShellApp* pApplication, IIo* pIoStream, bool bRaw) :
  CcShell(pIoStream),
  m_pApplication(pApplication),
  m_pIoStream(pIoStream),
  m_bRaw(bRaw)
{

}

CcShellWorker::~CcShellWorker()
{
  if (m_pIoStream)
  {
    m_pIoStream->close();
    CCDELETE(m_pIoStream);
  }
}

void CcShellWorker::run()
{
  if (m_bRaw)
  {
    CcShell::run();
  }
  else
  {
    size_t uiLastRead = 0;
    CcByteArray oTransportBuffer;
    oTransportBuffer.resize(102400);
    CcBufferList oPackageBuffer;
    do
    {
      uiLastRead = m_pIoStream->readArray(oTransportBuffer, false);
      if (uiLastRead <= oTransportBuffer.size())
      {
        oPackageBuffer.append(oTransportBuffer.getArray(), uiLastRead);
        NShell::CPackage* pPackage = static_cast<NShell::CPackage*>(oPackageBuffer.getBuffer(sizeof(NShell::CPackage)));
        if(pPackage)
        {
          switch(pPackage->getType())
          {
            case NShell::CPackage::EType::Login:
              m_pApplication->getVersion();
              break;
            default:
              break;
          }
        }
      }
    } while (m_pIoStream && uiLastRead > 0 && uiLastRead < oTransportBuffer.size());
  }
}
