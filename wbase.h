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
/**
 * @file wbase.h
 * @author oZ/acy (名賀月晃嗣)
 * @brief HWND管理基底クラス
 *
 * @date 2018.2.27 修正
 * @date 2018.12.24 修正
 * @date 2019.8.30 修正
 * @date 2021.3.26 コメントを修正
 *
 * @date 2021.6.11
 *   依據ライブラリをthemis+polymniaからeunomiaに切り替へるための修正
 *
 */
#ifndef INCLUDE_GUARD_URANIA_WINDOW_BASE_H
#define INCLUDE_GUARD_URANIA_WINDOW_BASE_H

#include <eunomia/noncopyable.h>
#include <eunomia/rect.h>
#include "system.h"


/**
 * @brief HWND管理用基底クラス
 *
 * WindowとDialogの基底となり、HWNDの管理や操作を行ふ。
 */
class urania::WndBase : eunomia::Noncopyable<urania::WndBase>
{
protected:
  HWND hw_;  ///< 管理對象のHWND

private:
  bool dst_;  ///< trueであればオブジェクト解體時にHWNDを破棄する。

public:
  WndBase() noexcept : hw_(NULL), dst_(false) {}
  virtual ~WndBase() = default;


protected:
  /// @brief HWNDの強固な連結
  ///
  /// WndBaseオブジェクトとHWNDを強固に結び附け、
  /// オブジェクト破棄時にHWNDを同時に破棄する。
  /// @param h 連結するHWND
  void link_(HWND h)
  {
    attach_(h);
    if (hw_)
      dst_ = true;
  }

  /// @brief HWNDの切り離しと破棄
  ///
  /// WndBaseオブジェクトとHWNDを切り離す。
  /// もしもlink_()されてゐるならHWNDを破棄する。
  void kill_()
  {
    if (dst_ && hw_)
      ::DestroyWindow(hw_);
    detach_();
  }

  /// @brief HWNDの連結
  ///
  /// WndBaseオブジェクトとHWNDを結び附ける。
  /// オブジェクト破棄時にHWNDを破棄しない。
  void attach_(HWND h)
  {
    kill_();
    hw_ = h;
  }

  /// @brief HWNDの切り離し
  ///
  /// WndBaseオブジェクトとHWNDを切り離すが、HWNDを破棄しない。
  void detach_()
  {
    hw_ = NULL;
    dst_ = false;
  }


  /// @brief メッセージ處理系初期化
  ///
  /// ウィンドウが作成されたときに呼び出される。
  /// オブジェクトとHWNDを結合し、及びその他の處理を行ふ。
  /// 派生クラスで實裝する。
  virtual void init_(HWND) =0;

  /// @brief メッセージ處理系初期化解除
  ///
  /// ウィンドウが破棄される/たときに呼び出される。
  /// HWNDからオブジェクトへの結合を切斷し、及びその他の處理を行ふ。
  /// 派生クラスで實裝する。
  virtual void uninit_() =0;

  /// @brief ウィンドウ破棄の實處理
  ///
  /// destroy()から呼び出される。
  /// ウィンドウを破棄するための處理を、派生クラスで實裝する。
  virtual void destroyWindow_() =0;

  /// @brief オブジェクト側からHWNDを破棄
  ///
  /// (オブジェクト解體時に)連結してゐるHWNDを破棄する。
  /// インスタンス生成可能な派生クラスの解體子は、
  /// 先祖クラスの解體子がdeleting_()を呼び出す場合を除き、
  /// deleting_()を呼び出すべし。
  void deleting_()
  {
    if (hw_) {
      uninit_();
      destroyWindow_();
    }
  }

  /// @brief HWNDが破棄された時の後處理
  ///
  /// HWNDが破棄されたときに呼び出される。
  /// オブジェクトとHWNDの間の結合を切斷する。
  void destroyed_()
  {
    if (hw_) {
      uninit_();
      detach_();
    }
  }

  //===========================================
  //  派生クラスで使う "カプセル破り"
  //===========================================
  /// @brief 派生クラスがHINSTANCEを取得するための"カプセル破り"
  static HINSTANCE getHI_()
  {
    return System::hi_S;
  }


public:
  /// @brief HWNDの取得
  ///
  /// WndBaseオブジェクトに結び附いたHWNDを取得する。
  /// @param wb 對象のWndBaseオブジェクトへのポインタ
  /// @return wbに結び附いたHWND
  static HWND getHWND(const urania::WndBase* wb)
  {
    if (wb)
      return wb->hw_;
    else
      return NULL;
  }

public:
  ///////////////////////////////////
  //  Window本体操作系
  ///////////////////////////////////

