#include "CcFontFile.h"

#include <QString>
#include <QDateTime>
#include <QDebug>

#include "CcStatic.h"

#pragma pack(push, 4)
class CcFontFile::CPrivate
{
public:
  class CTable
  {
  private:
    char   cName[4];
    uint32 uiChecksum;
    uint32 uiOffset;
    uint32 uiLength;

  public:
    QString getName()
    {
      QString sName;
      sName.append(cName[0]);
      sName.append(cName[1]);
      sName.append(cName[2]);
      sName.append(cName[3]);
      return sName;
    }

    uint32 getChecksum()
      { return CcStatic::swapUint32(uiChecksum);}
    uint32 getOffset()
      { return CcStatic::swapUint32(uiOffset);}
    uint32 getLength()
      { return CcStatic::swapUint32(uiLength);}

    bool checksumIsOk(void* pFileData)
    {
      return calculateChecksum(pFileData) == getChecksum();
    }

    uint32 calculateChecksum(void* pFileData)
    {
      char* pcData = static_cast<char*>(pFileData) + getOffset();
      uint32* pData = CCVOIDPTRCAST(uint32*, pcData);
      uint32 uiSum = 0;
      uint32* pEnd = pData+(((getLength()+3) & ~3) / sizeof(uint32));
      while (pData < pEnd)
          uiSum += CcStatic::swapUint32(*pData++);
      return uiSum;
    }
  };
  class CFileInfo
  {
  private:
    uint32 uiScalarType;
    uint16 uiNumTables;
    uint16 uiSearchRange;
    uint16 uiEntrySector;
    uint16 uiRangeShift;
    CTable aTables[1];

  public:
    uint32 getScalarType()
      { return CcStatic::swapUint32(uiScalarType);}
    uint16 getNumTables()
      { return CcStatic::swapUint16(uiNumTables);}
    uint16 getSearchRange()
      { return CcStatic::swapUint16(uiSearchRange);}
    uint16 getEntrySector()
      { return CcStatic::swapUint16(uiEntrySector);}
    uint16 getRangeShift()
      { return CcStatic::swapUint16(uiRangeShift);}
    CTable* getTables()
      { return aTables; }
  };
#pragma pack(pop)

public:
  void*                 pFileData = nullptr;
  CcFontTableGlyph* pGlyph = nullptr;
  CcFontTableHead*  pHead = nullptr;
  CcFontTableLoca*  pLocale = nullptr;
  CcFontTableName*  pName =nullptr;
  CcFontTableMaxp*  pMaxp = nullptr;
};


CcFontFile::CcFontFile()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcFontFile::~CcFontFile()
{
  CCDELETE(m_pPrivate);
}

bool CcFontFile::open()
{
  return m_oFile.open(QIODevice::ReadOnly);
}

void CcFontFile::close()
{
  m_oFile.close();
}

