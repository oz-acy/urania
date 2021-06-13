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
 * @file cmndlg.cpp
 * @author oZ/acy
 * @brief コモンダイアログラッパークラス定義
 *
 * @date 2016.2.29  FileDialogクラスに一本化
 * @date 2021.3.27  修正
 *
 * @date 2021.6.11
 *   依據ライブラリをthemis+polymniaからeunomiaに切り替へるための修正
 *
 */
#include <sstream>
#include "cmndlg.h"


/*===========================================
 *  FileDialog::FileDialog()
 */
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
 *  FileDialog::doModalOpenFile()
 *=========================================*/
bool urania::FileDialog::doModalOpenFile(const urania::WndBase* win)
{
  if (!win)
    ofn_.hwndOwner = NULL;
  else
    ofn_.hwndOwner = WndBase::getHWND(win);

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
    ofn_.hwndOwner = WndBase::getHWND(win);

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