  /// @brief ウィンドウの破棄
  void destroy() { destroyWindow_(); }

  /// @brief ウィンドウタイトルの變更
  /// @param ttl 新しいタイトル
  void resetTitle(const std::wstring& ttl)
  {
    if (hw_)
      ::SendMessage(hw_, WM_SETTEXT, 0, (ULONG_PTR)(ttl.c_str()));
  }

  /// @brief ウィンドウの閉止
  void close()
  {
    if (hw_)
      ::PostMessage(hw_, WM_CLOSE, 0, 0);
  }

  /// @brief ウィンドウの表示
  void show()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_SHOW);
  }

  /// @brief ウィンドウの隱蔽
  void hide()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_HIDE);
  }

  /// @brief ウィンドウの最大化
  void maximize()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_MAXIMIZE);
  }

  /// @brief ウィンドウの最小化
  void minimize()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_MINIMIZE);
  }

  /// @brief ウィンドウの復原
  void normalize()
  {
    if (hw_)
      ::ShowWindow(hw_, SW_RESTORE);
  }

  //========================================================
  /// @brief ウィンドウの移動
  ///
  /// ウィンドウを移動する。移動先はウィンドウの左上の位置(x, y)で指定する。
  /// @param x 移動先のx座標
  /// @param y 移動先のy座標
  //========================================================
  void move(int x, int y)
  {
    if (hw_)
      ::SetWindowPos(hw_, NULL, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
  }

  //==============================================
  /// @brief メッセージ送出
  ///
  /// ウィンドウにメッセージを投げる。
  /// @param msg メッセージ
  /// @param wp メッセージのパラメタ(WPARAM)
  /// @param lp メッセージのパラメタ(LPARAM)
  //==============================================
  void postMessage(UINT msg, WPARAM wp, LPARAM lp)
  {
    if (hw_)
      ::PostMessage(hw_, msg, wp, lp);
  }


  /////////////////////////////////////////
  //  Window本体情報取得系
  /////////////////////////////////////////
  /// @brief ウィンドウ幅を取得
  int getWidth()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetWindowRect(hw_, &rc);
    return rc.right - rc.left;
  }
  /// @brief ウィンドウ幅の取得
  int width() { return getWidth(); }

  /// @brief ウィンドウの高さを取得
  int getHeight()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetWindowRect(hw_, &rc);
    return rc.bottom - rc.top;
  }
  /// @brief ウィンドウの高さの取得
  int height() { return getHeight(); }

  /// @brief ウィンドウの幅と高さの取得
  /// @param[out] w 幅
  /// @param[out] h 高さ
  /// @return 成功時はtrue、失敗時はfalse
  bool getWidthAndHeight(int& w, int& h)
  {
    if (!hw_)
      return false;
    RECT rc;
    ::GetWindowRect(hw_, &rc);
    w = rc.right - rc.left;
    h = rc.bottom - rc.top;
    return true;
  }

  /* 仕樣に疑問有り 一端削除
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
  */


  ///////////////////////////////////////
  //  コントロールの有效化/無效化
  ///////////////////////////////////////
  /// @brief コントロールの有效化
  /// @param id コントロールID
  void enableCtrl(int id)
  {
    HWND w = GetDlgItem(hw_, id);
    ::EnableWindow(w, TRUE);
  }
  /// @brief コントロールの無效化
  /// @param id コントロールID
  void disableCtrl(int id)
  {
    HWND w = GetDlgItem(hw_, id);
    ::EnableWindow(w, FALSE);
  }

  //////////////////////////////////////////////
  //  コントロール生成系
  //////////////////////////////////////////////
  /// @brief エディットボックス生成
  /// @param id コントロールID
  /// @param re コントロールの位置、幅、高さの指定
  void createEditBox(int id, const eunomia::AnotherRect& re)
  {
    CreateWindow(
      L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }

  /// @brief 複數行エディットボックスの生成
  /// @param id コントロールID
  /// @param re コントロールの位置、幅、高さの指定
  void createMultiLineEditBox(int id, const eunomia::AnotherRect& re)
  {
    CreateWindow(L"EDIT", L"",
      WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_NOHIDESEL |
      ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }

  /// @brief リストボックス生成
  /// @param id コントロールID
  /// @param re コントロールの位置、幅、高さの指定
  void createListBox(int id, const eunomia::AnotherRect& re)
  {
    CreateWindow(
      L"LISTBOX", L"",
      WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_DISABLENOSCROLL | WS_VSCROLL,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }

  /// @brief コンボボックス生成
  /// @param id コントロールID
  /// @param re コントロールの位置、幅、高さの指定
  void createComboBox(int id, const eunomia::AnotherRect& re)
  {
    CreateWindow(
      L"COMBOBOX", L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL
      | CBS_DISABLENOSCROLL | CBS_DROPDOWNLIST,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }

  /// @brief ボタン生成
  /// @param id コントロールID
  /// @param str ボタンに表示する文字列
  /// @param re コントロールの位置、幅、高さの指定
  void createPushButton(
    int id, const std::wstring& str, const eunomia::AnotherRect& re)
  {
    CreateWindow(
      L"BUTTON", str.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }

  /// @brief ラベル生成
  /// @param id コントロールID
  /// @param str ラベルに表示する文字列
  /// @param re コントロールの位置、幅、高さの指定
  void createLabel(
    int id, const std::wstring& str, const eunomia::AnotherRect& re)
  {
    CreateWindow(
      L"STATIC", str.c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }
  
  /// @brief チェックボックス生成
  /// @param id コントロールID
  /// @param str チェックボックスに表示する文字列
  /// @param re コントロールの位置、幅、高さの指定
  void createCheckBox(
    int id, const std::wstring& str, const eunomia::AnotherRect& re)
  {
    CreateWindow(
      L"BUTTON", str.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
      re.x, re.y, re.width, re.height,
      hw_, reinterpret_cast<HMENU>(id), getHI_(), nullptr);
  }


  ///////////////////////////////
  //  EditBox操作系
  ///////////////////////////////
  /// @brief エディットボックスの内容の取得
  /// @param id コントロールID
  /// @return 内容
  std::wstring getTextEB(int id);

  /// @brief エディットボックスの内容の設定
  /// @param id コントロールID
  /// @param txt 設定する内容
  void setEBText(int id, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_SETTEXT, 0, (LPARAM)(txt.c_str()));
  }

  /// @brief エディットボックスの内容の消去
  /// @param id コントロールID
  void clearEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_CLEAR, 0, 0);
  }

  /// @brief クリップボードへの選擇文字列のコピー
  /// @param id コントロールID
  void copyEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_COPY, 0, 0);
  }

  /// @brief 選擇文字列のクリップボードへのコピー及び削除
  /// @param id コントロールID
  void cutEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_CUT, 0, 0);
  }

  /// @brief クリップボードからの文字列のペースト
  /// @param id コントロールID
  void pasteEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, WM_PASTE, 0, 0);
  }

  /// @brief 直前動作の取り消しの可否の確認
  /// @param id コントロールID
  /// @return 取り消し可能であればtrue、さもなくばfalse
  bool canUndoEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    if (::SendMessage(w, EM_CANUNDO, 0, 0))
      return true;
    else
      return false;
  }

  /// @brief 直前動作の取り消し
  /// @param id コントロールID
  void undoEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, EM_UNDO, 0, 0);
  }

  /// @brief 複數行エディットボックスからの一行の取得
  /// @param id コントロールID
  /// @param li 取得する行の番號(0-based)
  /// @return 指定した行の文字列
  std::wstring getLineTextEB(int id, int li);

  /// @brief 複數行エディットボックスの行數の取得
  /// @param id コントロールID
  /// @return 行數
  int countLineEB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    return ::SendMessage(w, EM_GETLINECOUNT, 0, 0);
  }


  ///////////////////////////////////
  //  ListBox操作系
  ///////////////////////////////////
  /// @brief リストボックスの項目の取得
  /// @param id コントロールID
  /// @param no 項目番號(0-based)
  /// @return 項目の文字列
  std::wstring getItemLB(int id, int no);

  /// @brief リストボックスへの項目の追加
  /// @param id コントロールID
  /// @param txt 插入する項目の文字列
  void addItemLB(int id, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_ADDSTRING, 0, (LPARAM)(txt.c_str()));
  }

  /// @brief リストボックスへの項目の插入
  /// @param id コントロールID
  /// @param no 插入位置
  /// @param txt 插入する項目の文字列
  void insertItemLB(int id, int no, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_INSERTSTRING, no, (LPARAM)(txt.c_str()));
  }

  /// @brief リストボックスの項目の削除
  /// @param id コントロールID
  /// @param no 削除位置
  void deleteItemLB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_DELETESTRING, no, 0);
  }

  /// @brief リストボックスの項目數の取得
  /// @param id コントロールID
  int countItemLB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    return ::SendMessage(w, LB_GETCOUNT, 0, 0);
  }

  /// @brief 選擇中のリストボックスの項目番號の取得
  /// @param id コントロールID
  /// @return 選擇中の項目の番號(0-based)、但し失敗時は-1
  int getCurrentLB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    int r = ::SendMessage(w, LB_GETCURSEL, 0, 0);
    if (r == LB_ERR)
      return -1;
    else
      return r;
  }

  /// @brief リストボックスの選擇項目の設定
  /// @param id コントロールID
  /// @param no 選擇する項目の番號(0-based)
  void setCurrentLB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_SETCURSEL, no, 0);
  }

  /// @brief リストボックスの内容の消去
  /// @param id コントロールID
  void clearLB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, LB_RESETCONTENT, 0, 0);
  }

  /// @brief リストボックスへの指定ディレクトリの内容の反映
  /// @param id コントロールID
  /// @param path ディレクトリのパス
  /// @param flag 列擧するファイルの屬性の指定
  ///
  /// flagに指定するのは(Win32APIの定義する)以下の値の組み合はせ。
  ///   - DDL_ARCHIVE
  ///   - DDL_DDL_DIRECTORY
  ///   - DDL_DRIVES
  ///   - DDL_EXCLUSIVE
  ///   - DDL_HIDDEN
  ///   - DDL_READONLY
  ///   - DDL_READWRITE
  ///   - DDL_SYSTEM 
  void dirLB(int id, const std::wstring& path, int flag);


  ////////////////////////////
  //  ComboBox操作系
  ////////////////////////////
  /// @brief コンボボックスの項目の取得
  /// @param id コントロールID
  /// @param no 項目番號(0-based)
  /// @return 項目の文字列
  std::wstring getItemCB(int id, int no);

  /// @brief コンボボックスへの項目の追加
  /// @param id コントロールID
  /// @param txt 插入する項目の文字列
  void addItemCB(int id, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w,CB_ADDSTRING, 0, (LPARAM)(txt.c_str()));
  }

  /// @brief コンボボックスへの項目の插入
  /// @param id コントロールID
  /// @param no 插入位置
  /// @param txt 插入する項目の文字列
  void insertItemCB(int id, int no, const std::wstring& txt)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w,CB_INSERTSTRING,no,(LPARAM)(txt.c_str()));
  }

  /// @brief コンボボックスの項目の削除
  /// @param id コントロールID
  /// @param no 削除位置
  void deleteItemCB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w,CB_DELETESTRING,no,0);
  }

  /// @brief コンボボックスの項目數の取得
  /// @param id コントロールID
  int countItemCB(int id)
  {
    HWND w = ::GetDlgItem(hw_,id);
    return ::SendMessage(w,CB_GETCOUNT,0,0);
  }

  /// @brief コンボボックスの選擇中の項目の番號の取得
  /// @param id コントロールID
  /// @return 選擇中の項目の番號(0-based)、但し失敗時は-1
  int getCurrentCB(int id)
  {
    HWND w = ::GetDlgItem(hw_, id);
    int r = ::SendMessage(w, CB_GETCURSEL, 0, 0);
    if (r == CB_ERR)
      return -1;
    else
      return r;
  }

  /// @brief コンボボックスの選擇項目の設定
  /// @param id コントロールID
  /// @param no 選擇する項目の番號(0-based)
  void setCurrentCB(int id, int no)
  {
    HWND w = ::GetDlgItem(hw_, id);
    ::SendMessage(w, CB_SETCURSEL, no, 0);
  }

  /// @brief コンボボックスの内容の消去
  /// @param id コントロールID
  void clearCB(int id)
  {
    HWND w = ::GetDlgItem(hw_,id);
    ::SendMessage(w, CB_RESETCONTENT, 0, 0);
  }

  /// @brief コンボボックスへの指定ディレクトリの内容の反映
  /// @param id コントロールID
  /// @param path ディレクトリのパス
  /// @param flag 列擧するファイルの屬性の指定
  ///
  /// flagに指定するのは(Win32APIの定義する)以下の値の組み合はせ。
  ///   - DDL_ARCHIVE
  ///   - DDL_DDL_DIRECTORY
  ///   - DDL_DRIVES
  ///   - DDL_EXCLUSIVE
  ///   - DDL_HIDDEN
  ///   - DDL_READONLY
  ///   - DDL_READWRITE
  ///   - DDL_SYSTEM 
  void dirCB(int id, const std::wstring& path, int flag);


  //================================
  //  CheckBox 等の状態取得
  //================================
  /// @brief チェックボックスなどのチェックの有無の取得
  /// @param id コントロールID
  /// @return チェックされてゐればtrue、さもなくばfalse
  bool isChecked(int id)
  {
    return IsDlgButtonChecked(hw_, id);
  }


  ////////////////////////////////////////
  //  横スクロールバー操作系
  ////////////////////////////////////////
  /// @brief 水平スクロールバーの位置の取得
  int getPosHSB()
  {
    return getPosSB(ID_SBH);
  }
  /// @brief 水平スクロールバーの位置の設定
  void setPosHSB(int pos)
  {
    setPosSB(ID_SBH, pos);
  }

  /// @brief 水平スクロールバーの範圍の取得
  /// @param[out] min 最小値
  /// @param[out] max 最大値
  /// @param[out] page ページサイズ
  void getRangeHSB(int& min, int& max, int& page)
  {
    getRangeSB(ID_SBH, min, max, page);
  }

  /// @brief 水平スクロールバーの範圍の設定
  /// @param min 最小値
  /// @param max 最大値
  /// @param page ページサイズ
  void setRangeHSB(int min, int max, int page)
  {
    setRangeSB(ID_SBH, min, max, page);
  }

  /// @brief 水平スクロールバーの有效化
  void enableHSB()
  {
    enableSB(ID_SBH);
  }

  /// @brief 水平スクロールバーの無效化
  void disableHSB()
  {
    disableSB(ID_SBH);
  }


  ///////////////////////////////////////////////
  //  縦スクロールバー操作系
  ///////////////////////////////////////////////
  /// @brief 垂直スクロールバーの位置の取得
  int getPosVSB()
  {
    return getPosSB(ID_SBV);
  }

  /// @brief 垂直スクロールバーの位置の設定
  void setPosVSB(int pos)
  {
    setPosSB(ID_SBV, pos);
  }

  /// @brief 垂直スクロールバーの範圍の取得
  /// @param[out] min 最小値
  /// @param[out] max 最大値
  /// @param[out] page ページサイズ
  void getRangeVSB(int& min, int& max, int& page)
  {
    getRangeSB(ID_SBV, min, max, page);
  }

  /// @brief 垂直スクロールバーの範圍の設定
  /// @param min 最小値
  /// @param max 最大値
  /// @param page ページサイズ
  void setRangeVSB(int min, int max, int page)
  {
    setRangeSB(ID_SBV, min, max, page);
  }

  /// @brief 垂直スクロールバーの有效化
  void enableVSB()
  {
    enableSB(ID_SBV);
  }

  /// @brief 垂直スクロールバーの無效化
  void disableVSB()
  {
    disableSB(ID_SBV);
  }

  /////////////////////////////////////////////////////
  //  共通 (2012.5.13以降追加; 2016.2.27改名)
  /////////////////////////////////////////////////////
  /// @brief スクロールバーの位置の取得
  /// @param id コントロールID
  int getPosSB(int id);

  /// @brief スクロールバーの位置の設定
  /// @param id コントロールID
  /// @param pos 位置
  void setPosSB(int id, int pos);

  /// @brief スクロールバーの範圍の取得
  /// @param[in] id コントロールID
  /// @param[out] min 最小値
  /// @param[out] max 最大値
  /// @param[out] page ページサイズ
  void getRangeSB(int id, int& min, int& max, int& page);

  /// @brief スクロールバーの範圍の設定
  /// @param id コントロールID
  /// @param min 最小値
  /// @param max 最大値
  /// @param page ページサイズ
  void setRangeSB(int id, int min, int max, int page);

  ////////////////////////////////////////////////
  // ID付スクロールバー用 (2016.2.27再實裝)
  ////////////////////////////////////////////////
  /// @brief スクロールバーの有效化
  /// @param id コントロールID
  void enableSB(int id)
  {
    if (!hw_)
      return;

    if (id == ID_SBH)
      ::EnableScrollBar(hw_, SB_HORZ, ESB_ENABLE_BOTH);
    else if (id == ID_SBV)
      ::EnableScrollBar(hw_, SB_VERT, ESB_ENABLE_BOTH);
    else {
      HWND w = ::GetDlgItem(hw_, id);
      ::EnableScrollBar(w, SB_CTL, ESB_ENABLE_BOTH);
    }
  }

  /// @brief スクロールバーの無效化
  /// @param id コントロールID
  void disableSB(int id)
  {
    if (!hw_)
      return;

    if (id == ID_SBH)
      ::EnableScrollBar(hw_, SB_HORZ, ESB_DISABLE_BOTH);
    else if (id == ID_SBV)
      ::EnableScrollBar(hw_, SB_VERT, ESB_DISABLE_BOTH);
    else {
      HWND w = ::GetDlgItem(hw_, id);
      ::EnableScrollBar(w, SB_CTL, ESB_DISABLE_BOTH);
    }
  }

};



#endif // INCLUDE_GUARD_URANIA_WINDOW_BASE_H
