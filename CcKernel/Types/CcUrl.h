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
 * @page      Types
 * @subpage   CcUrl
 *
 * @page      CcUrl
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUrl
 */
#ifndef CcUrl_H_
#define CcUrl_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcTypes.h"
#include "CcString.h"

/**
 * @brief Url Class to handle all aspects an URL can have.
 *        Features:
 *          -Usernames
 *          -Passwords
 *          -Ports
 */
class CcKernelSHARED  CcUrl {
private:

public: //methods
  /**
   * @brief Create a URL by String or empty if no String is set.
   */
  CcUrl(const CcString& url = "");

  /**
   * @brief Destructor
   */
  ~CcUrl();

  /**
   * @brief Parse a String o Url
   * @param String with url as content
   * @return true if Url was successfully parsed.
   */
  bool parseUrl(const CcString& url);

  /**
   * @brief Parse a String o Url
   * @return Get full Url from data.
   */
  CcString getUrl(void) const;

  /**
   * @brief Check if Parsed String is a valid URL
   * @return true if URL is valid.
   */
  bool isUrl(void) const;

  /**
   * @brief Get Hostname in URL
   * @return return Handle to Hostname String
   */
  const CcString& getHostname(void) const;

  /**
   * @brief Get Port in URL
   * @return return Handle to Port String
   */
  CcString getPortString(void) const;

  inline uint16 getPort() const
    { return m_uiPort; }

  /**
   * @brief Get Username in URL
   * @return return Handle to Username String
   */
  const CcString& getUsername(void) const;

  /**
   * @brief Get Password in URL
   * @return return Handle to Password String
   */
  const CcString& getPassword(void) const;

  /**
   * @brief Get Protocol in URL
   * @return return Handle to Protocol String
   */
  const CcString& getProtocol(void) const;

  /**
   * @brief Get Path in URL
   * @return return Handle to Path String
   */
  const CcString& getPath(void) const;
  
  /**
   * @brief Set new Hostname to Url
   * @param Hostname: Hostname to Set
   * @return Reference to this Url
   */
  CcUrl& setHostname(const CcString& Hostname);
  
  /**
   * @brief Set new Port to Url
   * @param Port: Port to Set
   * @return Reference to this Url
   */
  CcUrl& setPort(const CcString& sPort);
  
  /**
   * @brief Set new Port to Url
   * @param uiPort: Port as unsigned integer 16 bit
   * @return Reference to this Url
   */
  inline CcUrl& setPort(uint16 uiPort)
    { m_uiPort = uiPort; return *this; }
  /**
   * @brief Set new Username to Url
   * @param Username: Username to Set
   * @return Reference to this Url
   */
  CcUrl& setUsername(const CcString& Username);
  
  /**
   * @brief Set new Password to Url
   * @param Password: Password to Set
   * @return Reference to this Url
   */
  CcUrl& setPassword(const CcString& Password);
  
  /**
   * @brief Set new Protocol to Url
   * @param Protocol: Protocol to Set
   * @return Reference to this Url
   */
  CcUrl& setProtocol(const CcString& Protocol);

  /**
   * @brief Set new Path to Url
   * @param Path: Path to Set
   * @return Reference to this Url
   */
  CcUrl& setPath(const CcString& Path);

  CcUrl& operator=(const CcString& sUrlLink);

private:
  void getPortByProtocol();

private:
  CcString m_Hostname; //!< Hostname part in URL
  uint16   m_uiPort;     //!< Port part in URL
  CcString m_Username; //!< Username part in URL
  CcString m_Password; //!< Password part in URL
  CcString m_Protocol; //!< Protocol part in URL
  CcString m_Path;     //!< Path part in URL
  bool m_IsUrl;        //!< true if URL is valid
};

#endif /* CcUrl_H_ */
