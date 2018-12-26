/**
 * @file win.h
 * @author oZ/acy
 * @brief ウィンドウ
 * @date 2016.2.28  修正
 * @date 2016.10.11 修正
 *   WMsgHandlerクラスにonLButtonDown()、onMButtonDown()、onRButtonDown()を追加
 * @date 2018.12.25 修正
 *   Windows::msgHandler_を生ポインタからunique_ptrに變更
 */
#ifndef INC_URANIA_WINDOW_H___
#define INC_URANIA_WINDOW_H___

#include <map>
#include <memory>
#include "bwin.h"
#include "menu.h"


namespace urania
{

struct WndMessage;
class WMHandler;
class WindowFactory;


} // end of namesace urania


/**
 * @brief メッセージ引き渡し用の構造體
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
class urania::WMHandler : boost::noncopyable
{
public:
  /// @brief コマンドハンドラ
  typedef void (*CmdHandler)(Window*);

private:
  std::map<int, CmdHandler> cmap_;

public:
  virtual ~WMHandler() {}

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
  /// @param w ウィンドウ
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
    Window* w, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// マウス中ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief マウス中ボタン押下時のハンドラ
  /// @param w ウィンドウ
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
    Window*, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// @brief マウス右ボタン押下時のハンドラ
  ///
  /// マウス右ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @param w ウィンドウ
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
    Window*, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
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

  /// @brief コマンドに對するハンドラを登録する
  /// @param cmdid コマンドのID
  /// @param c ハンドラ
  void set(int cmdid, CmdHandler c) { cmap_[cmdid] = c; }

  /// @brief メッセージを受け取つたときに適當なハンドラを呼び出す
  /// @param msg 受け取つたメッセージ
  LRESULT operator()(urania::WndMessage* msg);

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
  urania::RCP_Menu menu_; ///< 関連づけられたメニュー
  bool dad_;  ///< trueならDrag&Dropを受け附ける

protected:
  Window() : msgHandler_(), dad_(false) {}
  void init__(HWND hw);
  void uninit__();

public:
  ~Window() { deleting__(); }

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

  /// @brief ウィンドウ作成
  ///
  /// ファクトリの設定に從つてウィンドウを作成する。
  /// @param mh メッセージハンドラ。
  ///    このハンドラオブジェクトは、作成されたWindowオブジェクトが破棄する。
  /// @param menu メニューのリソースID
  /// @return Windowオブジェクト。
  ///   呼び出し側は、このオブジェクトの破棄の責任を負ふ。
  ///   スマートポインタで管理することを推奬する。
  urania::Window* create(urania::WMHandler* mh, int menu =0)
  {
    return factory__(mh, menu, nullptr, 0);
  }

  /// @brief オーナー附ウィンドウの作成
  ///
  /// ファクトリの設定に從つてオーナー附ウィンドウを作成する。
  /// @param owner オーナーウィンドウ
  /// @param mh メッセージハンドラ。
  ///    このハンドラオブジェクトは、作成されたWindowオブジェクトが破棄する。
  /// @param menu メニューのリソースID
  /// @return Windowオブジェクト。
  ///   呼び出し側は、このオブジェクトの破棄の責任を負ふ。
  ///   スマートポインタで管理することを推奬する。
  urania::Window*
  createAsOwned(
    urania::BasicWindow* owner, urania::WMHandler* mh, int menu =0)
  {
    return factory__(mh, menu, owner, 0);
  }

  /// @brief 子ウィンドウの作成
  ///
  /// ファクトリの設定に從つて子ウィンドウを作成する。
  /// @param parent 親ウィンドウ
  /// @param cid 子ウィンドウID
  /// @param mh メッセージハンドラ。
  ///    このハンドラオブジェクトは、作成されたWindowオブジェクトが破棄する。
  /// @return Windowオブジェクト。
  ///   呼び出し側は、このオブジェクトの破棄の責任を負ふ。
  ///   スマートポインタで管理することを推奬する。
  urania::Window*
  createAsChild(urania::BasicWindow* parent, int cid, urania::WMHandler* mh)
  {
    return factory__(mh, 0, parent, cid);
  }

private:
  /// @brief ウィンドウ作成
  ///
  /// create()、createAsOwned()、createAsChildの下請けとして、
  /// Win32 APIを呼び出してウィンドウを作成する。
  urania::Window* factory__(
    urania::WMHandler* mh, int menu, urania::BasicWindow* par, int cid);
};




#endif // INC_URANIA_WINDOW_H___
