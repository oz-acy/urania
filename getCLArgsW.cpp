/**************************************************************************
 *
 *  getCLArgsW.cpp
 *  by oZ/acy
 *  (c) 2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  履歴
 *    27 Feb 2016  getCmdLineArgs() の wchar_t 版を作成
 *************************************************************************/

#include "system.h"

// もしかしてコンパイラ依存??
extern int __argc;
extern wchar_t** __wargv;


/*===========================================
*  コマンドライン引數を取得(wchar_t版)
*/
std::vector<std::wstring> urania::System::getCmdLineArgsW()
{
  std::vector<std::wstring> args(__argc);

  for (int i = 0; i < __argc; i++)
    args[i] = __wargv[i];

  return args;
}


//eof