void CcFontFile::parse()
{
  m_oFileBuffer = m_oFile.readAll();
  m_pPrivate->pFileData = m_oFileBuffer.data();
  CPrivate::CFileInfo* pFileInfo = static_cast<CPrivate::CFileInfo*>(m_pPrivate->pFileData);
  CPrivate::CTable* pTable = pFileInfo->getTables();
  uint16 uiNumTables = pFileInfo->getNumTables();
  for(uint16 uiTable=0; uiTable < uiNumTables; uiTable++)
  {
    uint32 uiCurrentTableOffset = pTable->getOffset();
    QString sName(pTable->getName());
    if(sName == "head")
    {
      m_pPrivate->pHead = CCVOIDPTRCAST(CcFontTableHead*, static_cast<char*>(m_pPrivate->pFileData) + uiCurrentTableOffset);
      if(!m_pPrivate->pHead->checkMagicNumber())
      {
        qDebug() << "Magic number in head table wrong";
      }
    }
    else if(!pTable->checksumIsOk(m_pPrivate->pFileData))
    {
      qDebug() << "Checksum failed on table: " << sName << "";
    }
    if(sName == "loca")
    {
      m_pPrivate->pLocale = CCVOIDPTRCAST(CcFontTableLoca*, static_cast<char*>(m_pPrivate->pFileData) + uiCurrentTableOffset);
    }
    if(sName == "name")
    {
      m_pPrivate->pName = CCVOIDPTRCAST(CcFontTableName*, static_cast<char*>(m_pPrivate->pFileData) + uiCurrentTableOffset);
#ifdef DEBUG
      qDebug() << "Name Copyright           " << m_pPrivate->pName->getCopyright()            ;
      qDebug() << "Name FamilyName          " << m_pPrivate->pName->getFamilyName()           ;
      qDebug() << "Name SubFamilyName       " << m_pPrivate->pName->getSubFamilyName()        ;
      qDebug() << "Name UniqueId            " << m_pPrivate->pName->getUniqueId()             ;
      qDebug() << "Name FullName            " << m_pPrivate->pName->getFullName()             ;
      qDebug() << "Name Version             " << m_pPrivate->pName->getVersion()              ;
      qDebug() << "Name PostScriptName      " << m_pPrivate->pName->getPostScriptName()       ;
      qDebug() << "Name Trademark           " << m_pPrivate->pName->getTrademark()            ;
      qDebug() << "Name Manurfacturer       " << m_pPrivate->pName->getManurfacturer()        ;
      qDebug() << "Name Designer            " << m_pPrivate->pName->getDesigner()             ;
      qDebug() << "Name Description         " << m_pPrivate->pName->getDescription()          ;
      qDebug() << "Name VendorUrl           " << m_pPrivate->pName->getVendorUrl()            ;
      qDebug() << "Name DesignerUrl         " << m_pPrivate->pName->getDesignerUrl()          ;
      qDebug() << "Name License             " << m_pPrivate->pName->getLicense()              ;
      qDebug() << "Name LicenseUrl          " << m_pPrivate->pName->getLicenseUrl()           ;
      qDebug() << "Name Reserved            " << m_pPrivate->pName->getReserved()             ;
      qDebug() << "Name TypographicFamily   " << m_pPrivate->pName->getTypographicFamily()    ;
      qDebug() << "Name TypographicSubFamily" << m_pPrivate->pName->getTypographicSubFamily() ;
#endif
    }
    else if(sName == "glyf")
    {
      m_pPrivate->pGlyph = CCVOIDPTRCAST(CcFontTableGlyph*, static_cast<char*>(m_pPrivate->pFileData) + uiCurrentTableOffset);
    }
    else if(sName == "maxp")
    {
      m_pPrivate->pMaxp = CCVOIDPTRCAST(CcFontTableMaxp*, static_cast<char*>(m_pPrivate->pFileData) + uiCurrentTableOffset);
    }
    else
    {
      qDebug() << "Unknown table: " << sName;
    }
    pTable++;
  }
}

CcFontTableGlyph::EType CcFontFile::getGlyphType(uint16 uiSign)
{
  uint32 uiGlyphOffset = m_pPrivate->pLocale->getGlyphOffset(uiSign, m_pPrivate->pHead->getIndexToLocFormat());
  CcFontTableGlyph* pGlyph = m_pPrivate->pGlyph->getGlyph(uiGlyphOffset);
  return pGlyph->getType();
}

CcFontTableGlyph::CCompound CcFontFile::getCompoundGlyph(uint16 uiSign)
{
  uint32 uiGlyphOffset = m_pPrivate->pLocale->getGlyphOffset(uiSign, m_pPrivate->pHead->getIndexToLocFormat());
  CcFontTableGlyph* pGlyph = m_pPrivate->pGlyph->getGlyph(uiGlyphOffset);
  return pGlyph->getCompoundGlyph();
}

CcFontTableGlyph::CSimple CcFontFile::getSimpleGlyph(uint16 uiSign)
{
  uint32 uiGlyphOffset = m_pPrivate->pLocale->getGlyphOffset(uiSign, m_pPrivate->pHead->getIndexToLocFormat());
  CcFontTableGlyph* pGlyph = m_pPrivate->pGlyph->getGlyph(uiGlyphOffset);
  return pGlyph->getSimleGlyph();
}

uint16 CcFontFile::getUnitsPerEm()
{
  return  m_pPrivate->pHead->getUintsPerEm();
}

CcFontTableHead* CcFontFile::getHead()
{
  return m_pPrivate->pHead;
}

CcFontTableName* CcFontFile::getName()
{
  return m_pPrivate->pName;
}

CcFontTableMaxp* CcFontFile::getMaxp()
{
  return m_pPrivate->pMaxp;
}
