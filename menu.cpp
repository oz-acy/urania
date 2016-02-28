/**************************************************************************
 *
 *  menu.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  class urania::Menu の実装定義
 *
 *  履歴
 *    2016.2.28  修正
 *************************************************************************/

#include "menu.h"
#include "system.h"

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


/*===================================
 *  Menu::getSub()
 *  サブメニューを取得
 */
urania::RCP_Menu urania::Menu::getSub(unsigned id)
{
  if (!hmenu_)
    return RCP_Menu();

  HMENU sub = ::GetSubMenu(hmenu_, id);
  if (!sub)
    return RCP_Menu();

  RCP_Menu mn(new Menu);
  mn->attach__(sub);
  return mn;
}


/*==================================================
 *  Menu::checkItem()
 *  指定アイテムにチェックをつける
 */
void urania::Menu::checkItem(unsigned cmdid)
{
  if (hmenu_)
    ::CheckMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_CHECKED);
}


/*======================================
*  Menu::uncheckItem()
*  指定アイテムのチェックをはずす
*=====================================*/
void urania::Menu::uncheckItem(unsigned cmdid)
{
  if (hmenu_)
    ::CheckMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_UNCHECKED);
}


/*==========================================================
 *  Menu::getItemCheck()
 *  アイテムがチェックされているかどうか調べる
 */
bool urania::Menu::getItemCheck(unsigned cmdid)
{
  if (!hmenu_)
    return false;

  UINT stt = ::GetMenuState(hmenu_, cmdid, MF_BYCOMMAND);
  if (stt == (UINT)-1)
    return false;

  if (stt & MF_CHECKED)
    return true;
  else
    return false;
}


/*=========================================
 *  Menu::enableItem()
 *  アイテムを選択可能にする
 */
void urania::Menu::enableItem(unsigned cmdid)
{
   if (hmenu_)
     ::EnableMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_ENABLED);
}


/*=========================================
 *  Menu::disableItem()
 *  アイテムを選択不可にする
 */
void urania::Menu::disableItem(unsigned cmdid)
{
  if (hmenu_)
    ::EnableMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_DISABLED);
}


/*===============================================
 *  Menu::grayItem()
 *  アイテムを淡色表示(選択不可)にする
 */
void urania::Menu::grayItem(unsigned cmdid)
{
  if (hmenu_)
    ::EnableMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_GRAYED);
}




//eof
