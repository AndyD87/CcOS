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
 * @page      Hash
 * @subpage   CcMd5
 *
 * @page      CcMd5
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMd5
 */
#ifndef H_CcMd5_H_
#define H_CcMd5_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcByteArray.h"
#include "CcHash.h"

class CcString;

class CcKernelSHARED CcMd5 : public IHash
{
public:
  /**
   * @brief Constructor
   */
  CcMd5();
  virtual ~CcMd5() = default;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcMd5& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcMd5& oToCompare) const;

  /**
   * @brief Directly generate hash from ByteArray
   * @param oByteArray: ByteArray to generate hash from
   * @return this
   */
  CcMd5& operator=(const CcByteArray& oByteArray);

  /**
   * @brief Directly generate hash from String
   * @param sString: String to generate hash from
   * @return this
   */
  CcMd5& operator=(const CcString& sString);

  virtual const CcByteArray& getValue() const override
   { return m_oResult; }
  virtual CcByteArray& getValue() override
   { return m_oResult; }

  /**
   * @brief Get result as Hex-String
   * @return resulting string or empty if failed
   */
  CcString getHexString() const;

  /**
   * @brief Overwrite result with new hexstring
   * @param sHexString: Hex data to store in result
   */
  void setHexString(const CcString& sHexString);

  //! @copydoc
  virtual CcMd5& generate(const void *data, size_t size) override;
  //! @copydoc
  virtual CcMd5& append(const void *data, size_t size) override;
  //! @copydoc
  virtual CcMd5& finalize(const void *data, size_t size) override;
  
  //! @copydoc CcHash::generate(const CcByteArray& oByteArray)
  inline CcMd5& generate(const CcByteArray& oByteArray)
    { return generate(oByteArray.getArray(), oByteArray.size());}

  //! @copydoc CcHash::append(const CcByteArray& oByteArray)
  inline CcMd5& append(const CcByteArray& oByteArray)
    { return append(oByteArray.getArray(), oByteArray.size());}

  //! @copydoc CcHash::finalize(const CcByteArray& oByteArray)
  inline CcMd5& finalize(const CcByteArray& oByteArray)
    { return finalize(oByteArray.getArray(), oByteArray.size());}

  //! @copydoc CcHash::finalize()
  inline CcMd5& finalize()
    { return finalize(nullptr, 0);}

private:
  void initValues();
  const void* body(const void *data, size_t size);
private:
  uint32 a;
  uint32 b;
  uint32 c;
  uint32 d;
  uint32 lo;
  uint32 hi;
  uint8  buffer[64];
  uint32 block[16];
  CcByteArray m_oResult;
};

#endif // H_CcMd5_H_
