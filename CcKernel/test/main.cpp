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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CKernelTest.h"
#include "CUuidTest.h"
#include "CFileTest.h"
#include "CStringTest.h"
#include "CTimeTest.h"
#include "CHashTest.h"
#include "CByteArrayTest.h"
#include "CProcessTest.h"
#include "CStatusTest.h"
#include "CSharedMemoryTest.h"
#include "CcConsole.h"
#include "CNetworkTest.h"
#include "CUrlTest.h"
#include "CVersionTest.h"

// Application entry point. 
int main(int argc, char **argv)
{
  bool bSuccess = true;
  CcKernel::setArg(argc, argv);
  CcConsole::writeLine("Start: CcKernelTest");
#ifdef DEBUG
  //CcKernel::initCLI();
#endif
  CKernelTest oKernelTest;
  bSuccess &= oKernelTest.test();
  CHashTest oHashTest;
  bSuccess &= oHashTest.test();
  CUuidTest oUuidTest;
  bSuccess &= oUuidTest.test();
  CStringTest oStringTest;
  bSuccess &= oStringTest.test();
  CFileTest oFileTest;
  bSuccess &= oFileTest.test();
  CTimeTest oTimeTest;
  bSuccess &= oTimeTest.test();
  CByteArrayTest oByteArrayTest;
  bSuccess &= oByteArrayTest.test();
  CProcessTest oProcessTest;
  bSuccess &= oProcessTest.test();
  CStatusTest oStatusTest;
  bSuccess &= oStatusTest.test();
  CSharedMemoryTest oSharedMemoryTest;
  bSuccess &= oSharedMemoryTest.test();
  CNetworkTest oNetworkTest;
  bSuccess &= oNetworkTest.test();
  CUrlTest oUrlTest;
  bSuccess &= oNetworkTest.test();
  CVersionTest oVersionTest;
  bSuccess &= oVersionTest.test();
  if (bSuccess)
    return 0;
  else
  {
    return 1;
  }
}
