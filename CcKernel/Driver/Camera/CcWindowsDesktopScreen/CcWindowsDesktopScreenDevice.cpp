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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcWindowsDesktopScreenDevice
 *
 * @look https://technet.microsoft.com/de-de/library/security/dd145119
 */

#include "CcWindowsDesktopScreenDevice.h"
#include <wingdi.h>
#include "CcKernel.h"

CcWindowsDesktopScreenDevice::CcWindowsDesktopScreenDevice()
{
}

CcWindowsDesktopScreenDevice::~CcWindowsDesktopScreenDevice()
{
}

CcImageData CcWindowsDesktopScreenDevice::getImage()
{
  CcImageData baTempBuffer;
  baTempBuffer.setType(EImageType::Bmp);
  // get the device context of the screen
  HDC hScreenDC = CreateDCA("DISPLAY", nullptr, nullptr, nullptr);
  // and a device context to put it in
  HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

  int width = GetDeviceCaps(hScreenDC, HORZRES);
  int height = GetDeviceCaps(hScreenDC, VERTRES);

  // maybe worth checking these are positive values
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

  // get a new bitmap
  HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));

  if (!BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY))
  {
  }
  else
  {
    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));

    PBITMAPINFO pbmi = CreateBitmapInfoStruct(hBitmap);
    PBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;

    BITMAPFILEHEADER hdr;       // bitmap file-header
    LPBYTE lpBits;              // memory pointer

    lpBits = static_cast<LPBYTE>(GlobalAlloc(GMEM_FIXED, pbmih->biSizeImage));

    if (!lpBits)
    {
      CCERROR("WindowsDesktopScreen: GlobalAlloc");
    }
    else
    {
      // Retrieve the color table (RGBQUAD array) and the bits
      // (array of palette indices) from the DIB.
      int iSuccess = GetDIBits(hMemoryDC, hBitmap, 0, static_cast<UINT>(pbmih->biHeight), lpBits, pbmi, DIB_RGB_COLORS);
      if (iSuccess == 0)
      {
        CCERROR("WindowsDesktopScreen: GetDIBits");
      }
      else
      {
        hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
        // Compute the size of the entire file.
        hdr.bfSize = static_cast<DWORD>((sizeof(BITMAPFILEHEADER) +
          pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage));
        hdr.bfReserved1 = 0;
        hdr.bfReserved2 = 0;

        // Compute the offset to the array of color indices.
        hdr.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD));

        // Write Header Data to temporary Buffer
        baTempBuffer.getBuffer().append(CCVOIDPTRCAST(char*, &hdr), sizeof(BITMAPFILEHEADER));

        // Write Data to temporary Buffer
        baTempBuffer.getBuffer().append(CCVOIDPTRCAST(char*, pbmih), sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof(RGBQUAD));

        // Write Data to temporary Buffer
        baTempBuffer.getBuffer().append(CCVOIDPTRCAST(char*, lpBits), pbmih->biSizeImage);
      }
      // Free memory.
      GlobalFree(static_cast<HGLOBAL>(lpBits));
    }
  }
  // clean up
  DeleteDC(hMemoryDC);
  DeleteDC(hScreenDC);

  return baTempBuffer;
}

