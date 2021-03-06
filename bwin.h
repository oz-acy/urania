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
 * @file bwin.h
 * @author oZ/acy (名賀月晃嗣)
 * @brief Window用基底クラス
 *
 * @date 2016.2.29   修正
 * @date 2018.12.26  修正
 * @date 2019.8.30   修正
 * @date 2021.4.4    修正
 *
 * @date 2021.6.11
 *   依據ライブラリをthemis+polymniaからeunomiaに切り替へるための修正
 *
 */
#ifndef INCLUDE_GUARD_URANIA_BASICWINDOW_H
#define INCLUDE_GUARD_URANIA_BASICWINDOW_H

#include <memory>
#include <vector>
#include "wbase.h"
#include "paintdev.h"


/*----------------------------------------------*//**
 * @brief Window管理・操作用基底クラス
 */
class urania::BasicWindow : public urania::WndBase
{
public:
  //-------------------
  //  struct D0_
  //  記述構造体
  //-------------------
  struct D0_
  {
    std::wstring title;  // タイトル
    HWND pwnd;  // 親ウィンドウ
    HMENU hm;  // メニュー
    WNDPROC winproc;  // ウィンドウプロシージャ

    int x, y, w, h;  // ウィンドウの位置、幅、高さ

    unsigned icon;  // アイコンのリソースID
    unsigned cursor;  //* カーソルのリソースID
    unsigned bkcolor;  // 背景色ID

    bool maxbox;  // 最大化ボックスの有無
    bool minbox;  // 最小化ボックスの有無
    bool h_scrollbar;  // 橫スクロールバーの有無
    bool v_scrollbar;  // 縱スクロールバーの有無
    bool popup;  // ポップアップウィンドウか否か
    bool border_only;  // 境界のみか否か
    bool can_resize;  // 大きさ變更の可否

    D0_() noexcept
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
  //  WNDCLASS生成情報格納クラス
  //--------------------------------//
  struct WC_
  {
    unsigned icon_id_;
    unsigned cursor_id_;
    unsigned bkcolor_;
    WNDPROC proc_;
    std::wstring wcname_;

    //========================================================================
    //  WNDCLASS登録の要不要を決めるための operator==() と operator!=()
    //========================================================================
    bool operator==(const WC_& other) const noexcept
    {
      if (icon_id_ != other.icon_id_)
        return false;
      if (cursor_id_ != other.cursor_id_)
        return false;
      if (bkcolor_ != other.bkcolor_)
        return false;
      if (proc_ != other.proc_)
        return false;

      return true;
    }

    bool operator!=(const WC_& other) const noexcept
    {
      return !(*this == other);
    }
  };


private:
  static std::vector<WC_> vwc_S;  // WNDCLASS情報ベクタ

protected:
  BasicWindow() = default;

  void createWindow0_(const D0_& de);

  /// @brief  BasicWindowオブジェクトとHWNDの二重結合を形成
  ///
  /// BasicWindowオブジェクトからHWNDへの結合と、
  /// HWNDからオブジェクトへの結合を形成する。
  /// @param hw オブジェクトと結合するHWND
  void bindHWND_(HWND hw)
  {
    link_(hw);
    SetWindowLongPtr(hw, 0, reinterpret_cast<ULONG_PTR>(this));
  }

  /// @brief HWNDからBasicWindowオブジェクトへの結合を切斷
  void unbindHWND_()
  {
    if (hw_)
      SetWindowLongPtr(hw_, 0, reinterpret_cast<ULONG_PTR>(nullptr));
  }

  /// @brief ウィンドウ破棄の實處理を實裝
  void destroyWindow_() override
  {
    kill_();
  }

  /// @brief ウィンドウプロシージャ(個別)
  ///
  /// オブジェクトに結び附いたウィンドウへのメッセージを
  /// 處理するプロシージャ。
  /// 派生クラスで實裝する。
  /// @param[in] msg メッセージ
  /// @param[in] wp メッセージのパラメータ(WPARAM)
  /// @param[in] lp メッセージのパラメータ(LPARAM)
  virtual LRESULT wproc_(UINT msg, WPARAM wp, LPARAM lp) =0;

  /// @brief ウィンドウプロシージャ
  ///
  /// BasicWindowオブジェクトと結合してゐるHWND用のウィンドウプロシージャ。
  /// HWNDと結び附いたオブジェクトのwproc_()を呼び出す。
  /// @param[in] hw メッセージを受けたウィンドウのHWND
  /// @param[in] msg メッセージ
  /// @param[in] wp メッセージのパラメータ(WPARAM)
  /// @param[in] lp メッセージのパラメータ(LPARAM)
  static LRESULT CALLBACK winproc_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

  static std::wstring registerWC_(const WC_& wc);


public:
  ~BasicWindow() = default;

