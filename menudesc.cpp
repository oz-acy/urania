/**************************************************************************
 *
 *  menudesc.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  class urania::MenuDesc、class urania::Menu(一部) の實裝
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

urania::RCP_Menu urania::Menu::create(const urania::MenuDesc& desc)
{
  RCP_Menu qm(new Menu);
  qm->link_(desc.createHandle_());
  return qm;
}




//eof
