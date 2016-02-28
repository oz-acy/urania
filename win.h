/**************************************************************************
 *
 *  win.h
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  ウィンドウ
 *
 *  履歴
 *    2016.2.28  修正
 */
#ifndef INC_URANIA_WINDOW_H___
#define INC_URANIA_WINDOW_H___

#include <map>
#include "bwin.h"
#include "menu.h"


namespace urania
{

struct WndMessage;
class WMHandler;
class WindowFactory;


} // end of namesace urania


/*--------------------------------------
 *  WndMessage
 *  メッセージ引き渡し用構造體
 */
struct urania::WndMessage
{
  urania::Window* window;
  UINT id;
  WPARAM wparam;
  LPARAM lparam;
};


/*------------------------------------------------
 * WMsgHandler
 */
class urania::WMHandler : boost::noncopyable
{
public:
  typedef void (*CmdHandler)(Window*);

private:
  std::map<int, CmdHandler> cmap_;

public:
  virtual ~WMHandler() {}

  virtual bool onDestroy() { return false; }
  virtual bool onSize(Window*, int typ, int w, int h) { return false; }
  virtual bool onKeyDown(Window*, int code, int rep, bool prev)
    { return false; }

  virtual void onPaint(BasicWindow*, PaintDevice*) {}
  virtual void onScroll(Window*, int id, int pos) {}
  virtual void onMouseWheel(Window* w, int delta, int keys, int x, int y) {}

  virtual void onDropFiles(
    Window* w, std::vector<std::wstring>& fa, int x, int y)
    {}


  void set(int cmdid, CmdHandler c) { cmap_[cmdid] = c; }
  LRESULT operator()(urania::WndMessage* msg);

private:
  void onDropFiles_(urania::Window*, WPARAM wp);
  void onScroll_(urania::Window*, UINT msg, WPARAM, LPARAM);
};




/*--------------------------------------
 *  Window
 *  Windowクラス
 */
class urania::Window : public urania::BasicWindow
{
  friend class urania::WindowFactory;

public:
  typedef LRESULT (*H_)(urania::Window* pw, UINT msg, WPARAM wp, LPARAM lp);

protected:
  urania::WMHandler* msgHandler_;
  urania::RCP_Menu menu_; // 関連づけられたメニュー
  bool dad_;  // trueなら Drag&Drop受け付ける


protected:
  Window() : msgHandler_(nullptr), dad_(false) {}

  void init__(HWND hw);
  void uninit__();


public:
  ~Window()
  {
    deleting__();
    delete msgHandler_;
  }

  urania::RCP_Menu getMenu() const { return menu_; }
  void setMenu(const urania::RCP_Menu& m) { linkMenu__(m); }

protected:
  virtual LRESULT wproc__(UINT msg, WPARAM wp, LPARAM lp);

  static HMENU getHM__(const urania::RCP_Menu& mn) { return mn->hmenu_; }

  HMENU linkMenu__(const urania::RCP_Menu& mn)
  {
    menu_ = mn;
    return mn->giveHM__();
  }
};




/*-------------------------
 *  class WindowFactory
 *  Window生成器
 *-----------------------*/
class urania::WindowFactory
{
public:
  std::wstring title;
  int x, y, w, h;

  unsigned icon;
  unsigned cursor;
  unsigned bkcolor;

  bool drag_and_drop;
  bool maxbox;
  bool minbox;
  bool h_scrollbar;
  bool v_scrollbar;
  bool popup;
  bool border_only;
  bool resizeable;

public:
  WindowFactory()
    : x(CW_USEDEFAULT), w(CW_USEDEFAULT),
      icon(DEFAULT_RC), cursor(DEFAULT_RC), bkcolor(BG_WHITE),
      drag_and_drop(false), maxbox(false), minbox(false),
      h_scrollbar(false), v_scrollbar(false), popup(false),
      border_only(false), resizeable(false)
  {}

  urania::Window* create(urania::WMHandler* mh, int menu =0)
  {
    return factory__(mh, menu, nullptr, 0);
  }

  urania::Window*
  createAsOwned(
    urania::BasicWindow* owner, urania::WMHandler* mh, int menu =0)
  {
    return factory__(mh, menu, owner, 0);
  }

  urania::Window*
  createAsChild(urania::BasicWindow* parent, int cid, urania::WMHandler* mh)
  {
    return factory__(mh, 0, parent, cid);
  }

private:
  urania::Window* factory__(
    urania::WMHandler* mh, int menu, urania::BasicWindow* par, int cid);
};




#endif // INC_URANIA_WINDOW_H___
