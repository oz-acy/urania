/**************************************************************************
 *
 *  win.h
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  WINdow class
 *
 *  last update: 25 Jan MMXIV
 */
#ifndef INC_URANIA_WINDOW_H___
#define INC_URANIA_WINDOW_H___

#include "bwin.h"
#include "menu.h"
#include <map>


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

private:
  typedef urania::WndMessage Msg_;

public:
  typedef urania::Window W_;
  typedef W_* PW_;
  typedef LRESULT (*H_)(PW_ pw, UINT msg, WPARAM wp, LPARAM lp);


protected:
  urania::WMHandler* msgHandler_;
  urania::RCP_Menu menu_; // 関連づけられたメニュー
  bool dad_;  // trueなら Drag&Drop受け付ける


protected:
  Window() : dad_(false), msgHandler_(nullptr) {}

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
    : icon(DEFAULT_RC), cursor(DEFAULT_RC), bkcolor(BG_WHITE),
      x(CW_USEDEFAULT), w(CW_USEDEFAULT),
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
