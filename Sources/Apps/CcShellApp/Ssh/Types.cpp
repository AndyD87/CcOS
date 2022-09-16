/*
 * This file is part of CcShellApp.
 *
 * CcShellApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcShellApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcShellApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation Ssh Types
 */
#include "Types.h"

namespace CcSsh
{
  const uint8 IS_OPTIONAL     = 0;
  const uint8 IS_RECOMMENDED  = 1;
  const uint8 IS_REQUIRED     = 2;

  CcByteArray key_exchange::generateSupported()
  {
    CcByteArray oData;

    return oData;
  }

  SNameRequirement MacRequirements[] = {
    {"hmac-sha1"    , IS_REQUIRED   , true  },
    {"hmac-sha1-96" , IS_RECOMMENDED, false },
    {"hmac-md5"     , IS_OPTIONAL   , true  },
    {"hmac-md5-96"  , IS_OPTIONAL   , false },
    {"none"         , IS_OPTIONAL   , true  }
  };
  size_t MacRequirementsSize = sizeof(MacRequirements) / sizeof(MacRequirements[0]);
  
  SNameRequirement EncryptionRequirements[] = {
    { "3des-cbc"         , IS_REQUIRED    , true  },
    { "blowfish-cbc"     , IS_OPTIONAL    , false },
    { "twofish256-cbc"   , IS_OPTIONAL    , false },
    { "twofish-cbc"      , IS_OPTIONAL    , false },
    { "twofish192-cbc"   , IS_OPTIONAL    , false },
    { "twofish128-cbc"   , IS_OPTIONAL    , false },
    { "aes256-cbc"       , IS_OPTIONAL    , false },
    { "aes192-cbc"       , IS_OPTIONAL    , false },
    { "aes128-cbc"       , IS_RECOMMENDED , true  },
    { "serpent256-cbc"   , IS_OPTIONAL    , false },
    { "serpent192-cbc"   , IS_OPTIONAL    , false },
    { "serpent128-cbc"   , IS_OPTIONAL    , false },
    { "arcfour"          , IS_OPTIONAL    , false },
    { "idea-cbc"         , IS_OPTIONAL    , false },
    { "cast128-cbc"      , IS_OPTIONAL    , false },
    { "none"             , IS_OPTIONAL    , false }
  };
  size_t EncryptionRequirementsSize = sizeof(EncryptionRequirements) / sizeof(EncryptionRequirements[0]);

  SNameRequirement CompressionRequirements[] = {
    {"zlib"         , IS_OPTIONAL   , false },
    {"none"         , IS_REQUIRED   , true  }
  };
  size_t CompressionRequirementsSize = sizeof(CompressionRequirements) / sizeof(CompressionRequirements[0]);

  SNameRequirement PublicKeyRequirements[] = {
    {"ssh-dss"         , IS_REQUIRED    , true  },
    {"ssh-rsa"         , IS_RECOMMENDED , false },
    {"pgp-sign-rsa"    , IS_OPTIONAL    , true  },
    {"pgp-sign-dss"    , IS_OPTIONAL    , true  }
  };
  size_t PublicKeyRequirementsSize = sizeof(PublicKeyRequirements) / sizeof(PublicKeyRequirements[0]);

  SNameRequirement KeyExchangeRequirements[] = {
    {"diffie-hellman-group1-sha1"   , IS_REQUIRED    , true  },
    {"diffie-hellman-group14-sha1"  , IS_REQUIRED    , true },
  };
  size_t KeyExchangeRequirementsSize = sizeof(KeyExchangeRequirements) / sizeof(KeyExchangeRequirements[0]);

}
