#ifndef CcFontStatic_H
#define CcFontStatic_H

#include "CcBase.h"

class CcFontStatic
{
public:
  CcFontStatic();

  static int16 swapInt16(int16 iInput);
  static int32 swapInt32(int32 iInput);
  static int64 swapInt64(int64 iInput);
  static uint16 swapUint16(uint16 uiInput);
  static uint32 swapUint32(uint32 uiInput);
  static uint64 swapUint64(uint64 uiInput);
  static float  uint16ToFloat(uint16 uiInput);

  static void swap(void *pBuffer, size_t uiSize);
};

#endif // CcFontStatic_H
