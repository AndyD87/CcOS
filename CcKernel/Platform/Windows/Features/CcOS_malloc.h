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
 * @page      Generic
 * @subpage   malloc
 *
 * @page      malloc
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Declarations for malloc and new
 */
#ifndef H_CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_
#define H_CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_

#include "CcBase.h"

void* malloc(size_t size);
void free(void*) NOEXCEPT;

CCEXTERNC float _fltused;

#endif // H_CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_
