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
 * @file window.h
 * @author oZ/acy (名賀月晃嗣)
 * @brief ウィンドウクラス
 *
 * @date 2016.2.28  修正
 * @date 2016.10.11
 *  WMsgHandlerクラスにonLButtonDown()、onMButtonDown()、onRButtonDown()を追加。
 * @date 2018.12.25
 *  Windows::msgHandler_を生ポインタからunique_ptrに變更。
 * @date 2019.8.30
 *  豫約されてゐる識別子に該當してゐたものを修正。
 * @date 2021.3.27  修正
 */
#ifndef INCLUDE_GUARD_URANIA_WINDOW_H
#define INCLUDE_GUARD_URANIA_WINDOW_H

#include <map>
#include <memory>
#include <utility>
#include "bwin.h"
#include "menu.h"


namespace urania
{
struct WndMessage;
class WMHandler;
class WindowFactory;

} // end of namesace urania


/**
 * メッセージ引き渡し用の構造體
 */
struct urania::WndMessage
{
  urania::Window* window;
  UINT id;
  WPARAM wparam;
  LPARAM lparam;
};


/**
 * @brief メッセージハンドラ基底
 */
class urania::WMHandler : themis::Noncopyable<urania::WMHandler>
{
public:
  /// @brief コマンドハンドラ
  ///
  /// ウィンドウがWM_COMMANDを受け取つたときに呼び出される
  /// コマンドハンドラの型。
  /// コマンドのID毎に必要な處理をハンドラとして實裝し、
  /// WMHandler::regist()で登錄する。
  using CmdHandler = void (*)(Window*);

private:
  std::map<int, CmdHandler> cmap_; ///< コマンドのIDとハンドラのマップ

public:
  /// 解體子
  virtual ~WMHandler() {}

  /// @brief コマンドに對するハンドラを登録する
  /// @param cmdid コマンドのID
  /// @param c ハンドラ
  void regist(int cmdid, CmdHandler c) { cmap_[cmdid] = c; }

  /// @brief メッセージに應じたハンドラを呼び出す
  /// @param msg メッセージ
  LRESULT operator()(urania::WndMessage* msg);

  /// @brief ウィンドウ破棄時のハンドラ
  ///
  /// ウィンドウが破棄されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @return デフォルトではfalseを返す。
  ///  オーバーライドする場合、有效な處理の後にはtrueを返すこと。
  virtual bool onDestroy() { return false; }

  /// @brief ウィンドウサイズ變化時のハンドラ
  ///
  /// ウィンドウの大きさが變化したときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param win ウィンドウ
  /// @param typ サイズ變更のタイプ
  /// @param w 幅
  /// @param h 高さ
  /// @return デフォルトではfalseを返す。
  ///  オーバーライドする場合、有效な處理の後にはtrueを返すこと。
  virtual bool onSize(Window* win, int typ, int w, int h) { return false; }

  /// @brief キー押下時のハンドラ
  ///
  /// キーが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param win ウィンドウ
  /// @param code キーのコード
  /// @param rep リピート回數
  /// @param prev メッセージが送られる前、キーが押されてゐたならtrue
  /// @return デフォルトではfalseを返す。
  ///  オーバーライドする場合、有效な處理の後にはtrueを返すこと。
  virtual bool onKeyDown(Window* win, int code, int rep, bool prev)
    { return false; }

