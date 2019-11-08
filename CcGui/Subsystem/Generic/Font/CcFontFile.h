#ifndef CcFontFILE_H
#define CcFontFILE_H

#include "CcFile.h"
#include "CcByteArray.h"
#include "CcBase.h"
#include "CcFontTableGlyph.h"
#include "CcFontTableHead.h"
#include "CcFontTableLoca.h"
#include "CcFontTableName.h"
#include "CcFontTableMaxp.h"

class QString;
class QDateTime;

class CcFontFile
{
public:
  CcFontFile();
  ~CcFontFile();

  void setFilePath(const QString& sFilePath)
    { close(); m_oFile.setFileName(sFilePath); }

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
  qint64 pos()
    { return m_oFile.setFilePointer(); }
  bool seek(qint64 iPos)
    { return m_oFile.setFilePointer(iPos); }

  class CPrivate;
  CPrivate*   m_pPrivate = nullptr;
  QByteArray  m_oFileBuffer;
  CcFile      m_oFile;
};

#endif // CcFontFILE_H
