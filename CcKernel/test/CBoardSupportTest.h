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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */
#pragma once

#include "CcBase.h"
#include "CcTest.h"

class CcBoardSupportSimulation;

/**
 * @brief Class implementation
 */
class CBoardSupportTest : public CcTest<CBoardSupportTest>
{
public:
  /**
   * @brief Constructor
   */
  CBoardSupportTest();

  /**
   * @brief Destructor
   */
  virtual ~CBoardSupportTest();

private:
  CcBoardSupportSimulation* m_pSimulation = nullptr;

private:
  bool testSimulationSetup();
  bool testVerifySetup();
  bool testCreateDevices();
  bool testSimulationRemoving();
};
