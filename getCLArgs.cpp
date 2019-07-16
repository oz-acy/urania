/**************************************************************************
 *
 *  getCLArgs.cpp
 *  by oZ/acy
 *  (c) 2004-2008 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  履歴
 *    14 Feb 2004  作成
 *    30 Mar 2008  修正
 *    27 Feb 2016  修正
 *************************************************************************/

#include "system.h"

// もしかしてコンパイラ依存??
extern int __argc;
extern char** __argv;


/*===========================================
*  コマンドライン引數を取得
*/
std::vector<std::string> urania::System::getCmdLineArgs()
{
  std::vector<std::string> args(__argc);

  for (int i = 0; i < __argc; i++)
    args[i] = __argv[i];

  return args;
}


//eof
