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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcHttpWebframeworkCssProvider
 */
#include "CcHttpWebframeworkCssProvider.h"
#include "Resources/CcOSWebframework.css.h"
#include "CcHttpGlobalStrings.h"

using namespace NHttp::Application::RestApiWebframework;

const void* CcHttpWebframeworkCssProvider::getContent(size_t& Size)
{
  Size = CcOSWebframework_Css_Length;
  return CcOSWebframework_Css;
}

const CcString& CcHttpWebframeworkCssProvider::getMimeType()
{
  return CcHttpGlobalStrings::MimeTypes::TextCss;
}
