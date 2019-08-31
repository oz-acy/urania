/**************************************************************************
 *
 *  win.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  履歴
 *    2016.2.29  修正 (クロージャの實裝をbindから[this](){}に變更)
 *    2019.8.30  修正 豫約されてゐる識別子に該當してゐたものを修正
 */
#include <cstdlib>
#include <algorithm>
#include <functional>
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
  //res->msgHandler_.reset(mh);
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




LRESULT urania::Window::wproc_(UINT msg, WPARAM wp, LPARAM lp)
{
  if (msgHandler_)
  {
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




void urania::Window::init_(HWND hw)
{
  // HWNDとWindow objectの結合
  bindHWND_(hw);

  // Drag and Dropの設定
  if (dad_)
    ::DragAcceptFiles(hw, TRUE);
}


void urania::Window::uninit_()
{
  // Drag and Dropの設定後始末
  if (dad_)
    ::DragAcceptFiles(hw_, FALSE);

  // 関連Menuの始末
  if (!!menu_)
    menu_->detach_();

  // HWND側からのリンクを切断
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
        //bind(&WMHandler::onPaint, this, _1, _2),
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
