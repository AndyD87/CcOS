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
 * GNU General   Public License for more details.
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
 * @brief     Class CcWindowsDisplayArea
 */

#include "CcWindowsDisplayArea.h"
#include "CcSystem.h"
#include "CcWString.h"
#include "CcWindowsDisplay.h"
#include "CcInputEvent.h"
#include "CcKernel.h"
#include "windowsx.h"
#include "Wingdi.h"

CcWindowsDisplayArea::CcWindowsDisplayArea(uint16 nr, const CcRectangle& oArea) :
  CcDisplayArea(oArea)
{
  m_WindowId.append(L"MainWClass-");
  m_WindowId.appendNumber(nr);
  init();
}

CcWindowsDisplayArea::~CcWindowsDisplayArea()
{
}

void CcWindowsDisplayArea::init(void)
{
  WNDCLASSEXW wcx;
  m_hInst = (HINSTANCE) GetModuleHandle(nullptr);
  // Fill in the window class structure with parameters 
  // that describe the main window. 
  wcx.cbSize = sizeof(wcx);          // size of structure 
  wcx.style = CS_HREDRAW | CS_VREDRAW; // redraw if size changes 
  wcx.lpfnWndProc = CcWindowsDisplay::mainWndProc;//MainWndProc;     // points to window procedure 
  wcx.cbClsExtra = 0;                // no extra class memory 
  wcx.cbWndExtra = 0;                // no extra window memory 
  wcx.hInstance = m_hInst;         // handle to instance 
  wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // predefined app. icon 
  wcx.hCursor = LoadCursor(nullptr, IDC_ARROW); // predefined arrow 
  wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // white background brush 
  wcx.lpszMenuName = nullptr;    // name of menu resource 
  wcx.lpszClassName = m_WindowId.getLPCWSTR();  // name of window class 
  wcx.hIconSm = (HICON) LoadImage(m_hInst, // small class icon 
    MAKEINTRESOURCE(5),
    IMAGE_ICON,
    GetSystemMetrics(SM_CXSMICON),
    GetSystemMetrics(SM_CYSMICON),
    LR_DEFAULTCOLOR);
  // Register the window class. 
  ATOM uiClass = RegisterClassExW(&wcx);

  // Check if Class Creation failed
  if (uiClass != 0)
  {
    // Create the main window. 
    m_hWnd = CreateWindowW(
      m_WindowId.getLPCWSTR(),     // name of window class 
      m_WindowId.getLPCWSTR(),            // title-bar string 
      WS_POPUP,        // WS_OVERLAPPEDWINDOW, // top-level window 
      getPosX(),          // default horizontal position 
      getPosY(),          // default vertical position 
      getWidth(),         // default width 
      getHeight(),         // default height 
      (HWND) nullptr,        // no owner window 
      (HMENU) nullptr,       // use class menu 
      m_hInst,              // handle to application instance 
      (LPVOID) nullptr);     // no window-creation data
    if (m_hWnd != 0)
    {
      CcWindowsDisplay::registerWindow(m_hWnd, this);
      memset(&m_bmi, 0, sizeof(m_bmi));
      m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      m_bmi.bmiHeader.biWidth = getWidth();
      m_bmi.bmiHeader.biHeight = - getHeight(); // set negative, because bitmaps coordinates are beginning at bottom left corner
      m_bmi.bmiHeader.biPlanes = 1;
      m_bmi.bmiHeader.biBitCount = 24;
      m_bmi.bmiHeader.biCompression = BI_RGB;
      m_Bitmap.setSize(getWidth(), getHeight());
      ShowWindow(m_hWnd, SW_SHOWDEFAULT);
      UpdateWindow(m_hWnd);
    }
    else
    {
      CCDEBUG("Create Window failed with: " + CcString::fromNumber(GetLastError()));
    }
  }
  else
  {
    CCDEBUG("Register Window failed with: " + CcString::fromNumber(GetLastError()));
  }
}

void CcWindowsDisplayArea::loop()
{
  MSG msg;
  BOOL bRet;
  do
  {
    bRet = GetMessage(&msg, NULL, 0, 0);

    if (bRet > 0)  // (bRet > 0 indicates a message that must be processed.)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else if (bRet < 0)  // (bRet == -1 indicates an error.)
    {
      CCDEBUG("ERROR in main-loop");
      bRet = 0;
    }
    else  // (bRet == 0 indicates "exit program".)
    {
      if (CcWindowsDisplay::hasOpenedWindows() == false)
        break;
    }
  } while (bRet != 0);
  CCDEBUG("Window ended");
}