  /// @brief マウス左ボタン押下時のハンドラ
  ///
  /// マウス左ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param win ウィンドウ
  /// @param x X座標
  /// @param y Y座標
  /// @param ctrl Ctrlキーが押されてゐるかどうか
  /// @param shft Shiftキーが押されてゐるかどうか
  /// @param lb 左ボタンが押されてゐるかどうか
  /// @param mb 中ボタンが押されてゐるかどうか
  /// @param rb 右ボタンが押されてゐるかどうか
  /// @return デフォルトではfalseを返す。
  ///  オーバーライドする場合、有效な處理の後にはtrueを返すこと。
  virtual bool onLButtonDown(
    Window* win, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// マウス中ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief マウス中ボタン押下時のハンドラ
  /// @param win ウィンドウ
  /// @param x X座標
  /// @param y Y座標
  /// @param ctrl Ctrlキーが押されてゐるかどうか
  /// @param shft Shiftキーが押されてゐるかどうか
  /// @param lb 左ボタンが押されてゐるかどうか
  /// @param mb 中ボタンが押されてゐるかどうか
  /// @param rb 右ボタンが押されてゐるかどうか
  /// @return デフォルトではfalseを返す。
  ///  オーバーライドする場合、有效な處理の後にはtrueを返すこと。
  virtual bool onMButtonDown(
    Window* win, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// @brief マウス右ボタン押下時のハンドラ
  ///
  /// マウス右ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param win ウィンドウ
  /// @param x X座標
  /// @param y Y座標
  /// @param ctrl Ctrlキーが押されてゐるかどうか
  /// @param shft Shiftキーが押されてゐるかどうか
  /// @param lb 左ボタンが押されてゐるかどうか
  /// @param mb 中ボタンが押されてゐるかどうか
  /// @param rb 右ボタンが押されてゐるかどうか
  /// @return デフォルトではfalseを返す。
  ///  オーバーライドする場合、有效な處理の後にはtrueを返すこと。
  virtual bool onRButtonDown(
    Window* win, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// @brief 再描畫要求時のハンドラ
  ///
  /// 再描畫が要求されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param win ウィンドウ
  /// @param pd 再描畫用のPaintDevice。
  ///   呼び出し元が生成と破棄の責任を負ふので、
  ///   onPaint()の中で破棄してはならない。
  virtual void onPaint(BasicWindow* win, PaintDevice* pd) {}

  /// @brief スクロールバー操作時のハンドラ
  ///
  /// スクロールバーが操作されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param win ウィンドウ
  /// @param id スクロールバーのID
  ///    水平スクロールバーはurania::ID_SBH
  ///    垂直スクロールバーはurania::ID_SBV
  ///    他の値(2以上の整數値)の場合はスクロールバーコントロール
  /// @param pos スクロールバーの位置
  virtual void onScroll(Window* win, int id, int pos) {}

  /// @brief マウスホイール操作時のハンドラ
  ///
  /// マウスホイールを操作したときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param w ウィンドウ
  /// @param delta 廻轉量
  /// @param keys 押下されてゐるキー
  ///   MK_CONTROL, MK_LBUTTON, MK_MBUTTON, MK_RBUTTON, MK_SHIFT, 
  ///   MK_XBUTTON1, MK_XBUTTON2 の論理和
  /// @param x マウスカーソルのX座標
  /// @param y マウスカーソルのY座標
  virtual void onMouseWheel(Window* w, int delta, int keys, int x, int y) {}

  /// @brief ファイルドロップ時のハンドラ
  ///
  /// ファイルをドロップしたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param w ウィンドウ
  /// @param fa ファイル名のベクタ
  /// @param x ドロップ位置のX座標
  /// @param y ドロップ位置のY座標
  virtual void onDropFiles(
    Window* w, std::vector<std::wstring>& fa, int x, int y)
    {}

private:
  /// @brief ファイルドロップ時の處理
  ///
  /// ファイルをドロップしたときに定型處理をしてonDropFiles()を呼び出す。
  void onDropFiles_(urania::Window*, WPARAM wp);

  /// @brief スクロールバー操作時の處理
  ///
  /// スクロールバーを操作したときに定型處理をしてonScroll()を呼び出す。
  void onScroll_(urania::Window*, UINT msg, WPARAM, LPARAM);
};




/**
 * @brief ウィンドウ
 */
class urania::Window : public urania::BasicWindow
{
  friend class urania::WindowFactory;

public:
  typedef LRESULT (*H_)(urania::Window* pw, UINT msg, WPARAM wp, LPARAM lp);

protected:
  std::unique_ptr<urania::WMHandler> msgHandler_; ///< メッセージハンドラ
  std::shared_ptr<urania::Menu> menu_; ///< 関連づけられたメニュー
  bool dad_;  ///< trueならDrag&Dropを受け附ける

protected:
  Window() : msgHandler_(), dad_(false) {}
  void init_(HWND hw) override;
  void uninit_() override;

public:
  ~Window() { deleting_(); }

  std::shared_ptr<urania::Menu> getMenu() const { return menu_; }
  void setMenu(const std::shared_ptr<urania::Menu>& m) { linkMenu_(m); }
  void setMenu(std::shared_ptr<urania::Menu>&& m) { linkMenu_(m); }

protected:
  /// 各Windowのメッセージ処理プロシージャ
  virtual LRESULT wproc_(UINT msg, WPARAM wp, LPARAM lp) override;

  static HMENU getHM_(const std::shared_ptr<urania::Menu>& mn)
    { return mn->hmenu_; }

  HMENU linkMenu_(const std::shared_ptr<urania::Menu>& mn)
  {
    menu_ = mn;
    return mn->giveHM_();
  }

  HMENU linkMenu_(std::shared_ptr<urania::Menu>&& mn)
  {
    menu_ = mn;
    return mn->giveHM_();
  }
};




/**
 *  @brief ウィンドウのファクトリ
 *
 * bkcolorに設定する番號は次のいづれか。
 *   - BG_WHITE 白
 *   - BG_BLACK 黑
 *   - BG_GRAY 灰色
 *   - BG_LTGRAY 淡灰色
 *   - BG_DKGRAY 濃灰色
 *   - BG_APPWORKSPACE システム設定のMDIクライアントウィンドウ背景色
 *   - BG_BKWINDOW システム設定のウィンドウ背景色
 */
class urania::WindowFactory
{
public:
  std::wstring title; ///< ウィンドウのタイトル
  int x; ///< ウィンドウの左上のx座標
  int y; ///< ウィンドウの左上のy座標
  int w; ///< ウィンドウの幅
  int h; ///< ウィンドウの高さ

  unsigned icon;  ///< アイコンのリソースID
  unsigned cursor;  ///< カーソルのリソースID
  unsigned bkcolor;  ///< 背景色の番號

  bool drag_and_drop;  ///< ドラッグ・アンド・ドロップを受け附けるときはtrue
  bool maxbox;  ///< 最大化ボタンを持たせる場合はtrue
  bool minbox;  ///< 最小化ボタンを持たせる場合はtrue
  bool h_scrollbar;  ///< 水平スクロールバーを持たせる場合はtrue
  bool v_scrollbar;  ///< 垂直スクロールバーを持たせる場合はtrue
  bool popup;  ///< ポップアップウィンドウを作成する場合はtrue
  bool border_only;  ///< 枠のみのウィンドウを作成する場合はtrue
  bool resizeable;  ///< ウィンドウの大きさを變更可能にする場合はtrue

public:
  WindowFactory()
    : x(CW_USEDEFAULT), w(CW_USEDEFAULT),
      icon(DEFAULT_RC), cursor(DEFAULT_RC), bkcolor(BG_WHITE),
      drag_and_drop(false), maxbox(false), minbox(false),
      h_scrollbar(false), v_scrollbar(false), popup(false),
      border_only(false), resizeable(false)
  {}

  /// @brief ウィンドウ生成
  ///
  /// ファクトリの設定に從つてウィンドウを生成する。
  /// @param hnd メッセージハンドラ。
  ///   このハンドラオブジェクトは、生成されたWindowオブジェクトが破棄する。
  /// @param menu メニューのリソースID
  /// @return 生成したWindowオブジェクトを保持するunique_ptr。
  std::unique_ptr<urania::Window>
  create(std::unique_ptr<urania::WMHandler>&& hnd, int menu =0)
  {
    return factory_(std::move(hnd), menu, nullptr, 0);
  }

  /// @brief オーナー附ウィンドウの生成
  ///
  /// ファクトリの設定に從つてオーナー附ウィンドウを生成する。
  /// @param owner オーナーウィンドウ
  /// @param hnd メッセージハンドラ。
  ///   このハンドラオブジェクトは、生成されたWindowオブジェクトが破棄する。
  /// @param menu メニューのリソースID
  /// @return 生成したWindowオブジェクトを保持するunique_ptr。
  std::unique_ptr<urania::Window>
  createAsOwned(
    urania::BasicWindow* owner, 
    std::unique_ptr<urania::WMHandler>&& hnd, 
    int menu =0)
  {
    return factory_(std::move(hnd), menu, owner, 0);
  }

  /// @brief 子ウィンドウの生成
  ///
  /// ファクトリの設定に從つて子ウィンドウを生成する。
  /// @param parent 親ウィンドウ
  /// @param cid 子ウィンドウID
  /// @param hnd メッセージハンドラ。
  ///   このハンドラオブジェクトは、生成されたWindowオブジェクトが破棄する。
  /// @return 生成したWindowオブジェクトを保持するunique_ptr。
  std::unique_ptr<urania::Window>
  createAsChild(
    urania::BasicWindow* parent, int cid, std::unique_ptr<WMHandler>&& hnd)
  {
    return factory_(std::move(hnd), 0, parent, cid);
  }

private:
  /// @brief ウィンドウ生成
  ///
  /// create()、createAsOwned()、createAsChild()の下請けとして、
  /// Win32 APIを呼び出してウィンドウを生成する。
  std::unique_ptr<urania::Window>
  factory_(
    std::unique_ptr<WMHandler>&& hnd, int menu,
    urania::BasicWindow* par, int cid);
};




#endif // INCLUDE_GUARD_URANIA_WINDOW_H
