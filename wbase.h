/**************************************************************************
 *
 *  wbase.h
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Window & Dialog BASE class
 *  Window, Dialog 共通の HWND 管理用基底クラス
 *
 *  履歴
 *    2016.2.27  修正
 *************************************************************************/
#ifndef INC_URANIA_WINDOW_BASE_H___
#define INC_URANIA_WINDOW_BASE_H___

#include <boost/utility.hpp>
#include <polymnia/ibuf.h>
#include "system.h"


/*-------------------------------------------
 *  class WndBase
 *  Window と Dialog の基底クラス
 *  HWND の管理・操作等を行う
 *-----------------------------------------*/
class urania::WndBase : boost::noncopyable
{
  friend class urania::System;
  friend class urania::CommonDialogBase;

//private:
//  typedef urania::WndBase* P_;

protected:
  HWND hw_;

private:
  bool dst_;  // trueなら破棄時にWindowをDestroy

public:
  WndBase() : hw_(NULL), dst_(false) {}
  virtual ~WndBase() {}


protected:
  //==============================================
  //  link__()
  //  Object と HWND を強固に連結
  //  Object 破棄時に HWND も破棄するようにする
  //==============================================
  void link__(HWND h)
  {
    attach__(h);
    if (hw_)
      dst_ = true;
  }

  //=============================================
  //  kill__()
  //  HWNDとObjectを切り離す
  //  もしlink__()されていればHWNDを破棄する
  //=============================================
  void kill__()
  {
    if (dst_ && hw_)
      ::DestroyWindow(hw_);
    detach__();
  }

  //==========================================
  //  attach__()
  //  Object と HWND を一時的に(?)連結
  //  Object 破棄時に HWND は破棄しない
  //==========================================
  void attach__(HWND h)
  {
    kill__();
    hw_ = h;
  }

  //========================================
  //  detach__()
  //  連結を切る HWNDの破棄はしない
  //========================================
  void detach__()
  {
    hw_ = NULL;
    dst_ = false;
  }



  //////////////////////////////////////
  //  派生クラスで定義するハンドラ
  //////////////////////////////////////
  virtual void init__(HWND) =0;
  virtual void uninit__() =0;
  virtual void destroyWindow__() =0;

  //========================================
  //  deleting__()
  //  アプリ側から HWND を破棄する
  //========================================
  void deleting__()
  {
    if (hw_)
    {
      uninit__();
      destroyWindow__();
    }
  }

  //==================================================
  //  destroyed__()
  //  システム側が HWND を破棄したときの後処理
  //==================================================
  void destroyed__()
  {
    if (hw_)
    {
      uninit__();
      detach__();
    }
  }


  //===========================================
  //  派生クラスで使う "カプセル破り"
  //===========================================
  static HINSTANCE getHI__()
  {
    return System::hi_S;
  }

  static HWND getHW__(urania::WndBase* wb)
  {
    return wb->hw_;
  }

public:
  /////////////////////////////////////
  //  HWND取得
  /////////////////////////////////////
  static HWND getHWND(urania::WndBase* wb)
  {
    return wb->hw_;
  }

public:
  ///////////////////////////////////
  //  Window本体操作系
  ///////////////////////////////////

  // 破棄
  void destroy() { destroyWindow__(); }

  // タイトル変更
  void resetTitle(const std::wstring& ttl)
  {
    if (hw_)
      ::SendMessage(hw_, WM_SETTEXT, 0, (ULONG_PTR)(ttl.c_str()));
  }

  // 窓を閉じる
  void close()
  {
    if (hw_)
      ::PostMessage(hw_, WM_CLOSE, 0, 0);
  }

