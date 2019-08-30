/**************************************************************************
 *
 *  bwin.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  Window用基底クラス 實裝
 *
 *    2016.2.28  修正
 */
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include "bwin.h"


/* WNDCLASS登録情報ベクタ */
std::vector<urania::BasicWindow::WC_> urania::BasicWindow::vwc_S;


/*============================================
 *  BasicWindow::registWC_()
 *  WNDCLASS登録 & WNDCLASS名を返す
 *  同じ内容のWNDCLASSの二重登録を阻止する
 *==========================================*/
std::wstring urania::BasicWindow::registWC_
(const urania::BasicWindow::WC_& wc)
{
  std::vector<WC_>::iterator it = std::find(vwc_S.begin(), vwc_S.end(), wc);
  if (it == vwc_S.end()) {
    ////////////////////////////////////////////////////////
    // WNDCLASSの登録とvectorへの追加
    ////////////////////////////////////////////////////////

    // クラス名をWC_に設定
    WC_ wc2 = wc;
    //std::wostringstream tmp;
    std::basic_ostringstream<wchar_t> tmp;
    tmp << vwc_S.size();

    wc2.wcname_ = std::wstring(L"URANIAWNDCLS") + tmp.str();

    // WNDCLASS Structure
    WNDCLASS wndcls =
    {
      CS_HREDRAW | CS_VREDRAW,
      wc2.proc_,
      0,
      sizeof(Window*),
      getHI_(),
      NULL,
      NULL,
      NULL,
      nullptr,
      wc2.wcname_.c_str()
    };

    // Icon の設定
    if (wc2.icon_id_ != DEFAULT_RC)
      wndcls.hIcon = LoadIcon(getHI_(), MAKEINTRESOURCE(wc2.icon_id_));
    else
      wndcls.hIcon = LoadIcon(0, IDI_APPLICATION);

    // Cursor の設定
    if (wc2.cursor_id_ != DEFAULT_RC)
      wndcls.hCursor = LoadCursor(getHI_(), MAKEINTRESOURCE(wc2.cursor_id_));
    else
      wndcls.hCursor = LoadCursor(0, IDC_ARROW);


    // 背景色の設定
    switch (wc2.bkcolor_) {
    case BG_WHITE:
      wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
      break;

    case BG_BLACK:
      wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
      break;

    case BG_GRAY:
      wndcls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
      break;

    case BG_LTGRAY:
      wndcls.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
      break;

    case BG_DKGRAY:
      wndcls.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
      break;

    case BG_APPWORKSPACE:
      wndcls.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
      break;

    case BG_BKWINDOW:
      wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      break;

    default:
      wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
      break;
    }

    // 実際に登録
    ::RegisterClass(&wndcls);

    vwc_S.push_back(wc2);
    return wc2.wcname_;
  }
  else
    return it->wcname_; // WNDCLASS名を返す
}


/*=========================================
 *  BasicWindow::createWindow0_()
 *  ウィンドウの生成
 *=======================================*/
void urania::BasicWindow::createWindow0_(const urania::BasicWindow::D0_& de)
{
  HWND hwnd;
  int style = WS_CLIPCHILDREN;

  // WNDCLASSが未登録なら登録する
  WC_ wc;
  wc.icon_id_ = de.icon;
  wc.cursor_id_ = de.cursor;
  wc.bkcolor_ = de.bkcolor;
  if (de.winproc)
    wc.proc_ = de.winproc;
  else
    wc.proc_ = winproc_;
  std::wstring wcname = registWC_(wc);

  // Window Style の設定
  if (de.can_resize)
    style |= WS_THICKFRAME;
  if (de.v_scrollbar)
    style |= WS_VSCROLL;
  if (de.h_scrollbar)
    style |= WS_HSCROLL;
  if (de.maxbox)
    style |= WS_MAXIMIZEBOX;
  if (de.minbox)
    style |= WS_MINIMIZEBOX;

  // 子Windowになるかどうかで変わる設定
  if (de.pwnd && de.hm) {
    style |= WS_CHILD | WS_CLIPSIBLINGS;
    if (!de.popup && !de.border_only)
      style |= WS_CAPTION | WS_SYSMENU;
    else if (de.border_only)
      style |= WS_BORDER;
  }
  else {
    if (!de.border_only && !de.popup)
      style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    else if (de.border_only)
      style |= WS_POPUP | WS_BORDER | WS_SYSMENU;
    else
      style |= WS_POPUP;
  }

  // Windowを実際に生成・可視化・更新
  hwnd
    = ::CreateWindow(
        wcname.c_str(), de.title.c_str(), style, de.x, de.y, de.w, de.h,
        de.pwnd, de.hm, getHI_(), (void*)this);

  ::ShowWindow(hwnd, SW_SHOW);
  ::UpdateWindow(hwnd);
}


/*===================================================================
 *  BasicWindow::winproc_()
 *  BasicWindowと結合しているHWND用のWindow Procedure
 *=================================================================*/
LRESULT CALLBACK urania::BasicWindow::winproc_(
  HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
  BasicWindow* pw = (BasicWindow*)GetWindowLongPtr(hw, 0);

  // pwが設定されてないときの処理
  if (!pw) {
    if (msg != WM_CREATE)
      return ::DefWindowProc(hw, msg, wp, lp);

    LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lp;
    BasicWindow* p= (BasicWindow*)(lpcs->lpCreateParams);
    p->init_(hw);
    p->wproc_(msg, wp, lp);

    return 0;
  }

  if (msg == WM_DESTROY) {
    pw->wproc_(msg, wp, lp);
    pw->destroyed_();
    return 0;
  }
  else
    return pw->wproc_(msg, wp, lp);
}


/*=======================================
 *  BasicWindow::defHandler()
 *  デフォルトのメッセージ処理
 *=====================================*/
LRESULT urania::BasicWindow::defHandler(UINT msg, WPARAM wp, LPARAM lp)
{
  return ::DefWindowProc(hw_, msg, wp, lp);
}



/*========================================================
 *  BasicWindow::getPaintDevice()
 *  WindowのDCに対応したPaintDeviceを生成
 *======================================================*/
namespace
{
  void releasePaint_(HDC dc, void* app)
  {
    ::ReleaseDC((HWND)app, dc);
  }
}

std::unique_ptr<urania::PaintDevice> urania::BasicWindow::getPaintDevice()
{
  RECT rc;
  ::GetClientRect(hw_, &rc);
  return
    PaintDevice::create(GetDC(hw_), releasePaint_, hw_, rc.right, rc.bottom);
}



//eof
