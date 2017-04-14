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
 * @page      CcHttp
 * @subpage   CcHttpClient
 * @subpage   CcHttpDefaultProvider
 * @subpage   CcHttpProvider
 * @subpage   CcHttpRequest
 * @subpage   CcHttpResponse
 * @subpage   CcHttpServer
 * @subpage   CcHttpServerWorker
 * @subpage   CcHttpTypes
 * @subpage   HttpProvider
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 */

#ifdef WIN32
#ifdef CcHttp_EXPORTS
#define CcHttpSHARED __declspec(dllexport)
#else
#define CcHttpSHARED __declspec(dllimport)
#endif
#else
#define CcHttpSHARED
#endif

#ifdef CCSSL_ENABLED
  #define SSL
#endif