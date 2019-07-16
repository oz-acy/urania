/**************************************************************************
 *
 *  msgloop.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  メッセージループ処理 標準
 *
 *  履歴
 *    2016.2.27  ファイル名變更、start() を messageLoop()に改名
 *************************************************************************/

#include "system.h"

void urania::System::messageLoop()
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
