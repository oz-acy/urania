/**
 * @file dialog.h
 * @brief ダイアログクラス
 * @author oZ/acy
 *
 * @date 2016.2.28  修正
 * @date 2016.10.1  修正: friend class を削除
 * @date 2019.8.30  修正
 */
#ifndef INCLUDE_GUARD_URANIA_DIALOG_H
#define INCLUDE_GUARD_URANIA_DIALOG_H

#include "wbase.h"


/**
 * @brief ダイアログ
 */
class urania::Dialog : public urania::WndBase
{
//  friend class urania::System;
  friend class urania::Window;

private:
  /// メッセージハンドラの型
  using H_ = BOOL (*)(urania::Dialog*, UINT, WPARAM, LPARAM);
  //typedef BOOL (*H_)(urania::Dialog*, UINT, WPARAM, LPARAM);
  /// 初期化函數の型
  using Ini_ = void (*)(urania::Dialog*);
  //typedef void (*Ini_)(urania::Dialog*);

 protected:
  Ini_ ini_;
  Ini_ uini_;
  H_ handler_;
  bool modal_;
  void* app_;

  Dialog(Ini_ i, Ini_ u, H_ h, bool m, void* a)
  : ini_(i), uini_(i), handler_(h), modal_(m), app_(a) {}

  void init_(HWND hw) override;
  void uninit_() override;
  void destroyWindow_() override;


 public:
  ~Dialog() { deleting_(); }

  /// Modalなダイアログを作成する。
  /// @param rid リソースID
  /// @param ini イニシャライザ。nullptrも可。
  ///            プロトタイプは void (*ini)(urania::Dialog*);
  /// @param ui アンイニシャライザ。nullptrも可。
  ///           プロトタイプは void (*ui)(urania::Dialog*);
  /// @param hnd メッセージハンドラ。
  ///      プロトタイプは BOOL (*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
  /// @param app 任意のポインタ(void*)。
  ///      設定しておくと getAppData() で參照できる。
  /// @return ダイアログの返した整數値
  static int doModal(int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app =nullptr);

  /// Modelessなダイアログを作成する。
  /// @param rid リソースID
  /// @param ini イニシャライザ。nullptrも可。
  ///          プロトタイプは void (*ini)(urania::Dialog*);
  /// @param ui アンイニシャライザ。nullptrも可。
  ///           プロトタイプは void (*ui)(urania::Dialog*);
  /// @param hnd メッセージハンドラ。
  ///      プロトタイプは BOOL (*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
  /// @param app 任意のポインタ(void*)。
  ///        設定しておくと getAppData() で參照できる。
  /// @return 作成されたダイアログへのポインタ
  static urania::Dialog* doModeless(
    int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app =nullptr);

  /// 所有者附のModalなダイアログを作成する。
  /// @param rid リソースID
  /// @param par 所有者
  /// @param ini イニシャライザ。nullptrも可。
  ///            プロトタイプは void (*ini)(urania::Dialog*);
  /// @param ui アンイニシャライザ。nullptrも可。
  ///             プロトタイプは void (*ui)(urania::Dialog*);
  /// @param hnd メッセージハンドラ。
  ///      プロトタイプは BOOL (*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
  /// @param app 任意のポインタ(void*)。
  ///     設定しておくと getAppData() で參照できる。
  /// @return ダイアログの返した整數値
  static int doOwnedModal(
    int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd,
    void* app =nullptr);

  /// 所有者附のModelessなダイアログを作成する。
  /// @param rid リソースID
  /// @param par 所有者
  /// @param ini イニシャライザ。nullptrも可。
  ///            プロトタイプは void (*ini)(urania::Dialog*);
  /// @param ui アンイニシャライザ。nullptrも可。
  ///           プロトタイプは void (*ui)(urania::Dialog*);
  /// @param hnd メッセージハンドラ。
  ///       プロトタイプは BOOL (*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
  /// @param app 任意のポインタ(void*)。
  ///    設定しておくと getAppData() で參照できる。
  /// @return 作成されたダイアログへのポインタ
  static urania::Dialog* doOwnedModeless(
    int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd,
    void* app =nullptr);


  /// Modalなダイアログを終了する。
  /// @param i doModal() あるいは doOwnedModal() に返す整數値
  void endModal(int i);


  /// 作成時に設定されたポインタを返す。
  /// @return 作成時に設定されたポインタ(void*)。
  void* getAppData() { return app_; }

protected:
  /// 各Dialogのメッセージ処理プロシージャ
  virtual BOOL dproc_(UINT msg, WPARAM wp, LPARAM lp);

  /// Dialogと結合しているDialogBox用のDlgProc
  static BOOL CALLBACK dlgproc_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
};



#endif // INCLUDE_GUARD_URANIA_DIALOG_H
