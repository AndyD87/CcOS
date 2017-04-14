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
* @brief define macros required in CcMedia
*/

#ifdef WIN32
#ifdef CcMedia_EXPORTS
#define CcMediaSHARED __declspec(dllexport)
#else
  #ifdef _WINDLL
    #define CcMediaSHARED __declspec(dllimport)
  #else
    #define CcMediaSHARED
  #endif
#endif
#else
#define CcMediaSHARED
#endif