void CcWindowsDisplayArea::close()
{
  if (m_hWnd != INVALID_HANDLE_VALUE)
  {
    m_bWindowClosedCalled = true;
    SendMessage(m_hWnd, WM_CLOSE, 0, 0);
  }
}

void CcWindowsDisplayArea::drawPixel(const CcColor& oPixel)
{
  int16 iPointX = m_CursorX + m_DrawXStart;
  int16 iPointY = m_CursorY + m_DrawYStart;
  if (iPointX >= 0 &&
      iPointY >= 0 &&
      m_CursorX < getWidth() &&
      m_CursorY < getHeight())
  {
    uint32 uiTemp = (iPointY) * getWidth();
    uiTemp += iPointX;
    if (uiTemp < m_Bitmap.pixCount)
    {
      m_Bitmap.bitmap[uiTemp].R = oPixel.getR();
      m_Bitmap.bitmap[uiTemp].G = oPixel.getG();
      m_Bitmap.bitmap[uiTemp].B = oPixel.getB();
    }
  }
  if (m_CursorX < m_DrawXSize - 1)
  {
    m_CursorX++;
  }
  else
  {
    m_CursorX = 0;
    if (m_CursorY < m_DrawYSize - 1)
    {
      m_CursorY++;
    }
    else
    {
      // End of Rectangle reached, draw window.
      m_CursorY = 0;
      RECT oRec;
      oRec.left = m_DrawXStart;
      oRec.top = m_DrawYStart;
      oRec.bottom = m_DrawYStart + m_DrawYSize;
      oRec.right = m_DrawXStart + m_DrawXSize;
      if (!InvalidateRect(m_hWnd, &oRec, FALSE))
      {
        CCDEBUG("InvalidateRect failed");
      }
    }
  }
}

bool CcWindowsDisplayArea::setPixelArea(const CcRectangle& oArea)
{
  m_DrawXStart = oArea.getX();
  m_DrawYStart = oArea.getY();
  m_DrawXSize = oArea.getWidth();
  m_DrawYSize = oArea.getHeight();
  m_CursorX = 0;
  m_CursorY = 0;
  if (( getDrawingArea().getWidth()  > oArea.getX() &&
        getDrawingArea().getHeight() > oArea.getY()) ||
      ( getDrawingArea().getWidth()  > oArea.getBottomRightCorner().getX() &&
        getDrawingArea().getHeight() > oArea.getBottomRightCorner().getY()))
    return true;
  else
    return false;
}

void CcWindowsDisplayArea::draw()
{
  if (!RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW))
  {
    CCDEBUG("RedrawWindow failed");
  }
}

void CcWindowsDisplayArea::drawBitmap(HWND hWnd)
{
  PAINTSTRUCT   ps;
  uint8        *data = nullptr;

  HDC hdc = BeginPaint(hWnd, &ps);
  if (hdc != nullptr)
  {
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateDIBSection(hdc, &m_bmi, DIB_RGB_COLORS, (void **) &data, 0, 0);
    if (hBitmap != nullptr)
    {
      HGDIOBJ hOldBmp = SelectObject(hMemDC, hBitmap);

      for (uint16 y = (uint16) ps.rcPaint.top; y < ps.rcPaint.bottom; y++)
      {
        for (uint16 x = (uint16) ps.rcPaint.left; x < ps.rcPaint.right; x++)
        {
          m_Bitmap.copy((SBitmapRGB*) data, x, y);
        }
      }

      BitBlt(hdc, 0, 0, m_Bitmap.width, m_Bitmap.height, hMemDC, 0, 0, SRCCOPY);

      SelectObject(hMemDC, hOldBmp);
      DeleteDC(hMemDC);
      DeleteObject(hBitmap);
    }

    EndPaint(hWnd, &ps);
  }
}

void CcWindowsDisplayArea::TrackMouse()
{
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_HOVER | TME_LEAVE; //Type of events to track & trigger.
  tme.dwHoverTime = 1; //How long the mouse has to be in the window to trigger a hover event.
  tme.hwndTrack = m_hWnd;
  TrackMouseEvent(&tme);
}

