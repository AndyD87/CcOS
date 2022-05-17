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
 * @brief     Default Settings for different parts of CcOS Project
 */
#pragma once

#include "CcBase.h"

//! Use a default buffer size of 100k for reading and writing files.
#define CCOS_DEFAULT_FILE_RW_BUFFER_SIZE 102400

//! Use a default buffer size of 10k for reading and writing files.
#define CCOS_DEFAULT_IO_RW_BUFFER_SIZE    10240

//! Use a default buffer size of 1k for shared memory
#define CCOS_DEFAULT_SHARED_BUFFER_SIZE    1024

//! Use a default buffer size of 1k for shared memory
#define CCOS_DEFAULT_MAX_PATH              4096
