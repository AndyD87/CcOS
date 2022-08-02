/*
 * This file is part of CcShellAppWorker.
 *
 * CcShellAppWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcShellAppWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcShellAppWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcShellAppWorker
 */
#include "CcShellAppWorker.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"
#include "CcShell.h"

CcShellAppWorker::CcShellAppWorker(CcShellApp* pApplication, IIo* pIoStream) :
  CcShell(pIoStream),
  m_pApplication(pApplication),
  m_pIoStream(pIoStream)
{

}

CcShellAppWorker::~CcShellAppWorker()
{
  if (m_pIoStream)
  {
    m_pIoStream->close();
    CCDELETE(m_pIoStream);
  }
}
