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
 * @page      CcProcess
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcProcess
 */
#ifndef CcProcess_H_
#define CcProcess_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcIODevice.h"

/**
 * @brief Process-Connection to an external Process
 *        It can start stop them or @todo in future attach to one.
 */
class CcKernelSHARED CcProcess 
{
public:
  /**
   * @brief Constructor
   */
  CcProcess( void );

  /**
  * @brief Constructor
  */
  CcProcess(const CcString& sApplication);

  /**
   * @brief Destructor
   */
  virtual ~CcProcess( void );

  void start(void);

  void setApplication(const CcString& sApplication);
  void setArguments(const CcString& sArguments);
  void setArguments(const CcStringList& slArguments);
  void setInput(CcIODevice *pInput);
  void setOutput(CcIODevice *pOutput);
  void addArgument(const CcString& sArgument);
  void clearArguments(void);

  const CcString& getApplication(void);
  const CcStringList& getArguments(void);
  CcIODevice* getInput(void);
  CcIODevice* getOutput(void);

private:
  CcString m_sApplication;
  CcStringList m_Arguments;
  CcIODevice *m_Input;
  CcIODevice *m_Output;
};

#endif /* CcProcess_H_ */
