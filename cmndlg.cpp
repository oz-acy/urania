/**************************************************************************
 *
 *  cmndlg.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  コモンダイアログラッパークラス定義
 *
 *  履歴
 *    2016.2.28  修正
 *************************************************************************/

#include <cstring>
#include <sstream>
#include "cmndlg.h"


/*===========================================
 *  FileDialogBase::FileDialogBase()
 *=========================================*/
urania::FileDialogBase::FileDialogBase(
  const std::wstring& flt, const std::wstring& ext)
: name_(L""), initDir_(L""), filter_(flt), defExt_(ext)
{
  ZeroMemory(&ofn_, sizeof(OPENFILENAME));
  ofn_.lStructSize = sizeof(OPENFILENAME);
  ofn_.lpstrFile = name_;
  ofn_.nMaxFile = MAX_PATH;
}


/*===========================================
 *  FileDialogBase::getFilePath()
 *=========================================*/
std::wstring urania::FileDialogBase::getFilePath() const
{
  return name_;
}


/*===========================================
 *  FileDialogBase::getFileName()
 *=========================================*/
std::wstring urania::FileDialogBase::getFileName() const
{
  return name_ + ofn_.nFileOffset;
}


/*===========================================
 *  FileDialogBase::getFileExt()
 *=========================================*/
std::wstring urania::FileDialogBase::getFileExt() const
{
  if (!ofn_.nFileExtension)
    return L"";
  else
    return name_ + ofn_.nFileExtension;
}


/*===========================================
 *  FileDialogBase::getFileDir()
 *=========================================*/
std::wstring urania::FileDialogBase::getFileDir() const
{
  wchar_t tmp[MAX_PATH];
  wcsncpy(tmp, name_, ofn_.nFileOffset);
  tmp[ofn_.nFileOffset] = L'\0';
  return tmp;
}


/*===========================================
 *  FileDialogBase::setFilePath()
 *=========================================*/
void urania::FileDialogBase::setFilePath(const std::wstring& path)
{
  wcsncpy(name_, path.c_str(), MAX_PATH);
  name_[MAX_PATH - 1] = L'\0';
}


/*===========================================
 *  FileDialogBase::setInitDir()
 *=========================================*/
void urania::FileDialogBase::setInitDir(const std::wstring& path)
{
  wcsncpy(initDir_, path.c_str(), MAX_PATH);
  initDir_[MAX_PATH - 1] = L'\0';
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

  if (initDir_[0])
    ofn_.lpstrInitialDir = initDir_;
  else
    ofn_.lpstrInitialDir = nullptr;


  ofn_.lpstrFilter = filter_.c_str();
  ofn_.lpstrDefExt = defExt_.c_str();

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

  if (initDir_[0])
    ofn_.lpstrInitialDir = initDir_;
  else
    ofn_.lpstrInitialDir = nullptr;

  ofn_.lpstrFilter = filter_.c_str();
  ofn_.lpstrDefExt = defExt_.c_str();

  if (::GetSaveFileName(&ofn_))
    return true;
  else
    return false;
}


//eof
