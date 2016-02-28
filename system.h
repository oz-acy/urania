/**************************************************************************
 *
 *  system.h
 *  by oZ/acy
 *  (c) 2001-2016 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  SYStem manage
 *
 *  履歴
 *    14 Feb 2004  System::getLongPathName() を追加
 *    27 Feb 2008  System::getDesktopWidth() を追加
 *                 System::getDesktopHeight() を追加
 *     4 Apr 2009  UNICODE對應
 *     7 Nov 2009  System::cnvWStr2MBStr() を追加
 *     2 Sep 2010  task系削除・start系追加
 *    13 May 2012  C++11對應
 *    27 Feb 2016  ファイル名變更、メソッド名變更
 *************************************************************************/

#ifndef INC_GP_GUI_SYSTEM_H__
#define INC_GP_GUI_SYSTEM_H__

#include <windows.h>
#include <string>
#include <vector>
#include "decl.h"


//int PASCAL WinMain(HINSTANCE hi, HINSTANCE pv, LPSTR cl, int cs);


/*---------------------
 *  class System
 *-------------------*/
class urania::System
{
  friend class urania::WndBase;
  friend class urania::Window;
  friend class urania::Dialog;
  friend class urania::Menu;
  friend int PASCAL ::WinMain(HINSTANCE hi, HINSTANCE pv, LPSTR cl, int cs);

private:
  static HINSTANCE hi_S;

private:
  System() =delete; //インスタンス生成禁止

public:
  //=============================================================
  //  void start()
  //  開始
  //  具體的にはシステムから渡されたHINSTANCE値を格納しておく
  //=============================================================
  static void start(HINSTANCE hi)
  {
    hi_S = hi;
  }

  //=======================================================
  //  void quit()
  //  メッセージキューに終了を投げる
  //=======================================================
  static void quit(int r)
  {
    ::PostQuitMessage(r);
  }

  static void messageLoop();
  template<class Func_> static void messageLoop(Func_ f);
  //static void startRT();


  // メッセージボックス系

  //=======================================================
  //  static void alert()
  //  警告用メッセージボックスを表示
  //=======================================================
  static void alert(const std::wstring& ttl, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_OK | MB_ICONEXCLAMATION);
  }

  //=======================================================
  //  static void notify()
  //  通知用のメッセージボックスを表示
  //=======================================================
  static void notify(const std::wstring& ttl, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_OK);
  }

  //=======================================================
  //  static askYesNoCancel()
  //  Yes, No, キャンセル三擇のメッセージボックスを表示 
  //
  //  戻り値: 1: OK, 2: NO, 3: CANCEL のいづれか
  //=======================================================
  static int askYesNoCancel(const std::wstring& ttl, const std::wstring& msg)
  {
    int res = ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_YESNOCANCEL);
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

  //=======================================================
  //  static askYNBox()
  //  はい、いいえ二擇のメッセージボックスを表示
  //
  //  戻り値: true: Yes, false: No のいづれか
  //=======================================================
  static bool askYesNo(const std::wstring& ttl, const std::wstring& msg)
  {
    int res = ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_YESNO);
    if (res == IDYES)
      return true;
    else
      return false;
  }



  // カーソル系
  static void showCursor()
    { 
      ::ShowCursor(TRUE);
    }
  static void hideCursor()
    {
      ::ShowCursor(FALSE);
    }

  // ファイル名關聯
  static std::wstring getLongPathName(const std::wstring& path);

  // コマンドライン引數取得
  static std::vector<std::string> getCmdLineArgs();
  static std::vector<std::wstring> getCmdLineArgsW();

  // ワイド文字列・マルチ文字列變換 (Win32API使用)
  static std::string strcpyWideToMultiByte(const std::wstring& ws);
  static std::wstring strcpyMultiByteToWide(const std::string& ws);


  // デスクトップ情報取得
  static int getDesktopWidth()
  {
    RECT rc;
    ::GetWindowRect(NULL, &rc);
    return rc.right - rc.left;
  }

  static int getDesktopHeight()
  {
    RECT rc;
    ::GetWindowRect(NULL, &rc);
    return rc.bottom - rc.top;
  }

};


/*===============================================================
 *  System::messageLoop(f)
 *  Windows Application のメッセージループ
 *  ウィンドウメッセージがない場合にコールバックを呼び出す。
 *  コールバックはboolを返し、falseを返した場合は、
 *  次にウィンドウメッセージを處理した後までコールバックしない。
 */
template<class Func_>
void urania::System::messageLoop(Func_ f)
{
  MSG msg;
  static bool idle = true;

  for (;;) {
    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message==WM_QUIT)
      {
        ::PostQuitMessage(0);
        return;
      }

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);

      idle = true;
    }
    else if (idle)
    {
      if (!f())
        idle = false;
    }
    else
    {
      ::WaitMessage();
    }
  }
}




#endif
