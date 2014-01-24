/**************************************************************************
 *
 *  menu.cpp
 *  by oZ/acy
 *  (c) 2002-2009 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  class urania::Menu の実装定義
 *
 *  last update: 4 Apr MMIX
 *
 *************************************************************************/

#include "menu.h"
#include "sys.h"

/*====================================
*  Menu::create()
*  リソースIDからMenuを生成
*/
urania::RCP_Menu urania::Menu::create(int rc)
{
  RCP_Menu qm(new Menu);
  qm->link__(::LoadMenu(System::hi_S, MAKEINTRESOURCE(rc)));
  return qm;
}




//eof
