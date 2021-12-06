/*
 * Copyright 2001-2021 oZ/acy (名賀月晃嗣)
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
 * @file system.h
 * @author oZ/acy
 * @brief システム管理
 *
 * @date 14 Feb 2004  System::getLongPathName() を追加
 * @date 27 Feb 2008  System::getDesktopWidth() を追加
 *                    System::getDesktopHeight() を追加
 * @date 4 Apr 2009  UNICODE對應
 * @date 7 Nov 2009  System::cnvWStr2MBStr() を追加
 * @date 2 Sep 2010  task系削除・start系追加
 * @date 13 May 2012  C++11對應
 * @date 27 Feb 2016  ファイル名變更、メソッド名變更
 * @date 1 Oct 2016   不要なfriend classを削除
 * @date 30 Aug 2019  インクルードガードを修正
 * @date 24 Mar 2021  System::messageLoop()の仕樣を修正
 * @date 13 Jun 2021
 *   System::getCmdLineArgs() を削除
 *   System::getCmdLineArgsW(const wchar_t*) を追加 
 * @date 6 Dec 2021
 *   MSVSでビルドするためにNOMINMAXを#define
 *
 */
#ifndef INCLUDE_GUARD_URANIA_SYSTEM_H
#define INCLUDE_GUARD_URANIA_SYSTEM_H

// windows.hのmin/maxマクロの抑止
#ifndef NOMINMAX
#define NOMINMAX
#endif 

#include <windows.h>
#include <string>
#include <vector>
#include "decl.h"


/**
 * システム管理クラス
 */
class urania::System
{
  friend class urania::WndBase;
  friend class urania::Menu;

private:
  static HINSTANCE hi_S;

private:
  System() =delete; //インスタンス生成禁止

public:
  /// 利用する際に、最初に一度だけ呼ぶ。
  /// @param[in] hi HINSTANCE値。WinMain()の1つ目の引數を渡す。
  static void start(HINSTANCE hi)
  {
    hi_S = hi;
  }

  /// @brief 終了指令
  ///
  /// メッセージキューに終了メッセージを投げる。
  /// メッセージループは、終了メッセージを受け取ると直ちに繰り返しを終了し、
  /// 指定された終了コードを返す。
  ///
  /// @param r 終了コード
  static void quit(int r)
  {
    ::PostQuitMessage(r);
  }

  /// @brief メッセージループ
  ///
  /// メッセージをキューから取り出し、プロシージャに投げる處理を繰り返す。
  /// 取り出したメッセージが終了メッセージのときは、繰り返しを終了し、
  /// 終了コードを返す。
  ///
  /// @return 終了コード
  static int messageLoop();

  /// @brief コールバック附メッセージループ
  ///
  /// メッセージをキューから取り出し、プロシージャに投げる處理を繰り返す。
  /// 但し、アイドル狀態(メッセージが來ない期間)にはコールバックを呼び出す。
  /// 取り出したメッセージが終了メッセージのときは、繰り返しを終了し、
  /// 終了コードを返す。
  ///
  /// @param f
  ///   アイドル状態の時に呼び出されるコールバック函數。
  ///   引數を取らず、bool値を返す。
  ///   falseを返した場合、次にメッセージを處理するまでコールバックしない。
  ///
  /// @return 終了コード
  template<class Func_> static int messageLoop(Func_ f);


  // メッセージボックス系

  /// @brief 警告メッセージボックス
  ///
  /// 警告用のメッセージボックスを表示する。
  /// 
  /// @param[in] title タイトル
  /// @param[in] msg メッセージ
  static void alert(const std::wstring& title, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONEXCLAMATION);
  }

  /// @brief 通知用メッセージボックス
  ///
  /// 通知用のメッセージボックスを表示する。
  ///
  /// @param[in] title タイトル
  /// @param[in] msg メッセージ
  static void notify(const std::wstring& title, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), title.c_str(), MB_OK);
  }

  /// @brief 三擇メッセージボックス
  ///
  /// はい、いいえ、キャンセルの三擇のメッセージボックスを表示する。
  ///
  /// @param[in] title タイトル
  /// @param[in] msg メッセージ
  /// @return 1: OK, 2: NO, 3: CANCEL のいづれか
  static int askYesNoCancel(const std::wstring& title, const std::wstring& msg)
  {
    int res = ::MessageBox(NULL, msg.c_str(), title.c_str(), MB_YESNOCANCEL);
    switch(res)
    {
    case IDYES:
      return 1;
    case IDNO:
      return 0;
    case IDCANCEL:
      return 2;
    default:
      return 2;  // キャンセル
    }
  }

  /// @brief 二擇メッセージボックス
  ///
  /// はい、いいえの二擇のメッセージボックスを表示する。
  ///
  /// @param[in] title タイトル
  /// @param[in] msg メッセージ
  /// @return true: Yes, false: No のいづれか
  static bool askYesNo(const std::wstring& title, const std::wstring& msg)
  {
    int res = ::MessageBox(NULL, msg.c_str(), title.c_str(), MB_YESNO);
    if (res == IDYES)
      return true;
    else
      return false;
  }


  // カーソル系
  /// @brief カーソル表示化
  ///
  /// カーソルを表示狀態にする。
  static void showCursor()
    { 
      ::ShowCursor(TRUE);
    }
  /// @brief カーソル非表示化
  ///
  /// カーソルを非表示狀態にする。
  static void hideCursor()
    {
      ::ShowCursor(FALSE);
    }

  /// @brief 「長いファイル名」の取得
  static std::wstring getLongPathName(const std::wstring& path);

  // コマンドライン引數を取得する。
  // 廢用
  //static std::vector<std::string> getCmdLineArgs();


  /// @brief コマンドライン引數の取得
  static std::vector<std::wstring> getCmdLineArgsW();

  /// @brief コマンドライン引數の取得
  /// @param cmdline コマンドライン文字列
  static std::vector<std::wstring> getCmdLineArgsW(const wchar_t* cmdline);

  // ワイド文字列・マルチバイト文字列變換 (Win32API使用)
  /// @brief ワイド文字列からマルチバイト文字列への變換
  static std::string strcpyWideToMultiByte(const std::wstring& ws);
  /// @brief マルチバイト文字列からワイド文字列への變換
  static std::wstring strcpyMultiByteToWide(const std::string& ws);


  /// @brief デスクトップの幅の取得
  static int getDesktopWidth()
  {
    RECT rc;
    ::GetWindowRect(NULL, &rc);
    return rc.right - rc.left;
  }

  /// @brief デスクトップの高さの取得
  static int getDesktopHeight()
  {
    RECT rc;
    ::GetWindowRect(NULL, &rc);
    return rc.bottom - rc.top;
  }
};




template<class Func_>
inline
int urania::System::messageLoop(Func_ f)
{
  MSG msg;
  bool idle = true;

  for (;;) {
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return msg.wParam;
        //舊
        //::PostQuitMessage(0);
        //return;
      }

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);

      idle = true;
    }
    else if (idle) {
      if (!f())
        idle = false;
    }
    else {
      ::WaitMessage();
    }
  }

  return -1; //到達不能の筈だが念の爲
}




#endif // INCLUDE_GUARD_URANIA_SYSTEM_H
