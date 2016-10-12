/**
 * @file win.h
 * @author oZ/acy
 * @brief ウィンドウ
 * @date 2016.2.28  修正
 * @date 2016.10.11 修正
 *   WMsgHandlerクラスにonLButtonDown()、onMButtonDown()、onRButtonDown()を追加
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

  /// ウィンドウが破棄されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief ウィンドウ破棄時のハンドラ
  virtual bool onDestroy() { return false; }

  /// ウィンドウの大きさが變化したときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief ウィンドウサイズ變化時のハンドラ
  /// @param win ウィンドウ
  /// @param typ サイズ變更のタイプ
  /// @param w 幅
  /// @param h 高さ
  virtual bool onSize(Window* win, int typ, int w, int h) { return false; }

  /// キーが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief キー押下時のハンドラ
  /// @param win ウィンドウ
  /// @param code キーのコード
  /// @param rep リピート回數
  /// @param prev メッセージが送られる前、キーが押されてゐたならtrue
  virtual bool onKeyDown(Window* win, int code, int rep, bool prev)
    { return false; }

  /// マウス左ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief マウス左ボタン押下時のハンドラ
  /// @param w ウィンドウ
  /// @param x X座標
  /// @param y Y座標
  /// @param ctrl Ctrlキーが押されてゐるかどうか
  /// @param shft Shiftキーが押されてゐるかどうか
  /// @param lb 左ボタンが押されてゐるかどうか
  /// @param mb 中ボタンが押されてゐるかどうか
  /// @param rb 右ボタンが押されてゐるかどうか
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
  virtual bool onMButtonDown(
    Window*, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// マウス右ボタンが押下されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief マウス右ボタン押下時のハンドラ
  /// @param w ウィンドウ
  /// @param x X座標
  /// @param y Y座標
  /// @param ctrl Ctrlキーが押されてゐるかどうか
  /// @param shft Shiftキーが押されてゐるかどうか
  /// @param lb 左ボタンが押されてゐるかどうか
  /// @param mb 中ボタンが押されてゐるかどうか
  /// @param rb 右ボタンが押されてゐるかどうか
  virtual bool onRButtonDown(
    Window*, int x, int y, bool ctrl, bool shft, bool lb, bool mb, bool rb)
    { return false; }

  /// 再描畫が要求されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief 再描畫要求時のハンドラ
  /// @param win ウィンドウ
  /// @param pd 對象となるPaintDevice
  virtual void onPaint(BasicWindow* win, PaintDevice* pd) {}

  /// スクロールバーが操作されたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief スクロールバー操作時のハンドラ
  /// @param win ウィンドウ
  /// @param id スクロールバーのID
  ///    水平スクロールバーはurania::ID_SBH
  ///    垂直スクロールバーはurania::ID_SBV
  ///    他の値(2以上の整數値)の場合はスクロールバーコントロール
  /// @param pos スクロールバーの位置
  virtual void onScroll(Window* win, int id, int pos) {}

  /// マウスホイールを操作したときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief マウスホイール操作時のハンドラ
  /// @param w ウィンドウ
  /// @param delta 廻轉量
  /// @param keys 押下されてゐるキー
  ///   MK_CONTROL, MK_LBUTTON, MK_MBUTTON, MK_RBUTTON, MK_SHIFT, 
  ///   MK_XBUTTON1, MK_XBUTTON2 の論理和
  /// @param x マウスカーソルのX座標
  /// @param y マウスカーソルのY座標
  virtual void onMouseWheel(Window* w, int delta, int keys, int x, int y) {}

  /// ファイルをドロップしたときに呼び出されるハンドラ。
  /// ライブラリのユーザは適宜オーバーライドして必要な處理を實裝する。
  /// @brief ファイルドロップ時のハンドラ
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
  void onDropFiles_(urania::Window*, WPARAM wp);
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




/**
 *  @brief ウィンドウのファクトリ
 */
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
