/**************************************************************************
 *
 *  wbase.h
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Window & Dialog BASE class
 *  Window, Dialog 共通の HWND 管理用基底クラス
 *
 *  last update: 25 Jan MMXIV
 *************************************************************************/
#ifndef INC_URANIA_WBASE_H___
#define INC_URANIA_WBASE_H___

#include "sys.h"
#include <boost/utility.hpp>
#include <polymnia/ibuf.h>


/*-------------------------------------------
 *  class WndBase
 *  Window と Dialog の基底クラス
 *  HWND の管理・操作等を行う
 *-----------------------------------------*/
class urania::WndBase : boost::noncopyable
{
  friend class urania::System;
  friend class urania::CommonDialogBase;

private:
  typedef urania::WndBase* P_;

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

  static HWND getHW__(P_ wb)
  {
    return wb->hw_;
  }

public:
  /////////////////////////////////////
  //  HWND取得
  /////////////////////////////////////
  static HWND getHWND(P_ wb)
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
  void enableCtrl(int id);
  void disableCtrl(int id);

  //////////////////////////////////////////////
  //  コントロール生成系
  //////////////////////////////////////////////
  void createSEditBox(int id, const urania::CtrlDesc& de);
  void createMEditBox(int id, const urania::CtrlDesc& de);
  void createListBox(int id, const urania::CtrlDesc& de);
  void createComboBox(int id, const urania::CtrlDesc& de);
  void createPushButton(
    int id, const std::wstring& str, const urania::CtrlDesc& de);
  void createLabel(int id, const std::wstring& str, const urania::CtrlDesc& de);
  void createCheckBox(
    int id, const std::wstring& str, const urania::CtrlDesc& de);
  void createActiveButton(
    int id, const std::wstring& str, const urania::CtrlDesc& de);



  ///////////////////////////////
  //  EditBox操作系
  ///////////////////////////////
  std::wstring getEBText(int id);
  void setEBText(int id, const std::wstring& txt);
  void clearEB(int id);
  void copyEB(int id);
  void cutEB(int id);
  void pasteEB(int id);
  bool canUndoEB(int id);
  void undoEB(int id);
  std::wstring getEBLineText(int id, int li);
  int countEBLine(int id);


  ///////////////////////////////////
  //  ListBox操作系
  ///////////////////////////////////
  std::wstring getLBItem(int id, int no);
  void addLBItem(int id, const std::wstring& txt);
  void insertLBItem(int id, int no, const std::wstring& txt);
  void deleteLBItem(int id, int no);
  int countLB(int id);
  int getCurrentLB(int id);
  void setCurrentLB(int id, int no);
  void resetLB(int id);
  void dirLB(int id, const std::wstring& path, int flag);

  ////////////////////////////
  //  ComboBox操作系
  ////////////////////////////
  std::wstring getCBItem(int id, int no);
  void addCBItem(int id, const std::wstring& txt);
  void insertCBItem(int id, int no, const std::wstring& txt);
  void deleteCBItem(int id, int no);
  int countCB(int id);
  int getCurrentCB(int id);
  void setCurrentCB(int id, int no);
  void resetCB(int id);
  void dirCB(int id, const std::wstring& path, int flag);


  //================================
  //  CheckBox 等の状態取得
  //================================
  bool isChecked(int id);


  ////////////////////////////////////////
  //  横スクロールバー操作系
  ////////////////////////////////////////
  int getHSBarPos() { return getSBPos(ID_SBH); }
  void setHSBarPos(int pos) { setSBPos(ID_SBH, pos); }

  void setHSBarRange(int min, int max)
  {
    if (hw_)
      ::SetScrollRange(hw_, SB_HORZ, min, max, TRUE);
  }

  void getHSBarRange(int& min, int& max)
  {
    if (hw_)
      ::GetScrollRange(hw_, SB_HORZ, &min, &max);
  }

  void enableHSBar()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_HORZ, ESB_ENABLE_BOTH);
  }

  void disableHSBar()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_HORZ, ESB_DISABLE_BOTH);
  }


  ///////////////////////////////////////////////
  //  縦スクロールバー操作系
  ///////////////////////////////////////////////
  int getVSBarPos() { return getSBPos(ID_SBV); }
  void setVSBarPos(int pos) { setSBPos(ID_SBV, pos); }

  void setVSBarRange(int min, int max)
  {
    if (hw_)
      ::SetScrollRange(hw_, SB_VERT, min, max, TRUE);
  }

  void getVSBarRange(int& min, int& max)
  {
    if (hw_)
      ::GetScrollRange(hw_, SB_VERT, &min, &max);
  }

  void enableVSBar()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_VERT, ESB_ENABLE_BOTH);
  }

  void disableVSBar()
  {
    if (hw_)
      ::EnableScrollBar(hw_, SB_VERT, ESB_DISABLE_BOTH);
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

  void enableIDSBar(int id)
  {
    if (!hw_)
      return;
    HWND w = ::GetDlgItem(hw_, id);
    ::EnableScrollBar(w, SB_CTL, ESB_ENABLE_BOTH);
  }

  void disableIDSBar(int id)
  {
    if (!hw_)
      return;
    HWND w = ::GetDlgItem(hw_, id);
    ::EnableScrollBar(w, SB_CTL, ESB_DISABLE_BOTH);
  }
  */

  //////////////////////////////
  //  共通 (2012.5.13以降追加)
  //////////////////////////////
  int getSBPos(int id);
  void setSBPos(int id, int pos);
  void setSBRange(int id, int min, int max, int page);
};



#endif
//eof
