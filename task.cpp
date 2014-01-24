/**************************************************************************
*
*  task.cpp
*  by oZ/acy
*  (c) 2002-2010 oZ/acy.  ALL RIGHTS RESERVED.
*
*  メッセージループ処理 標準
*
*  last update : 2 Sep MMX
**************************************************************************/

#include "sys.h"

/* メッセージループ用の処理 */
/*
bool urania::System::task()
{
  MSG msg;
  if (::GetMessage(&msg, NULL, 0, 0))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    return true;
  }
  else
  {
    ::PostQuitMessage(0);
    return false;
  }
}
*/

void urania::System::start()
{
  MSG msg;

  while (::GetMessage(&msg, NULL, 0, 0))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  ::PostQuitMessage(0);
  return;
}




//eof