PBITMAPINFO CcWindowsDesktopScreenDevice::CreateBitmapInfoStruct(HBITMAP hBmp)
{
  BITMAP bmp;
  PBITMAPINFO pbmi;
  DWORD    cClrBits;

  // Retrieve the bitmap color format, width, and height.
  if (!GetObjectW(hBmp, sizeof(BITMAP), CCVOIDPTRCAST(LPSTR, &bmp)))
    CCERROR("WindowsDesktopScreen: GetObject");

  // Convert the color format to a count of bits.
  cClrBits = static_cast<DWORD>(bmp.bmPlanes * bmp.bmBitsPixel);
  if (cClrBits == 1)
    cClrBits = 1;
  else if (cClrBits <= 4)
    cClrBits = 4;
  else if (cClrBits <= 8)
    cClrBits = 8;
  else if (cClrBits <= 16)
    cClrBits = 16;
  else if (cClrBits <= 24)
    cClrBits = 24;
  else cClrBits = 32;

  // Allocate memory for the BITMAPINFO structure. (This structure
  // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
  // data structures.)

  if (cClrBits < 24)
    pbmi = static_cast<PBITMAPINFO>(LocalAlloc(LPTR, ((static_cast<size_t>(1) << static_cast<size_t>(cClrBits))*sizeof(RGBQUAD)) + sizeof(BITMAPINFOHEADER)));

  // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel

  else
    pbmi = static_cast<PBITMAPINFO>(LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)));

  // Initialize the fields in the BITMAPINFO structure.
  if (pbmi != nullptr)
  {
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
      pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color
    // indices and store the result in biSizeImage.
    // The width must be DWORD aligned unless the bitmap is RLE
    // compressed.
    pbmi->bmiHeader.biSizeImage = (((static_cast<unsigned long>(pbmi->bmiHeader.biWidth) * cClrBits + 31) & ~static_cast<unsigned long>(31)) / 8)
                                  * static_cast<unsigned long>(pbmi->bmiHeader.biHeight);
    // Set biClrImportant to 0, indicating that all of the
    // device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;
  }
  return pbmi;
}

void CcWindowsDesktopScreenDevice::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
  HANDLE hf;                  // file handle
  BITMAPFILEHEADER hdr;       // bitmap file-header
  PBITMAPINFOHEADER pbih;     // bitmap info-header
  LPBYTE lpBits;              // memory pointer
  DWORD cb = 0;               // incremental count of bytes
  BYTE *hp;                   // byte pointer
  DWORD dwTmp;

  pbih = &pbi->bmiHeader;
  lpBits = static_cast<LPBYTE>(GlobalAlloc(GMEM_FIXED, pbih->biSizeImage));

  if (!lpBits)
    CCERROR("WindowsDesktopScreen: GlobalAlloc");

  // Retrieve the color table (RGBQUAD array) and the bits
  // (array of palette indices) from the DIB.
  if (!GetDIBits(hDC, hBMP, 0, static_cast<UINT>(pbih->biHeight), lpBits, pbi,
    DIB_RGB_COLORS))
  {
    CCERROR("WindowsDesktopScreen: GetDIBits");
  }

  // Create the .BMP file.
  hf = CreateFile(pszFile,
    GENERIC_READ | GENERIC_WRITE,
    static_cast<DWORD>(0),
    nullptr,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    nullptr);
  if (hf == INVALID_HANDLE_VALUE)
    CCERROR("WindowsDesktopScreen: CreateFile");
  hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
  // Compute the size of the entire file.
  hdr.bfSize = static_cast<DWORD>((sizeof(BITMAPFILEHEADER) +
    pbih->biSize + pbih->biClrUsed
    * sizeof(RGBQUAD) + pbih->biSizeImage));
  hdr.bfReserved1 = 0;
  hdr.bfReserved2 = 0;

  // Compute the offset to the array of color indices.
  hdr.bfOffBits = static_cast<DWORD>( sizeof(BITMAPFILEHEADER) +
    pbih->biSize + pbih->biClrUsed
    * sizeof(RGBQUAD));

  // Copy the BITMAPFILEHEADER into the .BMP file.
  if (!WriteFile(hf, static_cast<void*>(&hdr), sizeof(BITMAPFILEHEADER), static_cast<LPDWORD>(&dwTmp), nullptr))
  {
    CCERROR("WindowsDesktopScreen: WriteFile");
  }

  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
  if (!WriteFile(hf, static_cast<void*>(pbih), sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), static_cast<LPDWORD>(&dwTmp), nullptr))
    CCERROR("WindowsDesktopScreen: WriteFile");

  // Copy the array of color indices into the .BMP file.
  hp = lpBits;
  if (!WriteFile(hf, static_cast<void*>(hp), static_cast<size_t>(cb), static_cast<LPDWORD>(&dwTmp), nullptr))
    CCERROR("WindowsDesktopScreen: WriteFile");

  // Close the .BMP file.
  if (!CloseHandle(hf))
    CCERROR("WindowsDesktopScreen: CloseHandle");

  // Free memory.
  GlobalFree(static_cast<HGLOBAL>(lpBits));
}
