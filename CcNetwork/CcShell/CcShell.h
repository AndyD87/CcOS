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
 * @page      CcNetwork
 * @subpage   CcShell
 *
 * @page      CcShell
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcShell
 */

#ifndef _CCSHELL_H_
#define _CCSHELL_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcApp.h"
#include "CcFile.h"

#ifdef WIN32
# ifndef CcShellSHARED
#   ifdef CcShell_EXPORTS
//    Cmake definition for shared build is set
#     define CcShellSHARED __declspec(dllexport)
#   elif defined CC_STATIC
//    CCOS will be build as static library no im-/export
#     define CcShellSHARED
#   else
//    if no definition found, we are on importing as dll
#     define CcShellSHARED __declspec(dllimport)
#   endif
# endif
#else
# define CcShellSHARED
#endif

class CcShellSHARED CcShell : public CcApp
{
public:
  CcShell();
  CcShell(CcIODevice*in, CcIODevice *out);
  virtual ~CcShell();

  void run();

  void parseLine(const CcString& line);

  void addApp(CcApp* pApp, const CcString& Name);

  void setWorkingDir(const CcString& path);

  void setInput(CcIODevice *pInput);
  void setOutput(CcIODevice *pOutput);

private:
  CcIODevice *m_Input;
  CcIODevice *m_Output;
  CcString    m_sWorkingDirectory;
};

#endif /* _CCSHELL_H_ */
