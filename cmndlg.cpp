/**************************************************************************
 *
 *  cmndlg.cpp
 *  by oZ/acy
 *  (c) 2002-2009 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  コモンダイアログラッパークラス定義
 *
 *  last update : 4 Apr 2009
 *
 *************************************************************************/
#include "cmndlg.h"
#include <cstring>
#include <sstream>


/*===========================================
 *  FileDialogBase::FileDialogBase()
 *=========================================*/
urania::FileDialogBase::FileDialogBase(
  const std::wstring& flt, const std::wstring& ext)
{
  using namespace std;

  int len = ext.size() >= 3 ? 3 : ext.size();
  wcsncpy(def_ext_, ext.c_str(), 3);
  def_ext_[3] = '\0';

  wcsncpy(filter_, flt.c_str(), 255);
  filter_[255] = '\0';

  name_[0] = '\0';

  for (int i=0; filter_[i]; i++)
    if (filter_[i]=='|' || filter_[i]=='/')
      filter_[i] = '\0';

  ZeroMemory(&ofn_, sizeof(OPENFILENAME));

  ofn_.lStructSize = sizeof(OPENFILENAME);

  ofn_.lpstrFilter = filter_;
  ofn_.nFilterIndex = 0;

  ofn_.lpstrFile = name_;
  ofn_.nMaxFile = 256;
  ofn_.lpstrDefExt = def_ext_;
}


/*===========================================
 *  FileDialogBase::getPath()
 *=========================================*/
std::wstring urania::FileDialogBase::getPath() const
{
  return name_;
}

/*===========================================
 *  FileDialogBase::getTitle()
 *=========================================*/
std::wstring urania::FileDialogBase::getTitle() const
{
  return name_ + ofn_.nFileOffset;
}

/*===========================================
 *  FileDialogBase::getExt()
 *=========================================*/
std::wstring urania::FileDialogBase::getExt() const
{
  if (!ofn_.nFileExtension)
    return L"";
  else
    return name_ + ofn_.nFileExtension;
}

/*===========================================
 *  FileDialogBase::setPath()
 *=========================================*/
void urania::FileDialogBase::setPath(const std::wstring& path)
{
  using namespace std;

  wcsncpy(name_, path.c_str(), 255);
  name_[255] = '\0';
}



/*===========================================
 *  OpenFileDialog::doModal()
 *=========================================*/
bool urania::OpenFileDialog::doModal(const urania::WndBase* win)
{
  if (!win)
    ofn_.hwndOwner = NULL;
  else
    ofn_.hwndOwner = getHW__(win);

  if (::GetOpenFileName(&ofn_))
    return true;
  else
    return false;
}


/*===========================================
 *  SaveFileDialog::doModal()
 *=========================================*/
bool urania::SaveFileDialog::doModal(const urania::WndBase* win)
{
  if (!win)
    ofn_.hwndOwner = NULL;
  else
    ofn_.hwndOwner = getHW__(win);

  if (::GetSaveFileName(&ofn_))
    return true;
  else
    return false;
}


//eof
