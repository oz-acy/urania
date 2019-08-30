/**
 * @file system.h
 * @brief システム周り
 * @author oZ/acy
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
 */
#ifndef INCLUDE_GUARD_URANIA_SYSTEM_H
#define INCLUDE_GUARD_URANIA_SYSTEM_H

#include <windows.h>
#include <string>
#include <vector>
#include "decl.h"


/**
 * システム周りのあれこれ
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
  /// 利用開始する。最初に一度だけ呼ぶ。
  /// @param[in] hi HINSTANCE値。WinMain()の1つ目の引數を渡す。
  static void start(HINSTANCE hi)
  {
    hi_S = hi;
  }

  /// メッセージキューに終了を投げる。
  static void quit(int r)
  {
    ::PostQuitMessage(r);
  }

  /// メッセージループ
  static void messageLoop();

  /// コールバック附のメッセージループ。
  /// @param f
  ///   アイドル状態の時に呼び出されるコールバック函數。
  ///   引數を取らず、bool値を返す。
  ///   falseを返した場合、次にメッセージを處理するまでコールバックしない。
  template<class Func_> static void messageLoop(Func_ f);


  // メッセージボックス系

  /// @brief 警告用のメッセージボックスを表示する
  /// @param[in] title タイトル
  /// @param[in] msg メッセージ
  static void alert(const std::wstring& title, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONEXCLAMATION);
  }

  /// @brief 通知用のメッセージボックスを表示する
  /// @param[in] title タイトル
  /// @param[in] msg メッセージ
  static void notify(const std::wstring& title, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), title.c_str(), MB_OK);
  }

  //=======================================================
  //  static askYesNoCancel()
  //  Yes, No, キャンセル三擇のメッセージボックスを表示 
  //
  //  戻り値: 1: OK, 2: NO, 3: CANCEL のいづれか
  //=======================================================
  /// @brief Yes, No, キャンセル三擇のメッセージボックスを表示
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

  /// @brief はい、いいえ二擇のメッセージボックスを表示
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
  /// @brief カーソルを表示する
  static void showCursor()
    { 
      ::ShowCursor(TRUE);
    }
  /// @brief カーソルを非表示にする
  static void hideCursor()
    {
      ::ShowCursor(FALSE);
    }

  // ファイル名關聯
  /// @brief 「長いファイル名」を取得する
  static std::wstring getLongPathName(const std::wstring& path);

  // コマンドライン引數取得
  /// @brief コマンドライン引數を取得する
  static std::vector<std::string> getCmdLineArgs();
  /// @brief コマンドライン引數を取得する
  static std::vector<std::wstring> getCmdLineArgsW();

  // ワイド文字列・マルチバイト文字列變換 (Win32API使用)
  /// @brief ワイド文字列をマルチバイト文字列に變換する
  static std::string strcpyWideToMultiByte(const std::wstring& ws);
  /// @brief マルチバイト文字列をワイド文字列に變換する
  static std::wstring strcpyMultiByteToWide(const std::string& ws);


  // デスクトップ情報取得
  /// デスクトップの幅を取得する。
  static int getDesktopWidth()
  {
    RECT rc;
    ::GetWindowRect(NULL, &rc);
    return rc.right - rc.left;
  }

  /// デスクトップの高さを取得する。
  static int getDesktopHeight()
  {
    RECT rc;
    ::GetWindowRect(NULL, &rc);
    return rc.bottom - rc.top;
  }

};


template<class Func_>
inline
void urania::System::messageLoop(Func_ f)
{
  MSG msg;
  static bool idle = true;

  for (;;) {
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message==WM_QUIT) {
        ::PostQuitMessage(0);
        return;
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
}




#endif // INCLUDE_GUARD_URANIA_SYSTEM_H