LRESULT CcWindowsDisplayArea::executeMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT lRet(0);
  if (m_hWnd != 0)
  {
    switch (message)
    {
      case WM_NCACTIVATE:
      {
        EDisplayCommands eCmd = EDisplayCommands::Restore;
        getControlEventHandler().call(&eCmd);
        lRet = DefWindowProc(hWnd, message, wParam, lParam);
        break;
      }
      case WM_LBUTTONDOWN:
      {
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::LeftDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        getInputEventHandler().call(&Event);
        SET_FLAG(m_uiMouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
        SetCapture(m_hWnd);
        break;
      }
      case WM_LBUTTONUP:
      {
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::LeftUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        getInputEventHandler().call(&Event);
        REMOVE_FLAG(m_uiMouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
        ReleaseCapture();
        break;
      }
      case WM_LBUTTONDBLCLK:
      {
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::ClickDoubleLeft, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        getInputEventHandler().call(&Event);
        break;
      }
      case WM_MOUSEMOVE:
      {
        if (m_bMouseTrackingOn == false)
        {
          TrackMouse();
          m_bMouseTrackingOn = true;
        }
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::Move, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        Event.getMouseEvent().MouseFlags = m_uiMouseFlags;
        getInputEventHandler().call(&Event);
        break;
      }
      case WM_MOUSELEAVE:
      {
        if (m_bMouseTrackingOn == true)
        {
          CcInputEvent Event;
          Event.setMouseEvent(EMouseEventType::Leave, 0, 0);
          getInputEventHandler().call(&Event);
          m_bMouseTrackingOn = false;
        }
        break;
      }
      case WM_SIZE:
      {
        EDisplayCommands eCmd = EDisplayCommands::Size;
        getControlEventHandler().call(&eCmd);
        lRet = DefWindowProc(hWnd, message, wParam, lParam);
        break;
      }
      case WM_PRINTCLIENT:
      case WM_DISPLAYCHANGE:
      case WM_SETREDRAW:
      case WM_PAINT:
        drawBitmap(m_hWnd);
        lRet = 0;
        break;
      case WM_CLOSE:
      {
        if (m_bWindowClosedCalled == false)
        {
          EDisplayCommands eCmd = EDisplayCommands::Close;
          getControlEventHandler().call(&eCmd);
        }
        else
          DestroyWindow(m_hWnd);
        break;
      }
      case WM_DESTROY:
        CcWindowsDisplay::deleteWindow(m_hWnd, this);
        m_hWnd = 0;
        PostQuitMessage(0);
        return 0;
        break;
      default:
        lRet = DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  return lRet;
}

void CcWindowsDisplayArea::updateSize()
{
  m_Bitmap.setSize(getWidth(), getHeight());
  m_bmi.bmiHeader.biHeight = -getHeight();
  m_bmi.bmiHeader.biWidth = getWidth();
  SetWindowPos(m_hWnd, 0, getPosX(), getPosY(), getWidth(), getHeight(), 0);
}

void CcWindowsDisplayArea::updatePos()
{
  SetWindowPos(m_hWnd, 0, getPosX(), getPosY(), getWidth(), getHeight(), 0);
}

void CcWindowsDisplayArea::getMaxArea(CcRectangle& oArea)
{
  RECT workarea = {0};
  // Get the current work area
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  oArea.setX((int16)(workarea.left));
  oArea.setY((int16)(workarea.top));
  oArea.setWidth((uint16)(workarea.right - workarea.left));
  oArea.setHeight((uint16)(workarea.bottom - workarea.top));
}

void CcWindowsDisplayArea::setWindowTitle(const CcString& sTitle)
{
  SetWindowTextA(m_hWnd, sTitle.getLPCSTR());
}

void CcWindowsDisplayArea::hide()
{
  if (ShowWindow(m_hWnd, SW_MINIMIZE) == FALSE)
  {
    CCDEBUG("Minimize failed" + CcString::fromNumber(GetLastError()));
  }
}

void CcWindowsDisplayArea::show()
{
  if (ShowWindow(m_hWnd, SW_RESTORE) == FALSE)
  {
    CCDEBUG("Maximized failed" + CcString::fromNumber(GetLastError()));
  }
}

void CcWindowsDisplayArea::onColorChanged(void)
{

}
