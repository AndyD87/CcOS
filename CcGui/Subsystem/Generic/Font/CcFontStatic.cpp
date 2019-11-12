#include "CcFontStatic.h"

CcFontStatic::CcFontStatic()
{

}

int16 CcFontStatic::swapInt16(int16 iInput)
{
  swap(&iInput, sizeof(iInput))  ;
  return iInput;
}

int32 CcFontStatic::swapInt32(int32 iInput)
{
  swap(&iInput, sizeof(iInput))  ;
  return iInput;
}

int64 CcFontStatic::swapInt64(int64 iInput)
{
  swap(&iInput, sizeof(iInput))  ;
  return iInput;
}

uint16 CcFontStatic::swapUint16(uint16 uiInput)
{
  swap(&uiInput, sizeof(uiInput))  ;
  return uiInput;
}

uint32 CcFontStatic::swapUint32(uint32 uiInput)
{
  swap(&uiInput, sizeof(uiInput))  ;
  return uiInput;
}

uint64 CcFontStatic::swapUint64(uint64 uiInput)
{
  swap(&uiInput, sizeof(uiInput))  ;
  return uiInput;
}

float CcFontStatic::uint16ToFloat(uint16 uiInput)
{
  uint16 uiCalc = uiInput & 0x7fff;
  float fOutput = static_cast<float>(uiCalc) / 0x8000;
  if(uiInput & 0x8000) fOutput -= fOutput;
  return fOutput;
}

void CcFontStatic::swap(void *pBuffer, size_t uiSize)
{
  uchar* puBuffer = static_cast<uchar*>(pBuffer);
  for(size_t i = 0; i < (uiSize/2); i++)
  {
    uchar uTemp = puBuffer[i] ;
    puBuffer[i] = puBuffer[uiSize - (1+i)];
    puBuffer[uiSize - (1+i)] = uTemp;
  }
}
