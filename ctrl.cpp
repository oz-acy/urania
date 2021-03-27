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
 * @file ctrl.cpp
 * @author oZ/acy
 *
 * @date 2016.2.27   修正
 * @date 2018.12.24  修正
 * @date 2019.8.30   修正
 *
 */
#include <memory>
#include "wbase.h"


//// EditBox ////
/*===========================================
 *  WndBase::getTextEB()
 *  エディットボックスの文字列を取得
 */
std::wstring urania::WndBase::getTextEB(int id)
{
  HWND w = GetDlgItem(hw_, id);

  int len = ::SendMessage(w, WM_GETTEXTLENGTH, 0, 0);
  if (len == 0)
    return std::wstring();

  auto tmp = std::make_unique<wchar_t[]>(len + 1);
  len = ::SendMessage(w, WM_GETTEXT, len + 1, (LPARAM)(tmp.get()));

  return std::wstring(tmp.get());
}

/*===========================================
 *  WndBase::getLineTextEB()
 *  指定した行の文字列を取得
 *    id : コントロールID
 *    no : 行番号
 */
std::wstring urania::WndBase::getLineTextEB(int id, int no)
{
  HWND w = ::GetDlgItem(hw_, id);

  int idx = ::SendMessage(w, EM_LINEINDEX, (WPARAM)no, 0L);
  int len = ::SendMessage(w, EM_LINELENGTH, idx, 0);
  auto tmp = std::make_unique<wchar_t[]>(len + 4);
  *(WORD*)(tmp.get()) = (WORD)(len + 4);
  len = ::SendMessage(w, EM_GETLINE, no, (LPARAM)(tmp.get()));
  tmp[len] = L'\0';

  return std::wstring(tmp.get());
}



//// ListBox ////
/*************************************************
 *  WndBase::getItemLB()
 *  リストボックスの項目の文字列を取得
 */
std::wstring urania::WndBase::getItemLB(int id, int no)
{
  HWND w = ::GetDlgItem(hw_, id);
  int len = ::SendMessage(w, LB_GETTEXTLEN, no, 0);
  auto tmp = std::make_unique<wchar_t[]>(len + 1);
  ::SendMessage(w, LB_GETTEXT, no, (LPARAM)(tmp.get()));
  return std::wstring(tmp.get());
}


/***********************************************************
 *  WndBase::dirLB()
 *  リストボックスの内容を指定ディレクトリの内容とする
 */
void urania::WndBase::dirLB(int id, const std::wstring& path, int flag)
{
  HWND w = ::GetDlgItem(hw_, id);
  ::SendMessage(w, LB_DIR, flag, (LPARAM)(path.c_str()));
}


//// ComboBox ////
/*================================================
 *  WndBase::getItemCB()
 *  コンボボックスのリスト項目の文字列を取得する
 */
std::wstring urania::WndBase::getItemCB(int id, int no)
{
  HWND w = ::GetDlgItem(hw_,id);
  int len = ::SendMessage(w, CB_GETLBTEXTLEN, no, 0);
  auto tmp = std::make_unique<wchar_t[]>(len + 1);
  len = ::SendMessage(w, CB_GETLBTEXT, no, (LPARAM)(tmp.get()));
  return std::wstring(tmp.get());
}


/*===============================================================
 *  WndBase::dirCB()
 *  コンボボックスの内容を指定ディレクトリのファイル名にする
 */
void urania::WndBase::dirCB(int id, const std::wstring& path, int flag)
{
   HWND w = ::GetDlgItem(hw_,id);
   ::SendMessage(w, CB_DIR, flag,(LPARAM)(path.c_str()));
}




//eof
