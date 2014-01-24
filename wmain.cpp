/**************************************************************************
*
*  wmain.cpp
*  by oZ/acy
*  (c) 2002-2004 oZ/acy.  ALL RIGHTS RESERVED.
*
*  WinMain()
*
*  履歴
*    14 Feb 2004 : userMain() の引數廢止
**************************************************************************/

#include "sys.h"

// ライブラリ使用者の実質的開始地点
int userMain();


/*=======================
*  WinMain()
*======================*/
int PASCAL WinMain(HINSTANCE hi, HINSTANCE pv, LPSTR cl, int cs)
{
  (void)pv;
  (void)cl;
  (void)cs;

  urania::System::hi_S = hi;
  return userMain();
}


//eof
