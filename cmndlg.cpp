/**************************************************************************
 *
 *  cmndlg.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  コモンダイアログラッパークラス定義
 *
 *  履歴
 *    2016.2.29  FileDialogクラスに一本化
 *************************************************************************/

#include <cstring>
#include <sstream>
#include "cmndlg.h"


/*===========================================
 *  FileDialog::FileDialog()
 *=========================================*/
urania::FileDialog::FileDialog(
  const std::wstring& flt, const std::wstring& ext)
: name_(L""), initDir_(L""), filter_(flt), defExt_(ext)
{
  ZeroMemory(&ofn_, sizeof(OPENFILENAME));
  ofn_.lStructSize = sizeof(OPENFILENAME);
  ofn_.lpstrFile = name_;
  ofn_.nMaxFile = MAX_PATH;

  for(auto it = filter_.begin(); it != filter_.end(); ++it) {
    if (*it == L'|')
      *it = L'\0';
  }
}


/*===========================================
 *  FileDialog::getFilePath()
 *=========================================*/
std::wstring urania::FileDialog::getFilePath() const
{
  return name_;
}


/*===========================================
 *  FileDialog::getFileName()
 *=========================================*/
std::wstring urania::FileDialog::getFileName() const
{
  return name_ + ofn_.nFileOffset;
}


/*===========================================
 *  FileDialog::getFileExt()
 *=========================================*/
std::wstring urania::FileDialog::getFileExt() const
{
  if (!ofn_.nFileExtension)
    return L"";
  else
    return name_ + ofn_.nFileExtension;
}


/*===========================================
 *  FileDialog::getFileDir()
 *=========================================*/
std::wstring urania::FileDialog::getFileDir() const
{
  wchar_t tmp[MAX_PATH];
  wcsncpy(tmp, name_, ofn_.nFileOffset);
  tmp[ofn_.nFileOffset] = L'\0';
  return tmp;
}


/*===========================================
 *  FileDialogBase::setFilePath()
 *=========================================*/
void urania::FileDialog::setFilePath(const std::wstring& path)
{
  wcsncpy(name_, path.c_str(), MAX_PATH);
  name_[MAX_PATH - 1] = L'\0';
}


/*===========================================
 *  FileDialogBase::setInitDir()
 *=========================================*/
void urania::FileDialog::setInitDir(const std::wstring& path)
{
  wcsncpy(initDir_, path.c_str(), MAX_PATH);
  initDir_[MAX_PATH - 1] = L'\0';
}




/*===========================================
 *  FileDialog::doModalOpenFile()
 *=========================================*/
bool urania::FileDialog::doModalOpenFile(const urania::WndBase* win)
{
  if (!win)
    ofn_.hwndOwner = NULL;
  else
    ofn_.hwndOwner = getHW__(win);

  if (initDir_[0])
    ofn_.lpstrInitialDir = initDir_;
  else
    ofn_.lpstrInitialDir = nullptr;

  ofn_.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  ofn_.lpstrFilter = filter_.c_str();
  ofn_.lpstrDefExt = defExt_.c_str();

  if (::GetOpenFileName(&ofn_))
    return true;
  else
    return false;
}


/*===========================================
 *  FileDialog::doModalSaveFile()
 *=========================================*/
bool urania::FileDialog::doModalSaveFile(const urania::WndBase* win)
{
  if (!win)
    ofn_.hwndOwner = NULL;
  else
    ofn_.hwndOwner = getHW__(win);

  if (initDir_[0])
    ofn_.lpstrInitialDir = initDir_;
  else
    ofn_.lpstrInitialDir = nullptr;

  ofn_.Flags = OFN_OVERWRITEPROMPT;
  ofn_.lpstrFilter = filter_.c_str();
  ofn_.lpstrDefExt = defExt_.c_str();

  if (::GetSaveFileName(&ofn_))
    return true;
  else
    return false;
}




//eof
