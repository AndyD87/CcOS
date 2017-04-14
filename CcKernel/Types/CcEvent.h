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
 * @page      CcEvent
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcEvent
 */
#ifndef CcEvent_H_
#define CcEvent_H_

//! Forward Declaration
#include "CcKernelBase.h"
#include "CcObject.h"

/**
 * @brief Class for writing Output to Log. Additionally it handles Debug and Verbose output
 */

template <typename TYPE, typename PARAMTYPE>
class CcEvent
{
public:
  typedef void (TYPE::*CallbackFunction)(PARAMTYPE* pParam);

  CcEvent(TYPE* oObject, CallbackFunction pFunc = NULL)
  {
    m_oObject = oObject;
    m_func = pFunc;
  }

  void call(PARAMTYPE* pParam)
  {
    (*m_oObject.*m_func)(pParam);
  }

  CcObject* getObject(){ return m_oObject; }

private:
  TYPE* m_oObject;
  CallbackFunction m_func;
};

#endif /* CcEvent_H_ */