  /// @brief ウィンドウ更新(再描畫要求)
  ///
  /// ウィンドウの再描畫を要求する。
  /// ウィンドウの全部あるいは一部が再描畫せらるべき場合には、
  /// 再描畫處理が呼び出される。
  void update()
  {
    if (hw_)
      ::UpdateWindow(hw_);
  }

  /// @brief ウィンドウの再描畫領域の設定
  ///
  /// ウィンドウのクライアント領域全體を、
  /// 再描畫が必要な領域に設定する。
  void invalidate()
  {
    if (hw_)
      ::InvalidateRect(hw_, nullptr, FALSE);
  }

  /// @brief ウィンドウの再描畫領域の設定
  ///
  /// ウィンドウのクライアント領域の指定した範圍を
  /// 再描畫が必要な領域に設定する。
  /// @param[in] rect 再描畫領域に設定する長方形
  void invalidate(const eunomia::Rect& rect)
  {
    if (hw_) {
      ::RECT r;
      r.left = rect.left;
      r.right = rect.right;
      r.top = rect.top;
      r.bottom = rect.bottom;

      ::InvalidateRect(hw_, &r, FALSE);
    }
  }

  /// @brief ウィンドウ描畫用のPaintDeviceを取得
  ///
  /// ウィンドウを(再)描畫するためのPaintDeviceを取得する。
  std::unique_ptr<PaintDevice> getPaintDevice();

  /// @brief ウィンドウ再描畫
  ///
  /// ウィンドウ再描畫時に呼び出される。
  /// getPaintDevice()で取得したPaintDeviceを實引數として
  /// 描畫處理procを呼び出した後、
  /// PaintDeviceを破棄する。
  /// @param proc
  ///   描畫處理を行ふ函數オブジェクト。
  ///   プロトタイプは void (proc)(BasicWindow*, PaintDevice*);
  /// @param wp 再描畫要求メッセージのパラメタ(不使用)
  /// @param lp 再描畫要求メッセージのパラメタ(不使用)
  template<class PT_> LRESULT onPaint(PT_&& proc, WPARAM wp, LPARAM lp);

  /// @brief クライアント領域の幅を取得
  int getClientWidth()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetClientRect(hw_, &rc);
    return rc.right;
  }

  /// @brief クライアント領域の高さを取得
  int getClientHeight()
  {
    if (!hw_)
      return 0;
    RECT rc;
    ::GetClientRect(hw_, &rc);
    return rc.bottom;
  }


  /// @brief クライアント領域の幅と高さの取得
  /// @param[out] w 幅
  /// @param[out] h 高さ
  /// @return 成功時はtrue、さもなくばfalse
  bool getClientWidthAndHeight(int& w, int& h)
  {
    if (!hw_)
      return false;
    RECT rc;
    ::GetClientRect(hw_, &rc);
    w = rc.right;
    h = rc.bottom;
    return true;
  }

  /* 仕樣に疑義あり 一端削除
   * 削除を取り消す場合でも、getClientRectに改名するのが適當か?
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
  */

  /// @brief ウィンドウの大きさを變更
  /// @param w 幅
  /// @param h 高さ
  void resize(int w, int h)
  {
    if (hw_)
      ::SetWindowPos(hw_, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
  }

  /// @brief ウィンドウの大きさを變更
  ///
  /// クライアント領域の幅と高さを指定して
  /// ウィンドウの大きさを變更する。
  /// @param w 幅
  /// @param h 高さ
  void resizeClientArea(int w, int h)
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

  /// @brief タイマーを設定
  /// @param id タイマーのID
  /// @param elapse タイムアウト値(ミリ秒)
  void setTimer(int id, int elapse) { ::SetTimer(hw_, id, elapse, nullptr); }

  /// @brief タイマーを破棄
  /// @param id タイマーのID
  void killTimer(int id) { ::KillTimer(hw_, id); }

  /// @brief デフォルトのメッセージ處理
  virtual LRESULT defHandler(UINT msg, WPARAM wp, LPARAM lp);
};




/*=================================================================
 *  BasicWindow::onPaint()
 *  WM_PAINTに対応するためのヘルパ
 *  PaintDeviceを生成してprocに渡す
 *  ユーザ定義のハンドラから必要に応じて明示的に呼び出すこと
 *===============================================================*/
template<class PT_>
inline
LRESULT urania::BasicWindow::onPaint(PT_&& proc, WPARAM wp, LPARAM lp)
{
  (void)wp;
  (void)lp;

  PAINTSTRUCT ps;
  ::BeginPaint(hw_, &ps);
  RECT rc;
  ::GetClientRect(hw_, &rc);

  auto pd = PaintDevice::create(ps.hdc, nullptr, nullptr, rc.right, rc.bottom);

  proc(this, pd.get());

  pd.reset(); // EndPaintを呼ぶ前にPaintDeviceを破棄する
  ::EndPaint(hw_, &ps);

  return 0;
}




#endif // INCLUDE_GUARD_URANIA_BASICWINDOW_H
