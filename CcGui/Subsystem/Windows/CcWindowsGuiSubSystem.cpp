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
 * @brief     Class CcGuiSubsystem
 */

#include "CcGuiSubsystem.h"
#include "CcSystem.h"
#include "CcWString.h"
#include "CcWindowsGuiSubSystemMgmt.h"
#include "CcWindowsGuiUtil.h"
#include "CcInputEvent.h"
#include "CcKernel.h"
#include "windowsx.h"
#include "Wingdi.h"
#include "CcWindow.h"
#include "CcCWnd.h"

#include <afxvisualmanager.h>
#include <afxext.h>

bool g_bAfxInitialized = false;

class CcGuiSubsystem::CPrivate
{
public:
  CPrivate(CcGuiSubsystem* pSubsystem) : m_pSubsystem(pSubsystem){}

  CcWindow* getWindowHandle()
  {
    return m_pSubsystem->getWindowHandle();
  }

  void init();
  void onColorChanged();
  void updateSize();
  void updatePos();
  void TrackMouse();
  void drawBitmap(HWND hWnd);

  HINSTANCE m_hInst;
  HWND      m_hWnd;
  CcBitmap  m_Bitmap;
  BITMAPINFO m_bmi;
  CcSize    m_oNextSize;
  bool m_bMouseTrackingOn = false;
  bool m_bWindowClosedCalled = false;

  uint32 m_uiMouseFlags = 0;

  int32 m_DrawXStart;
  int32 m_DrawYStart;
  int32 m_DrawXSize;
  int32 m_DrawYSize;
  int32 m_CursorX;
  int32 m_CursorY;
  CcWString m_WindowTitle;
  CcWString m_WindowId;
  RECT m_oLastWindowRect;
  CcRectangle   m_oInnerRect;

private: //member
  CcGuiSubsystem* m_pSubsystem;
};

