/*
 * Copyright 2002-2021 oZ/acy (名賀月晃嗣)
 * Redistribution and use in source and binary forms, 
 *     with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * @file win.cpp
 * @author oZ/acy (名賀月晃嗣)
 *
 * @date 2016.2.29  修正 (クロージャの實裝をbindから[this](){}に變更)
 * @date 2019.8.30  修正 豫約されてゐる識別子に該當してゐたものを修正
 *
 */
#include <cstdlib>
#include <algorithm>
#include <windowsx.h>
#include "window.h"

/*====================================
 *  ウィンドウ生成の下請け
 */
//urania::Window*
std::unique_ptr<urania::Window>
urania::WindowFactory::factory_(
  std::unique_ptr<urania::WMHandler>&& hnd,
  int menu, urania::BasicWindow* par, int cid)
{
  std::unique_ptr<Window> res(new Window);

  // ハンドラマネージャとD&Dの設定
  res->msgHandler_ = std::move(hnd);
  res->dad_ = drag_and_drop;

  // BasicWindow記述構造体の構築
  BasicWindow::D0_ de0;

  de0.title = title;
  de0.x = x;
  de0.y = y;
  de0.w = w;
  de0.h = h;
  de0.icon = icon;
  de0.cursor = cursor;
  de0.bkcolor = bkcolor;
  de0.can_resize = resizeable;
  de0.v_scrollbar = v_scrollbar;
  de0.h_scrollbar = h_scrollbar;
  de0.minbox = minbox;
  de0.maxbox = maxbox;
  de0.popup = popup;
  de0.border_only = border_only;

  // 親又はオーナーWindowとMenu又は子WindowIDの設定
  if (par)
    de0.pwnd = res->getHW_(par);

  if (par && cid)
    de0.hm = reinterpret_cast<HMENU>(cid);
  else if (menu)
    de0.hm = res->linkMenu_(Menu::create(menu));

  // 実際に生成
  res->createWindow0_(de0);

  return res;
}


// Window用「ウィンドウプロシージャ」
// BasicWindow::winproc_()から呼び出され、ハンドラを呼び出す。
LRESULT urania::Window::wproc_(UINT msg, WPARAM wp, LPARAM lp)
{
  if (msgHandler_) {
    urania::WndMessage m;
    m.window = this;
    m.id = msg;
    m.wparam = wp;
    m.lparam = lp;
    return (*msgHandler_)(&m);
  }
  else
    return defHandler(msg, wp, lp);
}


// 初期化處理
// 
// Window生成時に呼び出され、WindowオブジェクトとHWNDを結合する。
//
// より具體的には、
// BasicWindow::winproc_()がWM_CREATEを處理するときに呼び出される。
void urania::Window::init_(HWND hw)
{
  // HWNDとWindow objectの結合
  bindHWND_(hw);

  // Drag and Dropの設定
  if (dad_)
    ::DragAcceptFiles(hw, TRUE);
}


// 未初期化處理
//
// ウィンドウ破棄時に呼び出される。
//
// より具體的には、
// (1) BasicWindow::winproc_()がWM_DESTROYを處理するとき、
// (2) Windowオブジェクトが解體されるとき、
// に、HWNDとの結合が殘つてゐるならば呼び出される。
void urania::Window::uninit_()
{
  // Drag and Dropの設定の後始末
  if (dad_)
    ::DragAcceptFiles(hw_, FALSE);

  // 関聯Menuの始末
  if (menu_)
    menu_->detach_();

  // HWND側からのリンクを切斷
  unbindHWND_();
}




LRESULT urania::WMHandler::operator()(urania::WndMessage* msg)
{
  using namespace std;
  using namespace std::placeholders;

  switch (msg->id)
  {
  case WM_DESTROY:
    if (onDestroy())
      return 0;
    break;

  case WM_COMMAND:
    {
      std::map<int, CmdHandler>::iterator jt;
      jt = cmap_.find(msg->wparam & 0x0FFFF);
      if (jt != cmap_.end()) {
        (jt->second)(msg->window);
        return 0;
      }
    }
    break;

  case WM_SIZE:
    {
      int w = msg->lparam & 0x0FFFF;
      int h = (msg->lparam >> 16) & 0x0FFFF;
      if (onSize(msg->window, msg->wparam, w, h))
        return 0;
    }
    break;

  case WM_KEYDOWN:
    {
      int rep = msg->lparam & 0x0FFFF;
      bool prev = msg->lparam & 0x01 << 30;
    
      if (onKeyDown(msg->window, msg->wparam, rep, prev))
        return 0;
    }
    break;

  case WM_LBUTTONDOWN:
    {
      int x = GET_X_LPARAM(msg->lparam);
      int y = GET_Y_LPARAM(msg->lparam);
      bool ctrl = msg->wparam & MK_CONTROL;
      bool shft = msg->wparam & MK_SHIFT;
      bool lb = msg->wparam & MK_LBUTTON;
      bool mb = msg->wparam & MK_MBUTTON;
      bool rb = msg->wparam & MK_RBUTTON;
      if (onLButtonDown(msg->window, x, y, ctrl, shft, lb, mb, rb))
        return 0;
    }
    break;

  case WM_MBUTTONDOWN:
    {
      int x = GET_X_LPARAM(msg->lparam);
      int y = GET_Y_LPARAM(msg->lparam);
      bool ctrl = msg->wparam & MK_CONTROL;
      bool shft = msg->wparam & MK_SHIFT;
      bool lb = msg->wparam & MK_LBUTTON;
      bool mb = msg->wparam & MK_MBUTTON;
      bool rb = msg->wparam & MK_RBUTTON;
      if (onMButtonDown(msg->window, x, y, ctrl, shft, lb, mb, rb))
        return 0;
    }
    break;

  case WM_RBUTTONDOWN:
    {
      int x = GET_X_LPARAM(msg->lparam);
      int y = GET_Y_LPARAM(msg->lparam);
      bool ctrl = msg->wparam & MK_CONTROL;
      bool shft = msg->wparam & MK_SHIFT;
      bool lb = msg->wparam & MK_LBUTTON;
      bool mb = msg->wparam & MK_MBUTTON;
      bool rb = msg->wparam & MK_RBUTTON;
      if (onRButtonDown(msg->window, x, y, ctrl, shft, lb, mb, rb))
        return 0;
    }
    break;

  case WM_PAINT:
    return
      msg->window->onPaint(
        [this](BasicWindow* bw, PaintDevice* pd){ this->onPaint(bw, pd); },
        msg->wparam, msg->lparam);
    break;

  case WM_MOUSEWHEEL:
    {
      int d = GET_WHEEL_DELTA_WPARAM(msg->wparam);
      int k = GET_KEYSTATE_WPARAM(msg->wparam);
      int x = GET_X_LPARAM(msg->lparam);
      int y = GET_Y_LPARAM(msg->lparam);
      onMouseWheel(msg->window, d, k, x, y);
    }
    return 0;
    break;

  case WM_HSCROLL:
  case WM_VSCROLL:
    onScroll_(msg->window, msg->id, msg->wparam, msg->lparam);
    return 0;
    break;

  case WM_DROPFILES:
    onDropFiles_(msg->window, msg->wparam);
    return 0;
    break;
  }

  return msg->window->defHandler(msg->id, msg->wparam, msg->lparam);
}




//eof
