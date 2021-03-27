/*
 * Copyright 2002-2021 oZ/acy (名賀月晃嗣)
 * Redistribution and use in source and binary forms, 
 *     with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * @file menu.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief urania::Menuの實裝
 *
 * @date 2016.2.28  修正
 * @date 2019.8.30  修正
 * @date 2021.3.26  修正
 *
 */
#include "menu.h"
#include "system.h"

std::shared_ptr<urania::Menu> urania::Menu::create(int rc)
{
  std::shared_ptr<Menu> qm(new Menu);
  qm->link_(::LoadMenu(System::hi_S, MAKEINTRESOURCE(rc)));
  return qm;
}

std::shared_ptr<urania::Menu> urania::Menu::getSub(unsigned id)
{
  if (!hmenu_)
    return std::shared_ptr<Menu>();

  HMENU sub = ::GetSubMenu(hmenu_, id);
  if (!sub)
    return std::shared_ptr<Menu>();

  std::shared_ptr<Menu> mn(new Menu);
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
