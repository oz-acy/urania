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
 * @file menudesc.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief class urania::MenuDesc、class urania::Menu(一部) の實裝
 *
 */
#include "menu.h"


//////// urania::MenuDesc ////////////////

bool urania::MenuDesc::addPopup(const std::wstring& s)
{
  node_.push_back(Node_(s));
  return true;
}


bool urania::MenuDesc::addPopup(const std::wstring& s,int i)
{
  if (i==Node_::MN_BRANCH || i==Node_::MN_SEPARATOR)
    return false;
  node_.push_back(Node_(s, i));
  return true;
}




HMENU urania::MenuDesc::createHandle_() const
{
  if (node_.size() == 0)
    return NULL;

  HMENU hmenu = CreateMenu();
  for (auto n : node_)
    addPopupMenu_(hmenu, n);
  //for (unsigned i = 0; i < node_.size(); i++)
  //  addPopupMenu_(hmenu, node_[i]);

  return hmenu;
}


void urania::MenuDesc::addPopupMenu_(HMENU pm, const MenuDesc::Node_& item)
{
  if (item.id_ == Node_::MN_SEPARATOR)
    AppendMenu(pm, MF_SEPARATOR, 0, nullptr);
  else if (item.id_ == Node_::MN_BRANCH) {
    HMENU hm = CreatePopupMenu();

    for (auto n : item.ary_)
      addPopupMenu_(hm, n);

    //for (unsigned i=0; i<item.ary_.size(); i++)
    //  addPopupMenu_(hm, item[i]);

    AppendMenu(pm, MF_ENABLED|MF_POPUP, (ULONG_PTR)hm, item.name_.c_str());
  }
  else
    AppendMenu(pm, MF_ENABLED|MF_STRING, item.id_, item.name_.c_str());
}




//////// urania::MenuDesc::Node_ ////////////////

bool urania::MenuDesc::Node_::addSeparator()
{
  if (id_ != MN_BRANCH)
    return false;
  ary_.push_back(Node_());
  return true;
}


bool urania::MenuDesc::Node_::addPopup(const std::wstring& s)
{
  if (id_ != MN_BRANCH)
    return false;
  ary_.push_back(Node_(s));
  return true;
}


bool urania::MenuDesc::Node_::addPopup(const std::wstring& s,int i)
{
  if (id_ != MN_BRANCH)
    return false;
  if (i == MN_BRANCH || i == MN_SEPARATOR)
    return false;
  ary_.push_back(Node_(s, i));
  return true;
}




//// urania::Menu ////////
std::shared_ptr<urania::Menu>
urania::Menu::create(const urania::MenuDesc& desc)
{
  std::shared_ptr<Menu> qm(new Menu);
  qm->link_(desc.createHandle_());
  return qm;
}




//eof
