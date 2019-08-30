/**************************************************************************
 *
 *  menu.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  urania::Menu の實裝
 *
 *  履歴
 *    2016.2.28  修正
 *    2019.8.30  修正
 */
#include "menu.h"
#include "system.h"

urania::RCP_Menu urania::Menu::create(int rc)
{
  RCP_Menu qm(new Menu);
  qm->link_(::LoadMenu(System::hi_S, MAKEINTRESOURCE(rc)));
  return qm;
}


urania::RCP_Menu urania::Menu::getSub(unsigned id)
{
  if (!hmenu_)
    return RCP_Menu();

  HMENU sub = ::GetSubMenu(hmenu_, id);
  if (!sub)
    return RCP_Menu();

  RCP_Menu mn(new Menu);
  mn->attach_(sub);
  return mn;
}


void urania::Menu::checkItem(unsigned cmdid)
{
  if (hmenu_)
    ::CheckMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_CHECKED);
}


void urania::Menu::uncheckItem(unsigned cmdid)
{
  if (hmenu_)
    ::CheckMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_UNCHECKED);
}


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


void urania::Menu::enableItem(unsigned cmdid)
{
   if (hmenu_)
     ::EnableMenuItem(hmenu_, cmdid, MF_BYCOMMAND|MF_ENABLED);
}


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