void CcGuiSubsystem::CPrivate::init()
{
  if (!g_bAfxInitialized)
  {
    g_bAfxInitialized = true;
    if (AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
      AfxGetInstanceHandle();
  }
  m_WindowId.append(L"MainWClass");
  WNDCLASSEXW wcx;
  m_hInst = (HINSTANCE) GetModuleHandle(nullptr);
  // Fill in the window class structure with parameters 
  // that describe the main window. 
  wcx.cbSize = sizeof(wcx);          // size of structure 
  wcx.style = CS_HREDRAW | CS_VREDRAW; // redraw if size changes 
  wcx.lpfnWndProc = CcWindowsGuiSubSystemMgmt::mainWndProc;//MainWndProc;     // points to window procedure 
  wcx.cbClsExtra = 0;                // no extra class memory 
  wcx.cbWndExtra = 0;                // no extra window memory 
  wcx.hInstance = m_hInst;         // handle to instance 
  wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // predefined app. icon 
  wcx.hCursor = LoadCursor(nullptr, IDC_ARROW); // predefined arrow 
  wcx.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH); // white background brush 
  wcx.lpszMenuName = nullptr;    // name of menu resource 
  wcx.lpszClassName = CcWindowsGuiUtil::getWidgetClass();  // name of window class 
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
      CcWindowsGuiUtil::getWidgetClass(),     // name of window class 
      m_WindowId.getLPCWSTR(),            // title-bar string 
      WS_POPUP | WS_OVERLAPPEDWINDOW,        // WS_OVERLAPPEDWINDOW, // top-level window 
      getWindowHandle()->getPos().getX(),          // default horizontal position 
      getWindowHandle()->getPos().getY(),          // default vertical position 
      getWindowHandle()->getWidth(),         // default width 
      getWindowHandle()->getHeight(),         // default height 
      (HWND) nullptr,        // no owner window 
      (HMENU) nullptr,       // use class menu 
      m_hInst,              // handle to application instance 
      (LPVOID) nullptr);     // no window-creation data
    if (m_hWnd != NULL)
    {
      CcWindowsGuiSubSystemMgmt::registerWindow(m_hWnd, m_pSubsystem);

      ShowWindow(m_hWnd, SW_SHOWDEFAULT);
      UpdateWindow(m_hWnd);

      memset(&m_bmi, 0, sizeof(m_bmi));
      m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      m_bmi.bmiHeader.biWidth = getWindowHandle()->getWidth();
      m_bmi.bmiHeader.biHeight = -getWindowHandle()->getHeight(); // set negative, because bitmaps coordinates are beginning at bottom left corner
      m_bmi.bmiHeader.biPlanes = 1;
      m_bmi.bmiHeader.biBitCount = 24;
      m_bmi.bmiHeader.biCompression = BI_RGB;
      m_Bitmap.setSize(getWindowHandle()->getWidth(), getWindowHandle()->getHeight());
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

void CcGuiSubsystem::CPrivate::onColorChanged()
{

}

void CcGuiSubsystem::CPrivate::updateSize()
{
  if (m_oNextSize != getWindowHandle()->getSize())
  {
    m_Bitmap.setSize(getWindowHandle()->getWidth(), getWindowHandle()->getHeight());
    m_bmi.bmiHeader.biHeight = -getWindowHandle()->getHeight();
    m_bmi.bmiHeader.biWidth = getWindowHandle()->getWidth();
    SetWindowPos(m_hWnd, 0, getWindowHandle()->getPos().getX(), getWindowHandle()->getPos().getY(), getWindowHandle()->getWidth(), getWindowHandle()->getHeight(), SWP_NOZORDER | SWP_NOMOVE);
  }
}

void CcGuiSubsystem::CPrivate::updatePos()
{
  SetWindowPos(m_hWnd, 0, getWindowHandle()->getPos().getX(), getWindowHandle()->getPos().getY(), getWindowHandle()->getWidth(), getWindowHandle()->getHeight(), SWP_NOZORDER | SWP_NOSIZE);
}

void CcGuiSubsystem::CPrivate::TrackMouse()
{
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_HOVER | TME_LEAVE; //Type of events to track & trigger.
  tme.dwHoverTime = 1; //How long the mouse has to be in the window to trigger a hover event.
  tme.hwndTrack = m_hWnd;
  TrackMouseEvent(&tme);
}

void CcGuiSubsystem::CPrivate::drawBitmap(HWND hWnd)
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

      for (int32 y = (int32) ps.rcPaint.top; y < ps.rcPaint.bottom; y++)
      {
        for (int32 x = (int32) ps.rcPaint.left; x < ps.rcPaint.right; x++)
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

CcStatus CcGuiSubsystem::open()
{
  CcStatus oStatus;
  if (m_pPrivate == nullptr)
  {
    CCNEW(m_pPrivate, CPrivate, this);
    m_pPrivate->init();
  }
  return oStatus;
}

CcStatus CcGuiSubsystem::close()
{
  CcStatus oStatus;
  if (m_pPrivate->m_hWnd != INVALID_HANDLE_VALUE)
  {
    m_pPrivate->m_bWindowClosedCalled = true;
    SendMessage(m_pPrivate->m_hWnd, WM_CLOSE, 0, 0);
  }
  CCDELETE(m_pPrivate);
  if (CMFCVisualManager::GetInstance() != NULL)
    delete CMFCVisualManager::GetInstance();
  return oStatus;
}

void CcGuiSubsystem::loop()
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
      if (CcWindowsGuiSubSystemMgmt::hasOpenedWindows() == false)
        break;
    }
  } while (bRet != 0);
}

void CcGuiSubsystem::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  int32 iPointX = m_pPrivate->m_CursorX + m_pPrivate->m_DrawXStart;
  int32 iPointY = m_pPrivate->m_CursorY + m_pPrivate->m_DrawYStart;
  uint64 uiCount = 0;
  while (uiCount < uiNumber)
  {
    uiCount++;
    if (iPointY >= 0 &&
      m_pPrivate->m_CursorX < getWindowHandle()->getWidth() &&
      m_pPrivate->m_CursorY < getWindowHandle()->getHeight())
    {
      int32 uiTemp = (iPointY) * getWindowHandle()->getWidth();
      uiTemp += iPointX;
      if (uiTemp < m_pPrivate->m_Bitmap.pixCount)
      {
        m_pPrivate->m_Bitmap.bitmap[uiTemp].R = oPixel.getR();
        m_pPrivate->m_Bitmap.bitmap[uiTemp].G = oPixel.getG();
        m_pPrivate->m_Bitmap.bitmap[uiTemp].B = oPixel.getB();
      }
    }
    if (m_pPrivate->m_CursorX < m_pPrivate->m_DrawXSize - 1)
    {
      m_pPrivate->m_CursorX++;
    }
    else
    {
      m_pPrivate->m_CursorX = 0;
      if (m_pPrivate->m_CursorY < m_pPrivate->m_DrawYSize - 1)
      {
        m_pPrivate->m_CursorY++;
      }
      else
      {
        // End of Rectangle reached, draw window.
        m_pPrivate->m_CursorY = 0;
      }
    }
    iPointX = m_pPrivate->m_CursorX + m_pPrivate->m_DrawXStart;
    iPointY = m_pPrivate->m_CursorY + m_pPrivate->m_DrawYStart;
  }
}

