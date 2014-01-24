/**************************************************************************
 *
 *  sys.h
 *  by oZ/acy
 *  (c) 2001-2012 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  SYStem manage
 *
 *  履歴
 *    14 Feb 2004:  System::getLongPathName() を追加
 *    27 Feb 2008:  System::getDesktopWidth() を追加
 *                  System::getDesktopHeight() を追加
 *     4 Apr 2009:  UNICODE對應
 *     7 Nov 2009:  System::cnvWStr2MBStr() を追加
 *     2 Sep 2010:  task系削除・start系追加
 *    13 May 2012:  C++11對應
 *************************************************************************/

#ifndef INC_GP_GUI_SYS_H__
#define INC_GP_GUI_SYS_H__

#include <windows.h>
#include <string>
#include <vector>
#include "dec.h"


int PASCAL WinMain(HINSTANCE hi, HINSTANCE pv, LPSTR cl, int cs);


/*---------------------
*  class System
*--------------------*/
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
  //=======================================================
  //  void quit()
  //  メッセージキューに終了を投げる
  //=======================================================
  static void quit(int r)
  {
    ::PostQuitMessage(r);
  }


  // メッセージループ系

  //static bool task();
  ////static bool taskMDI(urania::MDIFrame*);
  //static bool taskRT();
  ////static bool taskRT_MDI(urania::MDIFrame*);

  //template<class Func_>
  //static bool taskIdle(Func_ f);

  static void start();
  static void startRT();

  template<class Func_>
  static void start(Func_ f);


  // メッセージボックス系

  //=======================================================
  //  static void alertBox()
  //  警告用メッセージボックスを表示
  //=======================================================
  static void alertBox(const std::wstring& ttl, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_OK|MB_ICONEXCLAMATION);
  }

  //=======================================================
  //  static void msgBox()
  //  OK限定のメッセージボックスを表示
  //=======================================================
  static void msgBox(const std::wstring& ttl, const std::wstring& msg)
  {
    ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_OK);
  }

  //=======================================================
  //  static askYNCBox()
  //  Yes, No, キャンセル3擇のメッセージボックスを表示 
  //
  //  戻り値: 1: OK, 2: NO, 3: CANCEL のいづれか
  //=======================================================
  static int askYNCBox(const std::wstring& ttl, const std::wstring& msg);

  //=======================================================
  //  static askYNBox()
  //  はい、いいえ2擇のメッセージボックスを表示
  //
  //  戻り値: true: Yes, false: No のいづれか
  //=======================================================
  static bool askYNBox(const std::wstring& ttl, const std::wstring& msg)
  {
    int res = ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_YESNO);
    if (res==IDYES)
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

  // ワイド文字列・マルチ文字列變換 (Win32API使用)
  static std::string cnvWStr2MBStr(const std::wstring& ws);
  static std::wstring cnvMBStr2WStr(const std::string& ws);


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


/*
template<class Func_>
bool urania::System::taskIdle(Func_ f)
{
  MSG msg;
  static bool idle = false;

  if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    if (msg.message==WM_QUIT)
    {
      ::PostQuitMessage(0);
      return false;
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

  return true;      
}
*/


template<class Func_>
void urania::System::start(Func_ f)
{
  MSG msg;
  static bool idle = false;

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
