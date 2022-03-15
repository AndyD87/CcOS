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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlUtil
 *            This Class is a Static Class. It is used for Operations on XmlNodes.
 **/
#pragma once

#include "CcDocument.h"
#include "CcBase.h"
#include "CcXmlNode.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcXmlUtil 
{
public:
  /**
   * @brief Boolean is required, test if true or false can be found.
   *        All other values produce @p bSetIfInvalid.
   * @param oNode:          Xml node to parse for value
   * @param bSetIfInvalid:  Overwrite default value if bool not detected
   * @param pbOk:           If not null value will be set to true if
   *                        boolean value was detected
   * @return Detected value or @p bSetIfInvalid
   */
  static bool getBoolFromNodeValue(const CcXmlNode& oNode, bool bSetIfInvalid = false, bool* pbOk = nullptr);

  /**
   * @brief Get boolean string for bool
   * @param bTrueFalse: True or false string to set
   * @return Default string for boolean expected.
   */
  static const CcString& getStringFromBool(bool bTrueFalse);
};
