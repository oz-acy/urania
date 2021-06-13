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
 * @file dialog.h
 * @brief ダイアログクラス
 * @author oZ/acy
 *
 * @date 2016.2.28  修正
 * @date 2016.10.1  修正 friend classを削除
 * @date 2019.8.30  修正
 *
 * @date 2021.6.13  メッセージハンドラと初期化子の型を公開型に變更
 *
 */
#ifndef INCLUDE_GUARD_URANIA_DIALOG_H
#define INCLUDE_GUARD_URANIA_DIALOG_H

#include "wbase.h"


/**
 * @brief ダイアログ
 */
class urania::Dialog : public urania::WndBase
{
  friend class urania::Window;

public:
  /// @brief メッセージハンドラ
  using MsgHandler = BOOL (*)(urania::Dialog*, UINT, WPARAM, LPARAM);

  /// @brief 初期化子/初期化解除子
  using Initializer = void (*)(urania::Dialog*);

protected:
  Initializer ini_;
  Initializer uini_;
  MsgHandler handler_;
  bool modal_;
  void* app_;

  Dialog(Initializer i, Initializer u, MsgHandler h, bool m, void* a)
  : ini_(i), uini_(i), handler_(h), modal_(m), app_(a) {}

  void init_(HWND hw) override;
  void uninit_() override;
  void destroyWindow_() override;

public:
  ~Dialog() { deleting_(); }

  /// @brief Modalなダイアログの作成
  /// @param rid リソースID
  /// @param ini 初期化子。nullptrも可。
  /// @param ui 初期化解除子。nullptrも可。
  /// @param hnd メッセージハンドラ
  /// @param app
  ///   任意のポインタ(void\*)。設定しておくと getAppData() で參照できる。
  /// @return ダイアログの返した整數値
  static
  int
  doModal(
    int rid, Initializer ini, Initializer ui, MsgHandler hnd,
    void* app = nullptr);

  /// @brief Modelessなダイアログの作成
  /// @param rid リソースID
  /// @param ini 初期化子。nullptrも可。
  /// @param ui 初期化解除子。nullptrも可。
  /// @param hnd メッセージハンドラ
  /// @param app
  ///   任意のポインタ(void\*)。設定しておくと getAppData() で參照できる。
  /// @return 作成されたダイアログを保持するunique_ptr
  static
  std::unique_ptr<Dialog>
  doModeless(
    int rid, Initializer ini, Initializer ui, MsgHandler hnd,
    void* app = nullptr);

  /// @brief 所有者附のModalなダイアログの作成
  /// @param rid リソースID
  /// @param par 所有者
  /// @param ini 初期化子。nullptrも可。
  /// @param ui 初期化解除子。nullptrも可。
  /// @param hnd メッセージハンドラ
  /// @param app
  ///   任意のポインタ(void\*)。設定しておくと getAppData() で參照できる。
  /// @return ダイアログの返した整數値
  static
  int
  doOwnedModal(
    int rid, WndBase* par, Initializer ini, Initializer ui, MsgHandler hnd,
    void* app = nullptr);

  /// 所有者附のModelessなダイアログの作成
  /// @param rid リソースID
  /// @param par 所有者
  /// @param ini 初期化子。nullptrも可。
  /// @param ui 初期化解除子。nullptrも可。
  /// @param hnd メッセージハンドラ
  /// @param app
  ///   任意のポインタ(void\*)。設定しておくと getAppData() で參照できる。
  /// @return 作成されたダイアログを保持するunique_ptr
  static
  std::unique_ptr<Dialog>
  doOwnedModeless(
    int rid, WndBase* par, Initializer ini, Initializer ui, MsgHandler hnd,
    void* app = nullptr);


  /// @brief Modalなダイアログの終了
  /// @param i doModal() あるいは doOwnedModal() に返す整數値
  void endModal(int i);

  /// @brief 作成時設定のポインタの取得
  /// @return 作成時に設定された任意のポインタ(void*)
  void* getAppData() { return app_; }

protected:
  /// @brief 各Dialogのメッセージ處理「プロシージャ」
  virtual BOOL dproc_(UINT msg, WPARAM wp, LPARAM lp);

  /// @brief Dialogと結合しているDialogBox用のDlgProc
  static BOOL CALLBACK dlgproc_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
};




#endif // INCLUDE_GUARD_URANIA_DIALOG_H