  // 窓を表示
  void show()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_SHOW);
  }

  // 窓を隱す
  void hide()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_HIDE);
  }

  // 窓を最大化
  void maximize()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_MAXIMIZE);
  }

  // 窓を最小化
  void minimize()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_MINIMIZE);
  }

  // 窓を「元に戻」す
  void normalize()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_RESTORE);
  }

  //======================================
  //  move()
  //  窓を移動
  // (x, y) は左上の位置
  //======================================
  void move(int x, int y)
  {
    if (hw_)
      ::SetWindowPos(hw_, NULL, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
  }

  //=======================
  // postMessage()
  // メッセージを投げる
  //=======================
  void postMessage(UINT msg, WPARAM wp, LPARAM lp)
  {
    if (hw_)
      ::PostMessage(hw_, msg, wp, lp);
  }


  /////////////////////////////////////////
  //  Window本体情報取得系
  /////////////////////////////////////////

  int getWidth()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetWindowRect(hw_, &rc);
    return rc.right - rc.left;
  }

  int width() { return getWidth(); }

  int getHeight()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetWindowRect(hw_, &rc);
    return rc.bottom - rc.top;
  }

  int height() { return getHeight(); }

  bool getWidthAndHeight(int* w, int* h)
  {
    if (!hw_)
      return false;
    RECT rc;
    ::GetWindowRect(hw_, &rc);
    *w = rc.right - rc.left;
    *h = rc.bottom - rc.top;
    return true;
  }

  polymnia::Rect getRect()
  {
    polymnia::Rect re(0, 0, 0, 0);
    if (hw_)
    {
      RECT rc;
      ::GetWindowRect(hw_, &rc);
      re.x2 = rc.right - rc.left;
      re.y2 = rc.bottom - rc.top;
    }
    return re;
  }


  ///////////////////////////////////////
  //  コントロールの有效化/無效化
  ///////////////////////////////////////
  void enableCtrl(int id)
  {
    HWND w = GetDlgItem(hw_, id);
    ::EnableWindow(w, TRUE);
  }

  void disableCtrl(int id)
  {
    HWND w = GetDlgItem(hw_, id);
    ::EnableWindow(w, FALSE);
  }

  //////////////////////////////////////////////
  //  コントロール生成系
  //////////////////////////////////////////////
  void createEditBox(int id, const urania::CtrlDesc& de)
  {
    CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
  }

  void createMultiLineEditBox(int id, const urania::CtrlDesc& de)
  {
    CreateWindow(L"EDIT", L"",
      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_NOHIDESEL |
      ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
  }

  void createListBox(int id, const urania::CtrlDesc& de)
  {
    CreateWindow(
      L"LISTBOX", L"",
      WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_DISABLENOSCROLL | WS_VSCROLL,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
  }

  void createComboBox(int id, const urania::CtrlDesc& de)
  {
    CreateWindow(L"COMBOBOX", L"",
      WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DISABLENOSCROLL
      | CBS_DROPDOWNLIST, de.x, de.y, de.w, de.h, hw_,
      (HMENU)id, getHI__(), nullptr);
  }

  void createPushButton(
    int id, const std::wstring& str, const urania::CtrlDesc& de)
  {
    CreateWindow(
      L"BUTTON", str.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
  }

  void createLabel(int id, const std::wstring& str, const urania::CtrlDesc& de)
  {
    CreateWindow(
      L"STATIC", str.c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
  }
  
  void createCheckBox(
    int id, const std::wstring& str, const urania::CtrlDesc& de)
  {
    CreateWindow(
      L"BUTTON", str.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
  }

  void createActiveButton(
    int id, const std::wstring& str, const urania::CtrlDesc& de);



  ///////////////////////////////
  //  EditBox操作系
  ///////////////////////////////
  std::wstring getTextEB(int id);

  void setEBText(int id, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_SETTEXT, 0, (LPARAM)(txt.c_str()));
  }

  void clearEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_CLEAR, 0, 0);
  }

  void copyEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_COPY, 0, 0);
  }

  void cutEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_CUT, 0, 0);
  }

  void pasteEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_PASTE, 0, 0);
  }

  bool canUndoEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    if (::SendMessage(w, EM_CANUNDO, 0, 0))
      return true;
    else
      return false;
  }

  void undoEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, EM_UNDO, 0, 0);
  }

  std::wstring getLineTextEB(int id, int li);

  int countLineEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    return ::SendMessage(w, EM_GETLINECOUNT, 0, 0);
  }


  ///////////////////////////////////
  //  ListBox操作系
  ///////////////////////////////////
  std::wstring getItemLB(int id, int no);

  void addItemLB(int id, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_ADDSTRING, 0, (LPARAM)(txt.c_str()));
  }

  void insertItemLB(int id, int no, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_INSERTSTRING, no, (LPARAM)(txt.c_str()));
  }

  void deleteItemLB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_DELETESTRING, no, 0);
  }

  int countItemLB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    return ::SendMessage(w, LB_GETCOUNT, 0, 0);
  }

  int getCurrentLB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    int r = ::SendMessage(w, LB_GETCURSEL, 0, 0);
    if (r == LB_ERR)
      return -1;
    else
      return r;
  }

  void setCurrentLB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_SETCURSEL, no, 0);
  }

  void clearLB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_RESETCONTENT, 0, 0);
  }

  void dirLB(int id, const std::wstring& path, int flag);

  ////////////////////////////
  //  ComboBox操作系
  ////////////////////////////
  std::wstring getItemCB(int id, int no);

  void addItemCB(int id, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w,CB_ADDSTRING, 0, (LPARAM)(txt.c_str()));
  }

  void insertItemCB(int id, int no, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w,CB_INSERTSTRING,no,(LPARAM)(txt.c_str()));
  }

  void deleteItemCB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w,CB_DELETESTRING,no,0);
  }

  int countItemCB(int id)
  {
    HWND w = ::GetDlgItem(hw_,id);
    return ::SendMessage(w,CB_GETCOUNT,0,0);
  }

  int getCurrentCB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    int r = ::SendMessage(w, CB_GETCURSEL, 0, 0);
    if (r == CB_ERR)
      return -1;
    else
      return r;
  }

  void setCurrentCB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, CB_SETCURSEL, no, 0);
  }

  void clearCB(int id)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w, CB_RESETCONTENT, 0, 0);
  }

  void dirCB(int id, const std::wstring& path, int flag);


  //================================
  //  CheckBox 等の状態取得
  //================================
  bool isChecked(int id)
  {
    return IsDlgButtonChecked(hw_, id);
  }


  ////////////////////////////////////////
  //  横スクロールバー操作系
  ////////////////////////////////////////
  int getPosHSB()
  {
    return getPosSB(ID_SBH);
  }

  void setPosHSB(int pos)
  {
    setPosSB(ID_SBH, pos);
  }

  void setRangeHSB(int min, int max)
  {
    if (hw_)
      ::SetScrollRange(hw_, SB_HORZ, min, max, TRUE);
  }

  void getRangeHSB(int& min, int& max)
  {
    if (hw_)
      ::GetScrollRange(hw_, SB_HORZ, &min, &max);
  }

  void enableHSB()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_HORZ, ESB_ENABLE_BOTH);
  }

  void disableHSB()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_HORZ, ESB_DISABLE_BOTH);
  }


  ///////////////////////////////////////////////
  //  縦スクロールバー操作系
  ///////////////////////////////////////////////
  int getPosVSB()
  {
    return getPosSB(ID_SBV);
  }

  void setPosVSB(int pos)
  {
    setPosSB(ID_SBV, pos);
  }

  void setRangeVSB(int min, int max)
  {
    if (hw_)
      ::SetScrollRange(hw_, SB_VERT, min, max, TRUE);
  }

  void getRangeVSB(int& min, int& max)
  {
    if (hw_)
      ::GetScrollRange(hw_, SB_VERT, &min, &max);
  }

  void enableVSB()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_VERT, ESB_ENABLE_BOTH);
  }

  void disableVSB()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_VERT, ESB_DISABLE_BOTH);
  }

  /////////////////////////////////////////////////////
  //  共通 (2012.5.13以降追加; 2016.2.27改名)
  /////////////////////////////////////////////////////
  int getPosSB(int id);
  void setPosSB(int id, int pos);
  void setRangeSB(int id, int min, int max, int page);

  ////////////////////////////////////////////////
  // ID付スクロールバー用 (2016.2.27再實裝)
  ////////////////////////////////////////////////
  void enableSB(int id)
  {
    if (!hw_)
      return;
    HWND w = ::GetDlgItem(hw_, id);
    ::EnableScrollBar(w, SB_CTL, ESB_ENABLE_BOTH);
  }

  void disableSB(int id)
  {
    if (!hw_)
      return;
    HWND w = ::GetDlgItem(hw_, id);
    ::EnableScrollBar(w, SB_CTL, ESB_DISABLE_BOTH);
  }


  /////////////////////////////////////////////
  //  ID付きスクロールバー操作系
  //  (2012.5.13以降削除)
  /////////////////////////////////////////////
  /*
  void setIDSBarRange(int id, int min, int max)
  {
    if (!hw_)
      return;
    HWND w = ::GetDlgItem(hw_, id);
    ::SetScrollRange(w, SB_CTL, min, max, TRUE);
  }

  void getIDSBarRange(int id, int& min, int& max)
  {
    if (!hw_)
      return;
    HWND w = ::GetDlgItem(hw_, id);
    ::GetScrollRange(w, SB_CTL, &min, &max);
  }
  */
};



#endif
//eof
