/**************************************************************************
*
*  task3.cpp
*  by oZ/acy
*  (c) 2002-2010 oZ/acy. ALL RIGHTS RESERVED.
*
*
*  實時間處理向けメッセージ處理
*
*  last update: 2 Sep MMX
**************************************************************************/

#include "sys.h"


/* DirectX などの實時間處理でのメッセージループ處理用 */
/*
bool urania::System::taskRT()
{
  MSG msg;
  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    if (msg.message==WM_QUIT)
    {
      ::PostQuitMessage(0);
      return false;
    }

    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
  return true;
}
*/


void urania::System::startRT()
{
  MSG msg;
  for (;;) {
    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message==WM_QUIT)
      {
        ::PostQuitMessage(0);
        return;
      }

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }
}




//eof
