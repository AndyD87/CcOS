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
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSingleton
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief First class to test singletons in a template.
 *        The singletons target is to get managed by CcKernel for destroying savely.
 *
 *        ATTENTION!
 *        Be aware of using singletons.
 *        It should be last resort if no other solution would work.
 */
template <typename TYPE>
class CcSingleton
{
public:
  /**
   * @brief Get instance from inheriting class.
   * @return Instance as pointer
   */
  static TYPE* instance ()
  {
     if (!s_pInstance)
        s_pInstance = CCNEW(TYPE);
     return s_pInstance;
  }

  /**
   * @brief Reset static instance handler
   */
  virtual ~CcSingleton()
  {
    s_pInstance = nullptr;
  }
protected:
  CcSingleton() = default;

private:
   static TYPE* s_pInstance; //!< Singleton instance, created with instance()
};

template<typename TYPE> TYPE* CcSingleton<TYPE>::s_pInstance = nullptr;
