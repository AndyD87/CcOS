#ifndef CcFontFILE_H
#define CcFontFILE_H

#include "CcBase.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include "CcFontTableGlyph.h"
#include "CcFontTableHead.h"
#include "CcFontTableLoca.h"
#include "CcFontTableName.h"
#include "CcFontTableMaxp.h"

class CcString;
class CcDateTime;

class CcFontFile
{
public:
  CcFontFile();
  ~CcFontFile();

  void setFilePath(const CcString& sFilePath)
    { close(); m_oFile.setFilePath(sFilePath); }

  bool open();
  void close();
  void parse();

  CcFontTableGlyph::EType getGlyphType(uint16 uiSign);
  CcFontTableGlyph::CSimple getSimpleGlyph(uint16 uiSign);
  CcFontTableGlyph::CCompound getCompoundGlyph(uint16 uiSign);
  uint16 getUnitsPerEm();
  CcFontTableHead* getHead();
  CcFontTableName* getName();
  CcFontTableMaxp* getMaxp();

private:
  uint64 pos()
    { return m_oFile.getFilePointer(); }
  bool seek(uint64 iPos)
    { return m_oFile.setFilePointer(iPos); }

  class CPrivate;
  CPrivate*   m_pPrivate = nullptr;
  CcByteArray m_oFileBuffer;
  CcFile      m_oFile;
};

#endif // CcFontFILE_H
