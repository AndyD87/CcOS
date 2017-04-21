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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcShell
 */

#ifndef CCSHELL_H_
#define CCSHELL_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcApp.h"
#include "CcFile.h"


#ifdef WIN32
#ifdef CcShell_EXPORTS
#define CcShellSHARED __declspec(dllexport)
#else
#define CcShellSHARED __declspec(dllimport)
#endif
#else
#define CcShellSHARED
#endif

class CcShellSHARED CcShell : public CcApp
{
public:
  CcShell();
  CcShell(CcIODevice*in, CcIODevice *out);
  virtual ~CcShell();

  void run(void);

  void parseLine(const CcString& line);

  void addApp(CcApp* pApp, const CcString& Name);

  void setWorkingDir(const CcString& path);

  void setInput(CcIODevice *pInput);
  void setOutput(CcIODevice *pOutput);

private:
  CcIODevice *m_Input;
  CcIODevice *m_Output;
  CcString m_WorkingDirectory;
};

#endif /* CCSHELL_H_ */
