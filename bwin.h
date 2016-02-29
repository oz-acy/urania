/**************************************************************************
 *
 *  bwin.h
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Basic WINdow  for part "URANIA"
 *  Window用基底クラス
 *
 *  履歴
 *    2016.2.29  修正
 *************************************************************************/

#ifndef INC_URANIA_BASICWINDOW_H___
#define INC_URANIA_BASICWINDOW_H___

#include <vector>
#include "wbase.h"
#include "paintdev.h"


/*---------------------------------------
 *  class BasicWindow
 *  Window管理・操作用基底クラス
 *-------------------------------------*/
class urania::BasicWindow : public urania::WndBase
{
//public:
//  typedef urania::BasicWindow BW_;
//  typedef BW_* PBW_;


public:
  //-------------------
  //  struct D0_
  //  記述構造体
  //-------------------
  struct D0_
  {
    std::wstring title;  /* タイトル */
    HWND pwnd;  /* 親ウィンドウ */
    HMENU hm;  /* メニュー */
    WNDPROC winproc;  /* ウィンドウプロシージャ */

    int x, y, w, h;  /* ウィンドウの位置、大きさ */

    unsigned icon;  /* アイコンのリソース番号 */
    unsigned cursor;  /* カーソルのリソース番号 */
    unsigned bkcolor;  /* 背景色の番号(?) */

    bool maxbox;  /* 最大化ボックス付き? */
    bool minbox;  /* 最小化ボックス付き? */
    bool h_scrollbar;  /* 横スクロールバー付き? */
    bool v_scrollbar;  /* 縦スクロールバー付き? */
    bool popup;  /* ポップアップウィンドウ? */
    bool border_only;  /* 境界だけ? */
    bool can_resize;  /* リサイズ可能? */


    D0_()
    : pwnd(nullptr), hm(nullptr), winproc(nullptr),
      x(CW_USEDEFAULT), w(CW_USEDEFAULT),
      icon(DEFAULT_RC), cursor(DEFAULT_RC), bkcolor(BG_WHITE),
      maxbox(false), minbox(false),
      h_scrollbar(false), v_scrollbar(false), popup(false),
      border_only(false), can_resize(false)
    {}
  };


private:
  //----------------------------------
  //  struct WC_
  //  WNDCLASS 生成情報格納クラス
  //--------------------------------//
  struct WC_
  {
    unsigned icon_id_;
    unsigned cursor_id_;
    unsigned bkcolor_;
    WNDPROC proc_;
    std::wstring wcname_;

    //========================================================================
    //  WNDCLASS 登録の要不要を決めるための operator==() と operator!=()
    //========================================================================
    bool operator==(const WC_& my) const
    {
      if (icon_id_ != my.icon_id_)
        return false;
      if (cursor_id_ != my.cursor_id_)
        return false;
      if (bkcolor_ != my.bkcolor_)
        return false;
      if (proc_ != my.proc_)
        return false;

      return true;
    }

    bool operator!=(const WC_& my) const
    {
      return !(*this == my);
    }
  };


private:
  static std::vector<WC_> vwc_S;  // WNDCLASS情報ベクタ

protected:
  BasicWindow() {}

  void createWindow0__(const D0_& de);

  //===========================================
  //  bindHWND__()
  //  BasicWindow と HWND の二重結合を形成
  //===========================================
  void bindHWND__(HWND hw)
  {
    link__(hw);
    SetWindowLongPtr(hw, 0, reinterpret_cast<ULONG_PTR>(this));
  }

  //==========================================
  //  unbindHWND__()
  //  HWND から BasicWindow への結合を切斷
  //==========================================
  void unbindHWND__()
  {
    if (hw_)
      SetWindowLongPtr(hw_, 0, reinterpret_cast<ULONG_PTR>(nullptr));
  }

  void destroyWindow__()
  {
    kill__();
  }

  virtual LRESULT wproc__(UINT msg, WPARAM wp, LPARAM lp) =0;
  static LRESULT CALLBACK winproc__(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

  static std::wstring registWC__(const WC_& wc);


public:
  ~BasicWindow() {}


  void update()
  {
    if (hw_)
      ::UpdateWindow(hw_);
  }

  void invalidate()
  {
    if (hw_)
      ::InvalidateRect(hw_, nullptr, FALSE);
  }

  void invalidate(const polymnia::Rect& rect)
  {
    if (hw_)
    {
      ::RECT r;
      r.left = rect.x1;
      r.right = rect.x2;
      r.top = rect.y1;
      r.bottom = rect.y2;

      ::InvalidateRect(hw_, &r, FALSE);
    }
  }

  urania::PaintDevice* getPaintDevice();
  template<class PT_> LRESULT onPaint(PT_&& proc, WPARAM wp, LPARAM lp);

  int getClientWidth()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetClientRect(hw_, &rc);
    return rc.right;
  }

  int getClientHeight()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetClientRect(hw_, &rc);
    return rc.bottom;
  }

  bool getClientWidthAndHeight(int* w, int* h)
  {
    if (!hw_)
      return false;
    RECT rc;
    ::GetClientRect(hw_, &rc);
    *w = rc.right;
    *h = rc.bottom;
    return true;
  }

  polymnia::Rect getClientWidthAndHeight()
  {
    polymnia::Rect re(0, 0, 0, 0);
    if (hw_)
    {
      RECT rc;
      ::GetClientRect(hw_, &rc);
      re.x2 = rc.right;
      re.y2 = rc.bottom;
    }
    return re;
  }

  void resize(int w, int h)
  {
    if (hw_)
      ::SetWindowPos(hw_, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
  }

  void resizeScreen(int w, int h)
  {
    if (!hw_)
      return;
    RECT wr, cr;
    ::GetWindowRect(hw_, &wr);
    ::GetClientRect(hw_, &cr);
    w += wr.right - wr.left - cr.right;
    h += wr.bottom - wr.top - cr.bottom;
    ::SetWindowPos(hw_, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
  }

  void setTimer(int id, int elapse) { ::SetTimer(hw_, id, elapse, nullptr); }
  void killTimer(int id) { ::KillTimer(hw_, id); }


  //==========================================================
  //  BasicWindow::defHandler()
  //  デフォルトのメッセージ処理
  //==========================================================
  virtual LRESULT defHandler(UINT msg, WPARAM wp, LPARAM lp);
};




/*=================================================================
 *  BasicWindow::onPaint()
 *  WM_PAINTに対応するためのヘルパ
 *  PaintDeviceを生成してprocに渡す
 *  ユーザ定義のハンドラから必要に応じて明示的に呼び出すこと
 *===============================================================*/
template<class PT_>
inline LRESULT urania::BasicWindow::onPaint(PT_&& proc, WPARAM wp, LPARAM lp)
{
  (void)wp;
  (void)lp;

  PAINTSTRUCT ps;
  ::BeginPaint(hw_, &ps);
  RECT rc;
  ::GetClientRect(hw_, &rc);
  PaintDevice* pd
   = PaintDevice::create(ps.hdc, nullptr, nullptr, rc.right, rc.bottom);

  proc(this, pd);

  delete pd;
  ::EndPaint(hw_, &ps);

  return 0;
}



#endif
//eof
