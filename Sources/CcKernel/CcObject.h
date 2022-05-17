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
 * @brief     Class CcObject
 */
#pragma once

#include "CcBase.h"

class CcEvent;
class CcEventHandler;

/**
 * @brief Basic Class for all Objects wich has receive callbacks
 */
class CcKernelSHARED  CcObject
{
public:
  //! Method type for call object methods from events
  typedef void (CcObject::*FObjectMethod)(void*);

  /**
   * @brief Constructor
   */
  CcObject() = default;

  /**
   * @brief Destructor
   */
  virtual ~CcObject();

  /**
   * @brief Call method from object pointer
   * @param pFunc:  Metho of inheriting class
   * @param pParam: Parameter to pass from sender to receiver
   */
  virtual void objectBaseCall(CcObject::FObjectMethod pFunc, void* pParam)
  { (this->*(pFunc))(pParam); }

  /**
   * @brief Register on delete for automatic removing connections on delete
   * @param pEventHandle: Handle to register to call on delete
   */
  void registerOnDelete(const CcEvent& pEventHandle);

  /**
   * @brief Remove object from on delete signaling.
   * @param pObject: Object to remove from
   */
  void deregisterOnDelete(CcObject* pObject);

protected:
  //! @ return EventHandler for on delete signaling
  CcEventHandler& getOnDeleteHandler();

private:
  void clear();
private:
  CcEventHandler* m_pOnDeleteHandler = nullptr;
};
