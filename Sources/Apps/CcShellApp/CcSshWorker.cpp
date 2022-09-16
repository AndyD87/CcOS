/*
 * This file is part of CcSshWorker.
 *
 * CcSshWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcSshWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcSshWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcSshWorker
 */
#include "CcSshWorker.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"
#include "CcShell.h"
#include "CcShellApp.h"

#include "Ssh/Types.h"

CcSshWorker::CcSshWorker(CcShellApp* pApplication, IIo* pIoStream) :
  CcShell(pIoStream),
  m_pApplication(pApplication),
  m_pIoStream(pIoStream)
{
  m_pApplication->getVersion();
}

CcSshWorker::~CcSshWorker()
{
  if (m_pIoStream)
  {
    m_pIoStream->close();
    CCDELETE(m_pIoStream);
  }
}

void CcSshWorker::run()
{
  size_t uiLastRead = 0;
  CcByteArray oTransportBuffer;
  oTransportBuffer.resize(102400);
  do
  {
    uiLastRead = m_pIoStream->readArray(oTransportBuffer, false);
    if (uiLastRead <= oTransportBuffer.size())
    {
      m_oPackage.append(oTransportBuffer.getArray(), uiLastRead);
      execPackage();
    }
  } while (m_pIoStream && uiLastRead > 0 && uiLastRead < oTransportBuffer.size());
}

void CcSshWorker::execPackage()
{
  switch (getState())
  {
    case EState::Startup:
      if (m_oPackage.startsWith("SSH-2.0", 7))
      {
        m_pIoStream->writeString("SSH-2.0-CcShell\r\n");
        setState(EState::Handshake);
        m_oPackage.clear();
      }
      break;
    case EState::Handshake:
    {
      CcSsh::packet* pPacket = m_oPackage.cast<CcSsh::packet>();
      pPacket->swap();
      // Check message id
      if (pPacket->getPayloadSize())
      {
        switch (static_cast<CcSsh::EMessage>(pPacket->getPayload()[0]))
        {
        case CcSsh::EMessage::SSH_MSG_KEXINIT:
        {
          CcSsh::key_exchange pKey(pPacket);

          m_pIoStream->writeArray(m_oPackage);
          m_oPackage.clear();
          break;
        }
        default:
          m_pIoStream->close();
        }
      }
      break;
    }
    case EState::Error:
      break;
  }
}
