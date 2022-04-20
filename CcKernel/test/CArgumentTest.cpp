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
 * @brief     Implemtation of class CArgumentTest
 */
#include "CArgumentTest.h"
#include "CcArguments.h"
#include "CcStringStream.h"

CArgumentTest::CArgumentTest() :
  CcTest("CArgumentTest")
{
  appendTestMethod("Test basic callup", &CArgumentTest::testBasic);
  appendTestMethod("Test string conversion checks", &CArgumentTest::testFailedTypes);
  appendTestMethod("Test requirements", &CArgumentTest::testRequirements);
  appendTestMethod("Test print help", &CArgumentTest::testPrintHelp);
}

CArgumentTest::~CArgumentTest()
{
}

bool CArgumentTest::testBasic()
{
  bool bRet = false;
  CcArguments oArguments;
  oArguments.setVariablesList(
    {
      {"-int32",      CcVariant::EType::String},
      {"-request",    CcVariant::EType::String, "get"},
      {"-stream",     CcVariant::EType::String},
      {"-uint32",     CcVariant::EType::Uint32},
      {"-output",     CcVariant::EType::String}
    }
  );
  if (oArguments.parse(" -int32 https://coolcow.de Unparsed0 -request get -output \"Test File.html\" Unparsed1"))
  {
    if (oArguments.contains("-int32") &&
        oArguments.contains("-request") &&
        oArguments.contains("-output") &&
        oArguments.getUnparsed().size() == 2)
    {
      if (oArguments.getValue("-int32").getString() == "https://coolcow.de" &&
          oArguments.getValue("-request").getString() == "get" &&
          oArguments.getValue("-output").getString() == "Test File.html" &&
          oArguments.getUnparsed()[0] == "Unparsed0" &&
          oArguments.getUnparsed()[1] == "Unparsed1")
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CArgumentTest::testFailedTypes()
{
  bool bRet = false;
  CcArguments oArguments;
  oArguments.setVariablesList(
    {
      {"-int32",        CcVariant::EType::Int32},
      {"-uint32",       CcVariant::EType::Uint32},
      {"-int64",        CcVariant::EType::Int64},
      {"-uint64",       CcVariant::EType::Uint64},
      {"-int16",        CcVariant::EType::Int16},
      {"-uint16",       CcVariant::EType::Uint16},
      {"-int8",         CcVariant::EType::Int8},
      {"-uint8",        CcVariant::EType::Uint8},
      {"-request",    CcVariant::EType::String, "get"},
      {"-stream",     CcVariant::EType::Switch},
      {"-output",     CcVariant::EType::Bool}
    }
  );
  if (!oArguments.parse(" -int32 string"))
  {
    bRet = false;
    if (oArguments.parse(" -int32 20"))
    {
      if (!oArguments.parse(" -int32 20000000000"))
      {
        if (!oArguments.parse(" -int32 0x100000000"))
        {
          if (oArguments.parse(" -int32 -0x1") &&
              oArguments.getValue("-int32").isInt() &&
              oArguments.getValue("-int32").getInt32() == -1)
          {
            if (oArguments.parse(" -int32 -1"))
            {
              if (!oArguments.parse(" -int32 2147483648"))
              { 
                if (oArguments.parse(" -int32 -2147483648"))
                {
                  if (!oArguments.parse(" -int32 -2147483649"))
                  {
                    bRet = true;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (oArguments.parse(" -uint32 20"))
    {
      if (!oArguments.parse(" -uint32 20000000000"))
      {
        if (!oArguments.parse(" -uint32 0x100000000"))
        {
          if (oArguments.parse(" -uint32 0xffffffff") &&
              !oArguments.getValue("-uint32").isInt() &&
              oArguments.getValue("-uint32").getUint32() == UINT32_MAX)
          {
            if (!oArguments.parse(" -uint32 -1"))
            {
              bRet = true;
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (!oArguments.parse(" -int64 string"))
    {
      if (oArguments.parse(" -int64 20"))
      {
        if (!oArguments.parse(" -int64 19223372036854775807"))
        {
          if (!oArguments.parse(" -int64 0x10000000000000000"))
          {
            if (oArguments.parse(" -int64 -0x1") &&
                oArguments.getValue("-int64").isInt() &&
                oArguments.getValue("-int64").getInt64() == -1)
            {
              if (oArguments.parse(" -int64 -1"))
              {
                if (!oArguments.parse(" -int64 9223372036854775808"))
                {
                  if (oArguments.parse(" -int64 -9223372036854775808"))
                  {
                    if (!oArguments.parse(" -int64 -9223372036854775809"))
                    {
                      bRet = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (oArguments.parse(" -uint64 20"))
    {
      if (!oArguments.parse(" -uint64 19223372036854775807"))
      {
        if (!oArguments.parse(" -uint64 0x10000000000000000"))
        {
          if (oArguments.parse(" -uint64 0xffffffffffffffff") &&
              !oArguments.getValue("-uint64").isInt() &&
              oArguments.getValue("-uint64").getUint64() == UINT64_MAX)
          {
            if (!oArguments.parse(" -uint64 -1"))
            {
              bRet = true;
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (!oArguments.parse(" -int16 string"))
    {
      if (oArguments.parse(" -int16 20"))
      {
        if (!oArguments.parse(" -int16 165535"))
        {
          if (!oArguments.parse(" -int16 0x10000"))
          {
            if (oArguments.parse(" -int16 -0x1") &&
                oArguments.getValue("-int16").isInt() &&
                oArguments.getValue("-int16").getInt16() == -1)
            {
              if (oArguments.parse(" -int16 -1"))
              {
                if (!oArguments.parse(" -int16 32768"))
                {
                  if (oArguments.parse(" -int16 -32768"))
                  {
                    if (!oArguments.parse(" -int16 -32769"))
                    {
                      bRet = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (oArguments.parse(" -uint16 20"))
    {
      if (!oArguments.parse(" -uint16 165535"))
      {
        if (!oArguments.parse(" -uint16 0x10000"))
        {
          if (oArguments.parse(" -uint16 0xffff") &&
              !oArguments.getValue("-uint16").isInt() &&
              oArguments.getValue("-uint16").getUint16() == UINT16_MAX)
          {
            if (!oArguments.parse(" -uint16 -1"))
            {
              bRet = true;
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (!oArguments.parse(" -int8 string"))
    {
      if (oArguments.parse(" -int8 20"))
      {
        if (!oArguments.parse(" -int8 1255"))
        {
          if (!oArguments.parse(" -int8 0x100"))
          {
            if (oArguments.parse(" -int8 -0x1") &&
                oArguments.getValue("-int8").isInt() &&
                oArguments.getValue("-int8").getInt8() == -1)
            {
              if (oArguments.parse(" -int8 -1"))
              {
                if (!oArguments.parse(" -int8 128"))
                {
                  if (oArguments.parse(" -int8 -128"))
                  {
                    if (!oArguments.parse(" -int8 -129"))
                    {
                      bRet = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if (bRet == true)
  {
    bRet = false;
    if (oArguments.parse(" -uint8 20"))
    {
      if (!oArguments.parse(" -uint8 256"))
      {
        if (!oArguments.parse(" -uint8 0x100"))
        {
          if (oArguments.parse(" -uint8 0xff") &&
              !oArguments.getValue("-uint8").isInt() &&
              oArguments.getValue("-uint8").getUint8() == UINT8_MAX)
          {
            if (!oArguments.parse(" -uint8 -1"))
            {
              bRet = true;
            }
          }
        }
      }
    }
  }
  return bRet;
}

bool CArgumentTest::testRequirements()
{
  bool bRet = false;
  CcArguments oArguments;
  oArguments.setVariablesList(
    {
      { 
        "-i",      CcVariant::EType::String, "", "Description0", 
        { 
          {"-o",      CcVariant::EType::String, "", "Description1"},
          {"-t",      CcVariant::EType::String, "", "Description2"}
        }
      },
      {
        "-j",      CcVariant::EType::String, "", "Description4",
        {
          {"-t",      CcVariant::EType::String, "", "Description6"}
        },
        {
          {"-o",      CcVariant::EType::String, "", "Description5"}
        }
      }
    }
  );
  if (oArguments.parse(" -i input -o output -t test"))
  {
    // Arguments can not be found correctly but are required
    if (!oArguments.parse(" -i -o output -t test") &&
          oArguments.getErrorMessage().size() != 0)
    {
      // Arguments can not be found correctly but are not required
      // Verification of input value -o for argument -j has to be verified by application.
      if (oArguments.parse(" -j -o output -t test") &&
          oArguments.getErrorMessage().size() == 0 &&
          oArguments.getValue("-j").getString() == "-o" &&
          oArguments.getValue("-t").getString() == "test" &&
          oArguments.getUnparsed().size() > 0 && oArguments.getUnparsed()[0] == "output")
      {
        if (oArguments.parse(" -j input -o output -t test") &&
            oArguments.getValue("-j").getString() == "input" &&
            oArguments.getValue("-o").getString() == "output" &&
            oArguments.getValue("-t").getString() == "test")
        {
          bRet = true;
        }
      }
    }
  }
  return bRet;
}

bool CArgumentTest::testPrintHelp()
{
  bool bRet = false;
  CcArguments oArguments;
  oArguments.setVariablesList(
    {
      { 
        "-i",      CcVariant::EType::String, "", "Description0", 
        { 
          {"-o",      CcVariant::EType::String, "", "Description1"},
          {"-t",      CcVariant::EType::String, "", "Description2"}
        }
      },
      {
        "-j",      CcVariant::EType::String, "", "Description4",
        {
          {"-t",      CcVariant::EType::String, "", "Description6"}
        },
        {
          {"-o",      CcVariant::EType::String, "", "Description5"}
        }
      }
    }
  );
  CcString oString;
  CcStringStream oStream(oString);
  oArguments.writeHelp(oStream);
  if (oString.length() > 0)
  {
    bRet = true;
  }
  return bRet;
}