bool CcGuiSubsystem::setPixelArea(const CcRectangle& oArea)
{
  m_pPrivate->m_DrawXStart = oArea.getX();
  m_pPrivate->m_DrawYStart = oArea.getY();
  m_pPrivate->m_DrawXSize = oArea.getWidth();
  m_pPrivate->m_DrawYSize = oArea.getHeight();
  m_pPrivate->m_CursorX = 0;
  m_pPrivate->m_CursorY = 0;
  if (( getWindowHandle()->getWidth()  > oArea.getX() &&
        getWindowHandle()->getHeight() > oArea.getY()) ||
      ( getWindowHandle()->getWidth()  > oArea.getBottomRightCorner().getX() &&
        getWindowHandle()->getHeight() > oArea.getBottomRightCorner().getY()))
    return true;
  else
    return false;
}

void CcGuiSubsystem::draw()
{
  if (!RedrawWindow(m_pPrivate->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW))
  {
    CCDEBUG("RedrawWindow failed");
  }
}

intptr CcGuiSubsystem::executeMessage(void* hWndIn, uint32 messageIn, intptr wParamIn, intptr lParamIn)
{
  HWND hWnd     = static_cast<HWND>(hWndIn);
  UINT message  = static_cast<UINT>(messageIn);
  WPARAM wParam = static_cast<WPARAM>(wParamIn);
  LPARAM lParam = static_cast<LPARAM>(lParamIn);
  intptr lRet(0);
  if (m_pPrivate->m_hWnd != 0)
  {
    switch (message)
    {
      case WM_NCACTIVATE:
      {
        EGuiEvent eCmd = EGuiEvent::WindowRestore;
        getControlEventHandler().call(&eCmd);
        lRet = DefWindowProc(hWnd, message, wParam, lParam);
        break;
      }
      case WM_LBUTTONDOWN:
      {
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::LeftDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        getInputEventHandler().call(&Event);
        SET_FLAG(m_pPrivate->m_uiMouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
        SetCapture(m_pPrivate->m_hWnd);
        break;
      }
      case WM_LBUTTONUP:
      {
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::LeftUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        getInputEventHandler().call(&Event);
        REMOVE_FLAG(m_pPrivate->m_uiMouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
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
        if (m_pPrivate->m_bMouseTrackingOn == false)
        {
          m_pPrivate->TrackMouse();
          m_pPrivate->m_bMouseTrackingOn = true;
        }
        CcInputEvent Event;
        Event.setMouseEvent(EMouseEventType::Move, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        Event.getMouseEvent().MouseFlags = m_pPrivate->m_uiMouseFlags;
        getInputEventHandler().call(&Event);
        break;
      }
      case WM_MOUSELEAVE:
      {
        if (m_pPrivate->m_bMouseTrackingOn == true)
        {
          CcInputEvent Event;
          Event.setMouseEvent(EMouseEventType::Leave, 0, 0);
          getInputEventHandler().call(&Event);
          m_pPrivate->m_bMouseTrackingOn = false;
        }
        break;
      }
      case WM_SIZE:
      {
        CcSize oNewSize(static_cast<int16>(LOWORD(lParam)), static_cast<int16>(HIWORD(lParam)));
        m_pPrivate->m_oInnerRect = oNewSize;
        // @todo m_pWindowWidget->setSize(CcSize(oNewSize.getWidth(), oNewSize.getHeight()));
        RECT oWindowRect;
        getRectangle(m_pPrivate->m_hWnd, &oWindowRect);
        m_pPrivate->m_oNextSize.setSize((oWindowRect.right - oWindowRect.left) + 1, (oWindowRect.bottom - oWindowRect.top) + 1);
        getWindowHandle()->setSize(m_pPrivate->m_oNextSize);
        break;
      }
      case WM_MOVE:
      {
        CcPoint oNewPoint(static_cast<int16>(LOWORD(lParam)), static_cast<int16>(HIWORD(lParam)));
        m_pPrivate->m_oInnerRect = oNewPoint;
        RECT oWindowRect;
        getRectangle(m_pPrivate->m_hWnd, &oWindowRect);
        getWindowHandle()->setPos(CcPoint(oWindowRect.left, oWindowRect.top));
        break;
      }
      case WM_PRINTCLIENT:
      case WM_DISPLAYCHANGE:
      case WM_SETREDRAW:
      case WM_PAINT:
        getWindowHandle()->draw();
        m_pPrivate->drawBitmap(m_pPrivate->m_hWnd);
        lRet = 0;
        break;
      case WM_CLOSE:
      {
        if (m_pPrivate->m_bWindowClosedCalled == false)
        {
          EGuiEvent eCmd = EGuiEvent::WindowClose;
          getControlEventHandler().call(&eCmd);
        }
        else
          DestroyWindow(m_pPrivate->m_hWnd);
        break;
      }
      case WM_DESTROY:
        CcWindowsGuiSubSystemMgmt::deleteWindow(m_pPrivate->m_hWnd, this);
        m_pPrivate->m_hWnd = 0;
        PostQuitMessage(0);
        return 0;
        break;
      default:
        lRet = DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  return lRet;
}

void CcGuiSubsystem::getMaxArea(CcRectangle& oArea)
{
  RECT workarea = {0};
  // Get the current work area
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  oArea.setX((int32)(workarea.left));
  oArea.setY((int32)(workarea.top));
  oArea.setWidth((int32)(workarea.right - workarea.left));
  oArea.setHeight((int32)(workarea.bottom - workarea.top));
}

void CcGuiSubsystem::setWindowTitle(const CcString& sTitle)
{
  SetWindowTextW(m_pPrivate->m_hWnd, TOLPCWSTR(sTitle));
}

bool CcGuiSubsystem::setWindowState(EWindowState eState)
{
  bool bRet = false;
  switch (eState)
  {
    case EWindowState::Close:
      if(close())bRet = true;
      break;
    case EWindowState::Maximimized:
    {
      bRet = true;
      getRectangle(m_pPrivate->m_hWnd, &m_pPrivate->m_oLastWindowRect);
      //if (ShowWindow(m_hWnd, SW_MAXIMIZE))bRet = true;

      // Get Maximum work are
      RECT WorkArea;
      SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
      // Set new window size and position as maximized   
      SetWindowPos(m_pPrivate->m_hWnd, NULL, WorkArea.left, WorkArea.top, WorkArea.right, WorkArea.bottom, NULL);
      break;
    }
    case EWindowState::Normal:
      bRet = true;
      //if (ShowWindow(m_hWnd, SW_NORMAL))bRet = true;
      SetWindowPos(m_pPrivate->m_hWnd, NULL, m_pPrivate->m_oLastWindowRect.left, m_pPrivate->m_oLastWindowRect.top, m_pPrivate->m_oLastWindowRect.right, m_pPrivate->m_oLastWindowRect.bottom, NULL);
      break;
    case EWindowState::Minimized:
      if (ShowWindow(m_pPrivate->m_hWnd, SW_MINIMIZE))bRet = true;
      break;
    case EWindowState::Tray:
    case EWindowState::Hide:
      if(ShowWindow(m_pPrivate->m_hWnd, SW_HIDE))bRet = true;
      break;
  }
  return bRet;
}

void CcGuiSubsystem::hide()
{
  if (ShowWindow(m_pPrivate->m_hWnd, SW_MINIMIZE) == FALSE)
  {
    CCDEBUG("Minimize failed" + CcString::fromNumber(GetLastError()));
  }
}

void CcGuiSubsystem::show()
{
  if (ShowWindow(m_pPrivate->m_hWnd, SW_RESTORE) == FALSE)
  {
    CCDEBUG("Maximized failed" + CcString::fromNumber(GetLastError()));
  }
}

bool CcGuiSubsystem::hasFrame()
{
  return true;
}

CcRectangle CcGuiSubsystem::getInnerRect()
{
  return m_pPrivate->m_oInnerRect;
}

void CcGuiSubsystem::updateSize()
{
  m_pPrivate->updateSize();
}

void CcGuiSubsystem::updatePos()
{
  m_pPrivate->updatePos();
}

void* CcGuiSubsystem::getHandle()
{
  return CWnd::FromHandle(m_pPrivate->m_hWnd);
}
